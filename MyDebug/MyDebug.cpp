#include "MyDebug.h"
#include <QThread>
#include <QDateTime>
#include <QCoreApplication>
#include <QMutex>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <iostream>
#include <QMetaType>

/***************************************************
 * 类说明:
 * 调用函数 void MyDebug::installMessageHandler() 安装
 * 消息处理器, 而后在使用 qDebug、 qWarning 等debug函数的
 * 时候将会自动调用消息处理器outputMessage(); 在消息处理器
 * 中，会将数据发送至 MyDebug的实例，再由MyDebug的实例，将
 * 数据传入另一线程处理
 * 其实在使用的时候并不需要了解本类的实现过程
 * 只需要在main 函数中的应用程序对象QApplication a(argc, argv);
 * 建立起来后放置下面这句话就好了
 * MyDebug::Instance()->installMessageHandler();
 * 后期使用时，如果是异常，需要记录至log中，就使用qWarning
 * 用法和以前一样
 * ************************************************/

/***************************************************
 * 使用宏 qInstallMessageHandler
 * 安装本函数作为消息处理函数
 * QMessageLogContext 此类不能作为信号发送数据的类型
 * 我也很奇怪为什么不行
 * 最后将数据发送至MyDebug的实例
 * ************************************************/
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    MyDebugData data;
    data.version  = context.version;
    data.line     = context.line;
    data.file     = context.file;
    data.function = context.function;
    data.category = context.category;

    MyDebug::Instance()->sigDebugMsgData(type, data, msg);
}

/***************************************************
 * qDebug 消息不出来，直接打印出来
 * qWarning 和其他类型debug将会处理后，将数据写入log
 * 并用发信号的方式再次发送出来
 * ************************************************/
void MyDebugDataProcess::slotDataProcess(QtMsgType type, const MyDebugData &context, const QString &msg)
{
    static QMutex mutex;
    QMutexLocker lock(&mutex);

    QString text;    
    switch(type)
    {
    //如果是debug那么直接打印消息，然后退出本函数
    case QtDebugMsg:
        std::cout << msg.toStdString() << std::endl;
        return ;
        //text = QString("Debug------------------------------------------------------------------------------------");
        break;

        //如果是警告，则继续执行后面的数据处理
    case QtWarningMsg:
        text = QString("Warning...............................");
        break;

    case QtCriticalMsg:
        text = QString("Critical..............................");
        break;

    case QtFatalMsg:
        text = QString("Fatal.................................");
        break;

    default:
        text = QString("Default...............................");
        break;
    }
    QString context_info = QString("File: %1\r\nFunc: %2\r\nLine: %3")
            .arg(QString(context.file))
            .arg(QString(context.function))
            .arg(context.line);

    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");

    QString current_date = QString("Time: %1")
            .arg(current_date_time);

    QString message = QString("%1\r\n%2\r\n%3\r\n%4")
            .arg(text)
            .arg(current_date)
            .arg(context_info)
            .arg("MSG : "+msg);

    QString logPahtHtml = "<a href = \"%1\">Click here to open the log directory.</a>\r\n";
    logPahtHtml = logPahtHtml.arg(QCoreApplication::applicationDirPath()+"/"+logPath());

    QString msgHtml = msg;
    msgHtml.replace("  ", "&nbsp;");
    msgHtml.remove("\r");
    msgHtml.replace("\n", "<br>");
    msgHtml = QString("<font color=red>" + msgHtml + "</font>");

    QString contextInfoHtml = context_info;
    contextInfoHtml.remove("\r");
    contextInfoHtml.replace("\n", "<br>");

    QString messageHtml = QString("%1<br>%2<br>%3<br>%4<br>%5")
            .arg(text)
            .arg(logPahtHtml)
            .arg(current_date)
            .arg(contextInfoHtml)
            .arg("MSG : "+msgHtml);

    //写入文件
    QFile file(logPath() + logName());
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    if (startFlag) {
        startFlag = false;
        text_stream << "Test_Start~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n";
    }
    text_stream << message << "\r\n\r\n\r\n";
    file.flush();
    file.close();
    //将处理好的数据发送出去
    emit sigDebugStrData(message);
    emit sigDebugHtmlData(messageHtml);
    //emit sigDebugHtmlData(messageHtml);
    //如果文件没到达限定的大小，则退出
    if(file.size() < 1024*1024) {
        return ;
    }
    //log达到了限制值则将名字更改
    for(int loop = 1; loop < 100; ++loop) {
        QString fileName = QString("%1/log_%2.txt").arg(logPath()).arg(loop);
        QFile file_1(fileName);
        if(file_1.size() < 4) {
            file.rename(fileName);
            return ;
        }
    }
}

QString MyDebugDataProcess::logPath()
{
    //建立文件路径
    QString current_date_file_name = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QDir dir(QString("log/%1").arg(current_date_file_name));
    if(!dir.exists()) {
        dir.mkpath("./");
    }
    return dir.path() + "/" ;
}

QString MyDebugDataProcess::logName()
{
    return "log.txt";
}

bool MyDebugDataProcess::getStartFlag() const
{
    return startFlag;
}

void MyDebugDataProcess::setStartFlag(bool value)
{
    startFlag = value;
}

//MyDebug单例
MyDebug* MyDebug::self = nullptr;
MyDebug* MyDebug::Instance()
{
    if(!self) {
        QMutex muter;
        QMutexLocker clocker(&muter);

        if(!self) {
            self = new MyDebug();
        }
    }
    return self;
}

//消息器安装
void MyDebug::installMessageHandler()
{
    qInstallMessageHandler(outputMessage);
}

//消息器卸载
void MyDebug::uninstallMessageHandler()
{
    qInstallMessageHandler(0);
}

QString MyDebug::logPath()
{
    return debugDataProcess->logPath();
}

QString MyDebug::logName()
{
    return debugDataProcess->logName();
}

void MyDebug::setLogStartFlag()
{
    debugDataProcess->setStartFlag(true);
}

//MyDebug 构造函数，
MyDebug::MyDebug(QObject *parent) : QObject(parent)
{
    //注册元对象类型，供信号发送使用
    qRegisterMetaType<QtMsgType>("QtMsgType");
    qRegisterMetaType<MyDebugData>("MyDebugData");

    //创建一个线程，并将处理debug数据的对象移动至线程
    thread = new QThread();
    debugDataProcess = new MyDebugDataProcess();
    debugDataProcess->moveToThread(thread);
    thread->start();
    //将本类的收到的debug消息数据信号连接至处理数据类中的处理函数
    connect(this, &MyDebug::sigDebugMsgData, debugDataProcess, &MyDebugDataProcess::slotDataProcess);
    //数据处理类将会在数据处理完成后将数据发送出去，现将此信号绑定至本类信号，方便外类使用
    connect(debugDataProcess, &MyDebugDataProcess::sigDebugStrData, this, &MyDebug::sigDebugData);
    connect(debugDataProcess, &MyDebugDataProcess::sigDebugHtmlData, this, &MyDebug::sigDebugHtmlData);
}
