#include "MyMainwindow.h"
#include <QDebug>
#include <QSettings>
#include <QDesktopWidget>
#include "MySettingCommon.h"
#include "MySettingDialog.h"
#include <QFile>

MyMainwindow::MyMainwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);        

    diaSet  = MySettingDialog::Instance();
    genSet = MySettingCommon::Instance();
    diaSet->insertTab(0, genSet);
    initConfig();

    lineFeed.setPattern("[\r\n]$");
    regainGeometry();

    connect(this, &MyMainwindow::sigPass, this, &MyMainwindow::slotPass);
    connect(this, &MyMainwindow::sigFail, this, &MyMainwindow::slotFail);
}

MyMainwindow::~MyMainwindow()
{
    writeGeometry();
    delete ui;
}

void MyMainwindow::writeGeometry()
{
    if (!QFile::exists("./Geometry")) {
        QSettings settings("./Geometry", QSettings::IniFormat);
        settings.beginGroup("mainWindow");
        settings.setValue("geometry", saveGeometry());
        settings.setValue("state", saveState());
        settings.endGroup();
    }
}

void MyMainwindow::regainGeometry()
{    
    QString setFileName = "./Geometry";
    if (!QFile::exists(setFileName)) {
        setFileName = ":/Sources/Geometry";
    }

    QSettings settings(setFileName, QSettings::IniFormat);
    settings.beginGroup("mainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
    settings.endGroup();
}

void MyMainwindow::passRateCountAndUpdataUi()
{
    passRate = passNum / totalNum;

    ui->label_pass->setText(QString::number(passNum));
    ui->label_total->setText(QString::number(totalNum));
    ui->label_passRate->setText(QString::number(passRate, 'f', 2));
}

void MyMainwindow::initConfig()
{
    ui->label_tags->setText(genSet->getTAGS());
    ui->label_testStation_id->setText(genSet->getTEST_STATION_ID());
    ui->label_reportPath->setText(genSet->getBasePath());
}

void MyMainwindow::slotTextBrowser_1_Data(QString c)
{
    c.remove(lineFeed);
    ui->textBrowser_text_1->append(c);
}

void MyMainwindow::slotPass()
{
    ui->label_result->pass();
    passNum ++;
    totalNum++;
    passRateCountAndUpdataUi();
}

void MyMainwindow::slotFail()
{
    ui->label_result->fail();
    totalNum++;
    passRateCountAndUpdataUi();
}

void MyMainwindow::slotTextBrowser_2_Data(QString c)
{
    c.remove(lineFeed);
    ui->textBrowser_text_2->append(c);
}

void MyMainwindow::on_action_on_off_triggered(bool checked)
{
    ui->action_on_off->setText( checked ? "ON":"OFF");

    ui->action_setting->setDisabled(checked);
    ui->action_tools->setDisabled(checked);

    if (checked) {
        ui->label_result->ready();
    } else {
        ui->label_result->stop();
    }
}

void MyMainwindow::on_action_setting_triggered()
{
    if (diaSet->exec() != QDialog::Accepted) {
        return ;
    }
    initConfig();

    emit sigSettingFinish();
}
