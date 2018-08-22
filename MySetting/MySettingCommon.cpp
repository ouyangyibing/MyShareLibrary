#include "MySettingCommon.h"
#include "ui_MySettingGeneral.h"
#include <QDebug>
#include <QSettings>
#include <QRegExpValidator>
#include <QMutex>

MySettingCommon *MySettingCommon::self = nullptr;
MySettingCommon *MySettingCommon::Instance()
{
    if(!self) {
        QMutex muter;
        QMutexLocker clocker(&muter);

        if(!self) {
            self = new MySettingCommon();
        }
    }

    return self;
}

MySettingCommon::MySettingCommon(QWidget *parent) :
    MySetting(parent),
    ui(new Ui::MySettingGeneral)
{
    ui->setupUi(this);
    this->setWindowTitle("Common");
    ui->DATABASE->setDisabled(true);
    ui->ADVANCED->setDisabled(true);

    QRegExp db_host_rx("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");
    ui->DB_Host->setValidator(new QRegExpValidator(db_host_rx, this));

    QRegExp db_port_rx("\\d+");
    ui->DB_Port->setValidator(new QRegExpValidator(db_port_rx, this));

    ui->DB_Password->setEchoMode(QLineEdit::Password);

    ui->basePath->setDisabled(true);
}

MySettingCommon::~MySettingCommon()
{
    delete ui;
}

void MySettingCommon::syncValud()
{
    DB_Host = ui->DB_Host->text();
    DB_Port = ui->DB_Port->text();
    DB_User = ui->DB_User->text();
    DB_Name = ui->DB_Name->text();
    DB_Password = ui->DB_Password->text();

    barcodeValidator    = ui->barcodeValidator->text();
    //报表路径不允许在设置中配置
    //basePath  = ui->basePath->text();

    TEST_STATION_ID = ui->TEST_STATION_ID->text();
    TAGS = ui->TAGS->text();
}

void MySettingCommon::readToUi()
{
    //如果需要默认值，就在读取设置之前，先将值写入ui
    ui->barcodeValidator->setText(barcodeValidator);
    ui->basePath->setText(basePath);

    ui->DB_Host->setText(DB_Host);
    ui->DB_Port->setText(DB_Port);
    ui->DB_User->setText(DB_User);
    ui->DB_Name->setText(DB_Name);
    ui->DB_Password->setText(DB_Password);

    QSettings setting(fileName, QSettings::IniFormat);
    read(setting, ui->ADVANCED);
    read(setting, ui->DATABASE);
    read(setting, ui->OTHER);
}

void MySettingCommon::saveToFile()
{
    QSettings setting(fileName, QSettings::IniFormat);
    write(setting, ui->ADVANCED);
    write(setting, ui->DATABASE);
    write(setting, ui->OTHER);
}

void MySettingCommon::uiDisabled(bool flag)
{
    ui->DATABASE->setDisabled(flag);
    ui->ADVANCED->setDisabled(flag);
}

QString MySettingCommon::getDB_Password() const
{ return DB_Password; }

QString MySettingCommon::getDB_User() const
{ return DB_User; }

QString MySettingCommon::getDB_Name() const
{ return DB_Name; }

QString MySettingCommon::getDB_Port() const
{ return DB_Port; }

QString MySettingCommon::getDB_Host() const
{ return DB_Host; }

QString MySettingCommon::getBasePath()   const
{ return basePath;}

QString MySettingCommon::getBarcodeValidator() const
{ return barcodeValidator;}

QString MySettingCommon::getTEST_STATION_ID() const
{
    return TEST_STATION_ID;
}

void MySettingCommon::setBasePath(const QString &value)
{
    basePath = value;
}

QString MySettingCommon::getTAGS() const
{
    return TAGS;
}
