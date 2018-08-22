/**********************************************
 * 此类只做提升类
 * 否则会出现不可控的bug
 * *******************************************/

#ifndef CUSTOM_LABEL_H
#define CUSTOM_LABEL_H

#include <QLabel>
#include <QTimer>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = nullptr);

    QString getLabelText() const;

    void pass(QString setLabelText = "PASS");
    static void pass(QLabel *_label, QString setLabelText = "Pass");

    void fail(QString setLabelText = "FAIL");
    static void fail(QLabel *_label, QString setLabelText = "Fail");

    void ready(QString setLabelText = "Read");
    static void ready(QLabel *_label, QString setLabelText = "Read");

    void stop(QString setLabelText = "Stop");
    static void stop(QLabel *_label, QString setLabelText = "Stop");

    void error(QString setLabelText = "Error");
    static void error(QLabel *_label, QString setLabelText = "Error");

    void red();
    void blue();
    void gray();
    void green();

    static void red(QLabel *_label);
    static void blue(QLabel *_label);
    static void gray(QLabel *_label);
    static void green(QLabel *_label);
    static void lamp(QLabel *_label, bool result);

    void testing(QString setLabelText = "Testing");
    void testing(QString setLabelText, bool blink);
    void testing(QLabel *_label, QString setLabelText = "Testing");

    void initialize(QString setLabelText = "Initialize...");
    void initialize(QLabel *_label, QString setLabelText = "Initialize...");

    void blinking(QString setLabelText, bool blink = true);
    void blinking(bool blink);

private slots:
    void slotBlinking();
private:
    QLabel *label = nullptr;
    QTimer *timer;
    bool b_blink = true;
};

#endif // CUSTOM_LABEL_H
