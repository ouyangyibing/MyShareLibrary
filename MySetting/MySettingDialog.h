#ifndef MYSETTINGDIALOG_H
#define MYSETTINGDIALOG_H

#include <QDialog>
#include <QList>

class MySetting;

namespace Ui {
class MySettingDialog;
}

class MySettingDialog : public QDialog
{
    Q_OBJECT
public:
    ~MySettingDialog();
    int insertTab(int index, MySetting *mySetting);
    int exec();

    static MySettingDialog* Instance();

protected:    
    explicit MySettingDialog(QWidget *parent = 0);

private slots:
    void on_save_clicked();
    void on_inputPassword_clicked();

private:
    static MySettingDialog* self;
    QList<MySetting*> mySettings;
    Ui::MySettingDialog *ui;

    void show();
    void showExtension();
    void showFullScreen();
    void showMaximized();
    void showMinimized();
    void showNormal();   
};

#endif // MYSETTINGDIALOG_H
