#include "MyNameSpace.h"
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QCoreApplication>
#include <QMutex>
#include <QFile>
#include <QDir>

void MY::DelayMs(int time)
{        
    QEventLoop LoopEvent;
    QTimer::singleShot(time,&LoopEvent, SLOT(quit()));
    LoopEvent.exec();
}

/***************************************************
 * 此函数可做超时
 **************************************************/
void MY::DelayMs(QEventLoop &LoopEvent, int time)
{
    QTimer timer;
    QObject::connect(&timer,SIGNAL(timeout()),&LoopEvent,SLOT(quit()));
    timer.start(time);
    LoopEvent.exec();
    timer.stop();
}

/*******************************************************************
 * 本函数用于截取两字符串中间的字符
 * strData 需要处理的数据
 * strFront 需取出的
 * ****************************************************************/
QString MY::mid(QString strData, QString strFront, QString strBack)
{
    int Position_Begin = strData.indexOf(strFront) + strFront.length();
    int Position_End = strData.indexOf(strBack,Position_Begin);
    if (strData.indexOf(strFront) == -1) {
        return "null";
    } else {
        return strData.mid(Position_Begin,Position_End-Position_Begin).trimmed().toLocal8Bit();
    }
}

QRegularExpressionMatch MY::mid(QString strData, QString regularExpression)
{
    QRegularExpression re(regularExpression);
    return re.match(strData);
}

QString MY::getCurentTime(const QString &format)
{
    return QDateTime::currentDateTime().toString(format);
}

QString MY::getAppPath()
{
    return QCoreApplication::applicationDirPath();
}

void MY::outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    QMutexLocker lock(&mutex);

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug------------------------------------------------------------------------------------\r\n");
        break;

    case QtWarningMsg:
        text = QString("Warning----------------------------------------------------------------------------------\r\n");
        break;

    case QtCriticalMsg:
        text = QString("Critical---------------------------------------------------------------------------------\r\n");
        break;

    case QtFatalMsg:
        text = QString("Fatal------------------------------------------------------------------------------------\r\n");

    default:
        text = QString("Default----------------------------------------------------------------------------------\r\n");
    }
    QString context_info = QString("File: %1\r\nFunc: %2\r\nLine: %3\r\n")
            .arg(QString(context.file))
            .arg(QString(context.function))
            .arg(context.line);

    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");

    QString current_date = QString("Time: %1\r\n")
            .arg(current_date_time);

    QString message = QString("%1%2%3%4")
            .arg(text)
            .arg(current_date)
            .arg(context_info)
            .arg("MSG : "+msg);

    QString current_date_file_name = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QDir dir(QString("log/%1").arg(current_date_file_name));
    if(!dir.exists()) {
        dir.mkpath("./");
    }

    QFile file(QString("%1/log.txt").arg(dir.path()));
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n\r\n\r\n";
    file.flush();
    file.close();

    if(file.size() < 1024*1024) {
        return ;
    }

    for(int loop = 1; loop < 100; ++loop) {
        QString fileName = QString("%1/log_%2.txt").arg(dir.path()).arg(loop);
        QFile file_1(fileName);
        if(file_1.size() < 4) {
            file.rename(fileName);
            return ;
        }
    }
}

