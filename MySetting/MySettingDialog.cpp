#include "MySettingDialog.h"
#include "ui_MySettingDialog.h"
#include "MySetting.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QMutex>

MySettingDialog *MySettingDialog::self = nullptr;
MySettingDialog *MySettingDialog::Instance()
{
    if(!self) {
        static QMutex muter;
        QMutexLocker clocker(&muter);

        if(!self) {
            self = new MySettingDialog();
        }
    }

    return self;
}

MySettingDialog::MySettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MySettingDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    this->setWindowTitle("Setting");

    qDeleteAll(mySettings);
    mySettings.clear();
}

MySettingDialog::~MySettingDialog()
{
    delete ui;
}

int MySettingDialog::insertTab(int index, MySetting *mySetting)
{
    if (!mySettings.contains(mySetting)) {
        mySettings.append(mySetting);
        mySetting->readToUi();
        mySetting->syncValud();
    }    
    return ui->tabWidget->insertTab(index, mySetting, mySetting->windowTitle());
}

void MySettingDialog::on_save_clicked()
{
    if (!mySettings.isEmpty()) {
        for(MySetting* mySetting : mySettings) {
            mySetting->saveToFile();
            mySetting->syncValud();
        }
    }
    accept();
}

int MySettingDialog::exec()
{
    ui->tabWidget->setCurrentIndex(0);

    if (!mySettings.isEmpty()) {
        for(MySetting* mySetting : mySettings) {
            mySetting->readToUi();
            mySetting->uiDisabled(true);
        }
    }
    return QDialog::exec();
}

void MySettingDialog::on_inputPassword_clicked()
{
    bool ok;
    const QString passwd =  QInputDialog::getText(this,
                                                  "Input",
                                                  "Passwd",
                                                  QLineEdit::Password,
                                                  "",
                                                  &ok,
                                                  Qt::Dialog | Qt::WindowCloseButtonHint
                                                  );
    if(!ok) return ;

    if(QString("kinyat").toLower() == passwd.toLower()) {
        for(MySetting* mySetting : mySettings) {
            mySetting->uiDisabled(false);
        }
    } else {
        QMessageBox::warning(this, "warning", "wrong password!");
    }
}
