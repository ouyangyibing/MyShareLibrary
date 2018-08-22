#ifndef MYSETTINGCOMMON_H
#define MYSETTINGCOMMON_H

#include <QWidget>
#include "MySetting.h"
//#include "MySingleton.h"

namespace Ui {
class MySettingGeneral;
}

class MySettingCommon : public MySetting
{
    Q_OBJECT
public:
    static MySettingCommon* Instance();
    ~MySettingCommon();

    virtual void readToUi()override;
    virtual void saveToFile()override;
    virtual void uiDisabled(bool flag = true) override;
    virtual void syncValud()override;

protected:
    explicit MySettingCommon(QWidget *parent = 0);

public:
    QString getDB_Host() const;
    QString getDB_Port() const;
    QString getDB_Name() const;
    QString getDB_User() const;
    QString getDB_Password() const;

    QString getBarcodeValidator() const;
    QString getBasePath() const;

    QString getTAGS() const;
    QString getTEST_STATION_ID() const;

    void setBasePath(const QString &value);

private:
    static MySettingCommon* self;
    QString DB_Host = "192.168.8.61";
    QString DB_Port = "3306";
    QString DB_Name = "lewis_aero";
    QString DB_User = "barcode";
    QString DB_Password = "barcode";

    QString barcodeValidator = "^(LE|AE)\\w{11}$";
    QString basePath = "D:/Aero_AMT_Report/";

    QString TEST_STATION_ID;
    QString TAGS;

    Ui::MySettingGeneral *ui;
};

#endif // MYSETTINGCOMMON_H
