#include "MyLabel.h"
#include <QDebug>

MyLabel::MyLabel(QWidget *parent) :
    QLabel(parent),
    label(this)
{
    timer = new QTimer(this);
    this->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    this->setFont(QFont("微软雅黑",20));//font: 75 18pt "微软雅黑";
    connect(timer, &QTimer::timeout, this, &MyLabel::slotBlinking);
}

QString MyLabel::getLabelText() const
{
    return label->text();
}

void MyLabel::pass(QString setLabelText)
{
    timer->stop();
    pass(label, setLabelText);
}

void MyLabel::pass(QLabel *_label, QString setLabelText)
{
    _label->setText(setLabelText);
    _label->setStyleSheet("background-color: rgb(85, 255, 0);");
}

void MyLabel::fail(QString setLabelText)
{
    timer->stop();
    fail(label, setLabelText);
}

void MyLabel::fail(QLabel *_label, QString setLabelText)
{
    _label->setText(setLabelText);
    _label->setStyleSheet("background-color: rgb(255, 0, 0);");
}

void MyLabel::ready(QString setLabelText)
{
    timer->stop();
    ready(label, setLabelText);
}

void MyLabel::ready(QLabel *_label, QString setLabelText)
{
    _label->setText(setLabelText);
    _label->setStyleSheet("background-color: rgb(255, 255, 0);");
}

void MyLabel::stop(QString setLabelText)
{
    timer->stop();
    stop(label, setLabelText);
}

void MyLabel::stop(QLabel *_label, QString setLabelText)
{
    _label->setText(setLabelText);
    _label->setStyleSheet("background-color: rgb(159, 159, 159);");
}

void MyLabel::error(QString setLabelText)
{
    timer->stop();
    error(label, setLabelText);
}

void MyLabel::error(QLabel *_label, QString setLabelText)
{
    _label->setText(setLabelText);
    _label->setStyleSheet("background-color: rgb(255, 0, 0);");
}

void MyLabel::red()
{
    red(label);
}

void MyLabel::blue()
{
    blue(label);
}

void MyLabel::gray()
{
    gray(label);
}

void MyLabel::green()
{
    green(label);
}

void MyLabel::red(QLabel *_label)
{
    _label->setStyleSheet("background-color: red");
}

void MyLabel::blue(QLabel *_label)
{
    _label->setStyleSheet("background-color: blue");
}

void MyLabel::gray(QLabel *_label)
{
    _label->setStyleSheet("background-color: gray");
}

void MyLabel::green(QLabel *_label)
{
    _label->setStyleSheet("background-color: rgb(85, 255, 0);");
}

void MyLabel::lamp(QLabel *_label, bool result)
{
    if(result) {
        blue(_label);
    }
    else {
        _label->setStyleSheet("");
    }
}

void MyLabel::testing(QString setLabelText)
{
    blinking(setLabelText, true);
}

void MyLabel::testing(QString setLabelText, bool blink)
{
    blinking(setLabelText, blink);
    label->setStyleSheet("background-color: rgb(85, 170, 255);");
}

void MyLabel::testing(QLabel *_label, QString setLabelText)
{
    _label->setText(setLabelText);
    _label->setStyleSheet("background-color: rgb(85, 170, 255);");
}

void MyLabel::initialize(QString setLabelText)
{
    blinking(setLabelText, true);
}

void MyLabel::initialize(QLabel *_label, QString setLabelText)
{
    _label->setText(setLabelText);
    _label->setStyleSheet("background-color: rgb(85, 170, 255);");
}

void MyLabel::blinking(QString setLabelText, bool blink)
{
    label->setText(setLabelText);
    blinking(blink);
}

void MyLabel::blinking(bool blink)
{
    if(blink) {
        timer->start(1000);
    }
    else {
        timer->stop();
    }
}

void MyLabel::slotBlinking()
{
    b_blink = !b_blink;
    if(b_blink) {
        label->setStyleSheet("background-color: rgb(255, 255, 0);");
    }
    else {
        label->setStyleSheet("background-color: rgb(255, 0, 0);");
    }
}
