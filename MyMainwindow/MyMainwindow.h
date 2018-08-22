#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include "ui_MyMainwindow.h"

class MySettingCommon;
class MySettingDialog;

namespace Ui {
class MainWindow;
}

class MyMainwindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MyMainwindow(QWidget *parent = 0);
    ~MyMainwindow();

signals:
    void sigSettingFinish();
    void sigPass();
    void sigFail();

private slots:
    void on_action_setting_triggered();
    void on_action_on_off_triggered(bool checked);

    void slotTextBrowser_2_Data(QString c);
    void slotTextBrowser_1_Data(QString c);

    void slotPass();
    void slotFail();

protected:
    float passNum = 0;
    float totalNum = 0;
    float passRate = 0;

    Ui::MainWindow *ui;
    QRegularExpression lineFeed;
    MySettingCommon *genSet;
    MySettingDialog *diaSet;

    void initConfig();

    void writeGeometry();    
    void regainGeometry();

    void passRateCountAndUpdataUi();
};

#endif // MYMAINWINDOW_H
