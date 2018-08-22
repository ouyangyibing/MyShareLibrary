#ifndef PORT_COMBOBOX_H
#define PORT_COMBOBOX_H

#include <QComboBox>

class QMouseEvent;
//�ڹ��������� QT += serialport

class MyComboBox : public QComboBox
{
public:
    MyComboBox(QWidget *parent = 0);
    ~MyComboBox();

    void mousePressEvent(QMouseEvent *e);
};

#endif // PORT_COMBOBOX_H
