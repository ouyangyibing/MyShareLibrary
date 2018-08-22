#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QApplication>

/*****************************************
示例:
int main(int argc, char *argv[])
{
    MySingleApplication a(argc, argv);
    if(!a.isRunning()) {
        MainWindow w;        
        w.showMaximized();

        a.setWidget(&w);
        return a.exec();
    }
    return 0;
}
***************************************/

class QLocalServer;

class MySingleApplication : public QApplication
{
    Q_OBJECT
public:
    MySingleApplication(int &argc, char **argv);

    bool isRunning();     // 是否已經有实例在运行
    bool isNoRunning();   // 是否没有实例在运行

    void setWidget(QWidget *value);


private slots:
    // 有新连接时触发
    void _newLocalConnection();

private:
    // MainWindow指针
    QWidget *_widget;
    // 初始化本地连接
    void _initLocalConnection();
    // 创建服务端
    void _newLocalServer();
    // 激活窗口
    void _activateWindow();

    bool _isRunning;                // 是否已經有实例在运行
    QLocalServer *_localServer;     // 本地socket Server
    QString _serverName;            // 服务名称
};

#endif // SINGLEAPPLICATION_H
