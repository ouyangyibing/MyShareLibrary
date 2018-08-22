#ifndef MYDEBUG_H
#define MYDEBUG_H

#include <QObject>
#include <QMetaType>
#include <QDebug>

/*************************************
 * DEFINES += QT_MESSAGELOGCONTEXT
 * 使用前在pro文件中加上这句话
 * **********************************/

class QThread;
class MyDebug;

//外部也需要的数据类
class MyDebugData
{
public:
    MyDebugData() {}

    int version;
    int line;
    QByteArray file;
    QByteArray function;
    QByteArray category;
};
Q_DECLARE_METATYPE(MyDebugData)

//外部类不能使用，只作为MyDebug 的私有类
class MyDebugDataProcess : public QObject
{
    Q_OBJECT

    MyDebugDataProcess(QObject *parent = nullptr) : QObject(parent) {}
    friend class MyDebug;
public:

    bool getStartFlag() const;
    void setStartFlag(bool value);

signals:
    void sigDebugStrData(const QString &);
    void sigDebugHtmlData(const QString &);
public slots:
    void slotDataProcess(QtMsgType type, const MyDebugData &context, const QString &msg);
    QString logPath();
    QString logName();

private:
    bool startFlag = false;
};

//外部需要用到的类
class MyDebug : public QObject
{
    Q_OBJECT

    explicit MyDebug(QObject *parent = nullptr);
public:
    static  MyDebug* Instance();
    void installMessageHandler();
    void uninstallMessageHandler();
    QString logPath();
    QString logName();
    void setLogStartFlag();

signals:
    void sigDebugMsgData(QtMsgType type,const MyDebugData &context, const QString &msg);
    void sigDebugData(const QString &);
    void sigDebugHtmlData(const QString &);

private:
    QThread* thread;
    MyDebugDataProcess *debugDataProcess;

    static MyDebug* self;
};

#endif // MYDEBUG_H
