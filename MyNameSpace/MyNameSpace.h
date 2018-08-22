#ifndef M_SPACE_H
#define M_SPACE_H

#include <QEventLoop>
#include <QRegularExpression>
#include <QDebug>

class QDateTime;
class QTimer;

namespace  MY
{
const QString PRESS_START_BUTTON = "Press start button";
const QString INPUT_SN = "Input SN";
const QString READY = "Ready";
const QString TESTING = "Testing...";
const QString STOP = "Stop";
const QString PASS = "0";
const QString FAIL = "1";

const QString NETWORK_NULL = "NETWORK_NULL";
const QString NETWORK_PASS = "NETWORK_PASS";
const QString NETWORK_FAIL = "NETWORK_FAIL";

void  DelayMs(int time =100);
void  DelayMs(QEventLoop &LoopEvent,int time =1000);

QString  getCurentTime(const QString &format = "yyyy-MM-dd hh:mm:ss");
QString  getAppPath();

QString  mid(QString strData, QString strFront, QString strBack);
QRegularExpressionMatch  mid(QString strData, QString regularExpression);

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
}

//常用示例
//QMessageBox::warning(this,"警告","指针 gyro 未初始化");
//QTextCodec::codecForName("System")->toUnicode("乱码的字节数组");

/*****************************************
 * EXPLOIT_DEBUG 开发时，
 * 需要debug的时候打开该宏 EXPLOIT_DEBUG
 ****************************************/
#define EXPLOIT_DEBUG
#ifdef  EXPLOIT_DEBUG
#ifndef MDEBUG
#define MDEBUG(MMESSAGE) qDebug()<<"Custom warning:"\
    <<MMESSAGE\
    <<"\nWarning position \n__FILE__:\""<<__FILE__\
    <<"\n__LINE__:"<<__LINE__\
    <<endl
#endif //MDEBUG
#else
#define MDEBUG(MMESSAGE)
#endif //EXPLOIT_DEBUG

#endif // M_SPACE_H
