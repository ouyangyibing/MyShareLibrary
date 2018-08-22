#ifndef CTIMEKEEPING_H
#define CTIMEKEEPING_H

#include <QLabel>
#include <QTime>

class QTimer;

class MyTimekeeping : public QLabel
{
    Q_OBJECT
public:
    explicit MyTimekeeping(QWidget *parent = nullptr);

    //开始计时，RefreshInterval_MS 为计时的刷新时间间隔,单位毫秒
    void start(int RefreshInterval_MS = 150);

    //停止计时，并返回从开始到结束的时间,单位秒
    QString stop_S();

    //获取从开始到现在（结束前）所用在时间,单位秒
    QString getUseTime_S() const;

    //获取开始计时后，上一次获取间隔时间与这一次获取间隔时间的时间之差,单位秒
    //不受sopt影响，会一直计时
    QString getSpacingInterval_S();

    //获取现在的时间
    static QString getCurentTime(const QString &format = "yyyy-MM-dd hh:mm:ss");

private:
    //刷新ui的定时器
    QTimer *timer = nullptr;

    //计时器
    QTime time;

    //上一次获取时间的时间戳，单位毫秒
    int lastTime = 0;
    //结束时的时间戳，单位毫秒
    int stopTime = 0;
};

#endif // CTIMEKEEPING_H
