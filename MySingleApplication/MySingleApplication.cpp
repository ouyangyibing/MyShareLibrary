#include "MySingleApplication.h"
#include <QLocalSocket>
#include <QFileInfo>
#include <QDebug>
#include <QLocalSocket>
#include <QLocalServer>
#include <QWidget>

#define TIME_OUT                (500)    // 500ms

MySingleApplication::MySingleApplication(int &argc, char **argv)
    : QApplication(argc, argv)
    , _widget(NULL)
    , _isRunning(false)
    , _localServer(NULL) {

    // 取应用程序名作为LocalServer的名字
    //_serverName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    _serverName = QCoreApplication::applicationFilePath()+_serverName;

    _initLocalConnection();
}

/******************************************************************
 * 检查是否已經有一个实例在运行, true - 有实例运行， false - 没有实例运行
 * ***************************************************************/
bool MySingleApplication::isRunning()
{
    return _isRunning;
}

bool MySingleApplication::isNoRunning()
{
    return !_isRunning;
}
/**********************************************************
 * 通过socket通讯实现程序单实例运行，监听到新的连接时触发该函数
 * *******************************************************/
void MySingleApplication::_newLocalConnection()
{
    QLocalSocket *socket = _localServer->nextPendingConnection();
    if(socket) {
        socket->waitForReadyRead(2*TIME_OUT);
        delete socket;
        // 其他处理，如：读取启动参数

        _activateWindow();
    }
}

void MySingleApplication::setWidget(QWidget *value)
{
    _widget = value;
}

/**************************************************
 * 通过socket通讯实现程序单实例运行，
 * 初始化本地连接，如果连接不上server，则创建，否则退出
 *************************************************/
void MySingleApplication::_initLocalConnection()
{
    _isRunning = false;

    QLocalSocket socket;
    socket.connectToServer(_serverName);
    if(socket.waitForConnected(TIME_OUT)) {
        fprintf(stderr, "%s already running.\n",
                _serverName.toLocal8Bit().constData());
        _isRunning = true;
        // 其他处理，如：将启动参数发送到服务端
        return;
    }

    //连接不上服务器，就创建一个
    _newLocalServer();
}

/*******************
 * 创建LocalServer
 ******************/
void MySingleApplication::_newLocalServer()
{
    _localServer = new QLocalServer(this);
    connect(_localServer, SIGNAL(newConnection()), this, SLOT(_newLocalConnection()));
    if(!_localServer->listen(_serverName)) {
        // 此时监听失败，可能是程序崩溃时,残留进程服务导致的,移除之
        if(_localServer->serverError() == QAbstractSocket::AddressInUseError) {
            QLocalServer::removeServer(_serverName); // <-- 重点
            _localServer->listen(_serverName); // 再次监听
        }
    }
}

/************
 * 激活主窗口
 * *********/
void MySingleApplication::_activateWindow()
{
    if(_widget) {
        _widget->raise();
        _widget->show();
        _widget->activateWindow(); // 激活窗口
    }
}
