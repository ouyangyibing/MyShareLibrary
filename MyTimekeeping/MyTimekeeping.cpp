#include "MyTimekeeping.h"
#include <QTimer>
#include <QDebug>

#define UNIT_MS_S 1000.0 //毫秒至秒的进制

MyTimekeeping::MyTimekeeping(QWidget *parent) :
    QLabel(parent)
{    
    timer = new QTimer(this);    
    connect(timer, &QTimer::timeout, [this](){setText(getUseTime_S());});

    setText("0");
}

/*****************************
 * 初始化所有时间戳
 * 开始定时器刷新ui
 * **************************/
void MyTimekeeping::start(int RefreshInterval_MS)
{
    lastTime = 0;
    stopTime = 0;
    setText("0");
    time.start();

    if(timer->isActive()) {
        qWarning("定时器正在运行");
    } else {
        timer->start(RefreshInterval_MS);
    }
}

QString MyTimekeeping::stop_S()
{
    timer->stop();
    stopTime = time.elapsed();    
    setText(getUseTime_S());
    return getUseTime_S();
}

QString MyTimekeeping::getUseTime_S() const
{
    if(timer->isActive()) {
        return QString::number(time.elapsed()/UNIT_MS_S, 'f', 2);
    } else {
        return QString::number(stopTime/UNIT_MS_S, 'f', 2);
    }
}

QString MyTimekeeping::getSpacingInterval_S()
{
    //int nTime;
    //if(timer->isActive()) {
    //    int current_Time = time.elapsed();
    //    nTime = current_Time - lastTime;
    //    lastTime = current_Time;
    //} else {
    //    nTime = stopTime - lastTime;
    //    lastTime = stopTime;
    //}
    //return QString::number(nTime/UNIT_MS_S, 'f', 2);

    int current_Time = time.elapsed();
    int nTime = current_Time - lastTime;
    lastTime = current_Time;
    return QString::number(nTime/UNIT_MS_S, 'f', 2);
}

QString MyTimekeeping::getCurentTime(const QString &format)
{
    return QDateTime::currentDateTime().toString(format);
}
