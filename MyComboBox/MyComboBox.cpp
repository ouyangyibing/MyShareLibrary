#include "MyComboBox.h"
#include <QDebug>
#include <QMouseEvent>
#include <QSerialPort>
#include <QSerialPortInfo>

MyComboBox::MyComboBox(QWidget *parent):
    QComboBox(parent)
{
    this->setEditable(true);
}

void MyComboBox::mousePressEvent(QMouseEvent *e)
{
    QString tempText = this->currentText();

    this->clear();
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())    {
        this->addItem("无串口","null");
    } else {
		std::sort(infos.begin(), infos.end(),
              [](const QSerialPortInfo &info1, const QSerialPortInfo &info2){return info1.portName() < info2.portName();});
	}    

    foreach (QSerialPortInfo info, infos) {
        this->addItem(info.portName(), info.portName());        
    }

    this->setCurrentText(tempText);
    this->setCurrentIndex( this->findText(tempText));

    QComboBox::mousePressEvent(e);
}

MyComboBox::~MyComboBox()
{
}
