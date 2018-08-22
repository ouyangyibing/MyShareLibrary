#include "MyDatabase.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QThread>
#include <QEventLoop>
#include <QSqlQuery>

class MyDatabasePriveData
{
public:
    inline bool isRunning() const   { return  _running; }
    inline void readyRun()          { _running = true;  }
    inline void endRun()            { _running = false; }
    inline void resetRun()          { _running = false; }

    inline bool isRunOpen() const   { return    _runOpen;  }
    inline void readyRunOpen()      { _runOpen = true;     }
    inline void endRunOpen()        { _runOpen = false;    }
    inline void resetRunOpen()      { _runOpen = false;    }

    inline bool isRunAlter() const  { return     _runAlter;}
    inline void readyRunAlter()     { _runAlter = true;    }
    inline void endRunAlter()       { _runAlter = false;   }
    inline void resetRunAlter()     { _runAlter = false;   }

    inline bool isRunCheck() const  { return     _runCheck;}
    inline void readyRunCheck()     { _runCheck = true;    }
    inline void endRunCheck()       { _runCheck = false;   }
    inline void resetRunCheck()     { _runCheck = false;   }

    inline bool result() const          { return    _result;}
    inline void resetResult()           { _result   = false;}
    inline void setResult(bool result)  { _result = result; }    

private:
    volatile bool _running  = false;
    volatile bool _runOpen  = false;
    volatile bool _runAlter = false;
    volatile bool _runCheck = false;
    volatile bool _result   = false;
};

MyDatabasePrivate::MyDatabasePrivate(QObject *parent) :
    QObject(parent),
    db(new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"))),
    d(new MyDatabasePriveData)
{
    resetAllData();

    connect(this, &MyDatabasePrivate::sigOpen,
            this, &MyDatabasePrivate::slotOpen);

    connect(this, &MyDatabasePrivate::sigAlter,
            this, &MyDatabasePrivate::slotAlter);

    connect(this, &MyDatabasePrivate::sigCheckItemResult,
            this, &MyDatabasePrivate::slotCheckItemResult);
}

void MyDatabasePrivate::init(const QString &host, int port, const QString &databaseName, const QString &user_name, const QString &password)
{
    db->setHostName(host);
    db->setPort(port);
    db->setDatabaseName(databaseName);
    db->setUserName(user_name);
    db->setPassword(password);
}

bool MyDatabasePrivate::result()
{
    return d->result();
}

bool MyDatabasePrivate::isRunning()
{
    return d->isRunning();
}

bool MyDatabasePrivate::isIdle()
{
    return !d->isRunning();
}

bool MyDatabasePrivate::isFinish()
{
    return !d->isRunning();
}

bool MyDatabasePrivate::isRunOpen()
{
    return d->isRunOpen();
}

bool MyDatabasePrivate::isOpenFinish()
{
    return !d->isRunOpen();
}

bool MyDatabasePrivate::isRunAlter()
{
    return d->isRunAlter();
}

bool MyDatabasePrivate::isAlterFinish()
{
    return !d->isRunAlter();
}

bool MyDatabasePrivate::isRunCheck()
{
    return d->isRunCheck();
}

bool MyDatabasePrivate::isCheckFinish()
{
    return !d->isRunCheck();
}

void MyDatabasePrivate::slotOpen()
{
    d->setResult( db->open());
    if (!d->result()) {
        qWarning()<< "DB connect fail"
                  << "\r\nDB last error:\r\n    " << db->lastError()
                  << endl;
    }
    openFinish();
}

void MyDatabasePrivate::openFinish()
{
    d->endRun();
    d->endRunOpen();
    emit sigOpenFinish();
    emit sigFinish();
}

void MyDatabasePrivate::slotAlter(const QString &alterCommand, QString *lastInsertId)
{
    d->setResult( exec_alter(alterCommand, lastInsertId));
    alterFinish();
}

void MyDatabasePrivate::slotCheckItemResult(const QString &checkCommand, const QString &checkValue)
{
    d->setResult( checkItemResult(checkCommand, checkValue));
    checkItemResultFinish();
}

bool MyDatabasePrivate::checkItemResult(const QString &checkCommand, const QString &checkValue)
{
    QSqlQuery query;
    if (!query.exec(checkCommand)) {
        qWarning()<< "DB query exec fail"
                  << "\r\nQuery last error:\r\n     " << query.lastError()
                  << "\r\nDB last error:\r\n     " << db->lastError()
                  << "\r\nThe instructions executed this time are:\r\n     " << checkCommand
                  << endl;
        return false;
    }

    if (!query.next()) {
        qWarning()<< "The record was not found!"
                  << "\r\nThe instructions executed this time are:\r\n     " << checkCommand
                  << endl;
        return false;
    }

    if (checkValue == query.value(0).toString()) {
        return true;
    } else {
        qWarning()<< "Check results do not match!"
                  << "\r\ncheck results:" << query.value(0).toString()
                  << "\r\nDesired results:" << checkValue
                  << "\r\nThe instructions executed this time are:\r\n     " << checkCommand
                  << endl;
        return false;
    }
}

bool MyDatabasePrivate::exec_alter(const QString &alter_command, QString * const lastInsertId)
{
    QSqlQuery query;
    //先初始化，不然函数出错时，可能会获取到上一次的值    
    if (lastInsertId) {//检查指针是否为空，这句不能删
        (*lastInsertId) = "";
    }
    //如果事务开启失败则打印日志并退出
    if (!db->transaction()) {
        qWarning()<< "DB transaction open fail"
                  << "\r\nDB open" << db->isOpen()
                  << "\r\nQuery last error:\r\n     " << query.lastError()
                  << "\r\nDB last error:\r\n     " << db->lastError()
                  << "\r\nThe instructions executed this time are:\r\n     " << alter_command
                  << endl;
        db->rollback();
        return false;
    }

    if (!query.exec(alter_command)) {
        qWarning()<< "DB query exec fail"
                  << "\r\nQuery last error:\r\n     " << query.lastError()
                  << "\r\nDB last error:\r\n     " << db->lastError()
                  << "\r\nThe instructions executed this time are:\r\n     " << alter_command
                  << endl;
        db->rollback();
        return false;
    }

    if (lastInsertId) {
        (*lastInsertId) = query.lastInsertId().toString();
    }

    if (!db->commit()) {
        qWarning()<< "DB data commit fail"
                  << "\r\nQuery last error:\r\n     " << query.lastError()
                  << "\r\nDB last error:\r\n     " << db->lastError()
                  << "\r\nThe instructions executed this time are:\r\n     " << alter_command
                  << endl;

        return false;
    }

    return true;
}

void MyDatabasePrivate::alterFinish()
{
    d->endRun();
    d->endRunAlter();
    emit sigAlterFinish();
    emit sigFinish();
}

void MyDatabasePrivate::checkItemResultFinish()
{
    d->endRun();
    d->endRunCheck();
    emit sigCheckFinish();
    emit sigFinish();
}

MyDatabasePrivate::~MyDatabasePrivate()
{    
    db->close();
    delete db;
    delete d;
}

void MyDatabasePrivate::readyOpen()
{
    d->resetResult();
    d->readyRun();
    d->readyRunOpen();
}

void MyDatabasePrivate::readyAlter()
{
    d->resetResult();
    d->readyRun();
    d->readyRunAlter();
}

void MyDatabasePrivate::readyCheckItemResult()
{
    d->resetResult();
    d->readyRun();
    d->readyRunCheck();
}

void MyDatabasePrivate::resetAllData()
{
    d->resetRun();
    d->resetRunOpen();
    d->resetRunAlter();
    d->resetRunCheck();
    d->resetResult();
}

MyDatabase::MyDatabase(QObject *parent) : QObject(parent)
{
    db = new MyDatabasePrivate();
    thread = new QThread();
    db->moveToThread(thread);
    thread->start();

    connect(db, &MyDatabasePrivate::sigOpenFinish,
            this, &MyDatabase::sigOpenFinish);

    connect(db, &MyDatabasePrivate::sigAlterFinish,
            this, &MyDatabase::sigAlterFinish);
}

MyDatabase::~MyDatabase()
{
    db->close();
    delete db;

    thread->quit();
    thread->wait(100);
    thread->deleteLater();
}

void MyDatabase::init(const QString &host, int port, const QString &databaseName, const QString &user_name, const QString &password)
{
    db->init(host, port, databaseName, user_name, password);
}

bool MyDatabase::result() const
{
    return db->result();
}

void MyDatabase::open()
{
    if (db->isIdle()) {
        db->readyOpen();
        db->sigOpen();
    }
}

void MyDatabase::waitOpenFinish()
{
    QEventLoop LoopEvent;
    connect(db, &MyDatabasePrivate::sigOpenFinish, &LoopEvent, &QEventLoop::quit);

    if (db->isRunOpen()) {
        LoopEvent.exec();
    }
}

void MyDatabase::write(const QString &tableName, const QString &items, const QString &values, QString * const lastInsertId)
{
    QString alter_command = QString("INSERT INTO %1 (%2)VALUES(%3)").arg(tableName,items,values);
    alter(alter_command, lastInsertId);
}

void MyDatabase::write(const QString &tableName, const QStringList &items, const QStringList &values, QString * const lastInsertId)
{
    QString strItems;
    QString strValues = "'";
    QStringList::const_iterator value = values.begin();
    QStringList::const_iterator item = items.begin();
    for(; item != items.end(); ++value, ++item) {
        if(!(static_cast<QString>(*value).isEmpty())) {
            strItems += *item + ",";
            strValues += *value + "','";
        }
    }
    strItems.truncate(strItems.length()-1);
    strValues.truncate(strValues.length()-2);

    write(tableName, strItems, strValues, lastInsertId);
}

void MyDatabase::alter(const QString &alterCommand, QString * const lastInsertId)
{
    if (db->isRunning()) {
        waitForFinish();
    }

    if (db->isIdle()) {
        db->readyAlter();
        db->sigAlter(alterCommand, lastInsertId);
    } else {
        qWarning()<< "The program has an exception and the database thread is performing other operations, which are ignored!"
                  << "\r\nThe unprocessed data is as follows:\r\n    "
                  << alterCommand
                  << endl;
    }
}

void MyDatabase::update(const QString &tableName, const QStringList &items, const QStringList &values,
                        const QStringList &whereItems, const QStringList &whereValues)
{
    QString setData = "";
    QStringList::const_iterator item = items.begin();
    QStringList::const_iterator value = values.begin();
    for ( ;
          (item != items.end()) && (value != values.end());
          ++item, ++value) {
        if (static_cast<QString>(*value).isEmpty()
                || static_cast<QString>(*item).isEmpty()) {
            continue ;
        }
        setData += QString("%1 = '%2'").arg(*item, *value);

        if ((item < items.end()-1) && (value < values.end()-1)) {
            setData += ",";
        }
    }

    QString whereData = "";
    QStringList::const_iterator whereItem = whereItems.begin();
    QStringList::const_iterator whereValue = whereValues.begin();
    for ( ;
          (whereItem != whereItems.end()) && (whereValue != whereValues.end());
          ++whereItem, ++whereValue) {
        if (static_cast<QString>(*whereValue).isEmpty()
                || static_cast<QString>(*whereItem).isEmpty()) {
            continue ;
        }
        whereData += QString("%1 = '%2'").arg(*whereItem, *whereValue);

        if ((whereItem < whereItems.end()-1) && (whereValue < whereValues.end()-1)) {
            whereData += " and ";
        }
    }

    alter(QString("UPDATE %1 SET %2 Where %3").arg(tableName, setData, whereData));
}

void MyDatabase::update(const QString &tableName, const QStringList &items, const QStringList &values,
                        const QString &whereItem, const QString &whereValue)
{
    QString setData = "";
    QStringList::const_iterator item = items.begin();
    QStringList::const_iterator value = values.begin();
    for ( ;
          (item != items.end()) && (value != values.end());
          ++item, ++value) {
        if (static_cast<QString>(*value).isEmpty()
                || static_cast<QString>(*item).isEmpty()) {
            continue ;
        }
        setData += QString("%1 = '%2'").arg(*item, *value);

        if ((item < items.end()-1) && (value < values.end()-1)) {
            setData += ",";
        }
    }
    alter(QString("UPDATE %1 SET %2 Where %3 = '%4'").arg(tableName, setData, whereItem, whereValue));
}

void MyDatabase::waitAlterFinish()
{
    QEventLoop LoopEvent;
    connect(db, &MyDatabasePrivate::sigAlterFinish, &LoopEvent, &QEventLoop::quit);

    if (db->isRunAlter()) {
        LoopEvent.exec();
    }
}

void MyDatabase::waitCheckItemResultFinish()
{
    QEventLoop LoopEvent;
    connect(db, &MyDatabasePrivate::sigCheckFinish, &LoopEvent, &QEventLoop::quit);

    if (db->isRunCheck()) {
        LoopEvent.exec();
    }
}

void MyDatabase::waitForFinish()
{
    QEventLoop LoopEvent;
    connect(db, &MyDatabasePrivate::sigFinish, &LoopEvent, &QEventLoop::quit);

    if (db->isRunning()) {
        LoopEvent.exec();
    }
}

/*******************************************************
 * 假设
 * a 表格记录此次测试的条码
 * b 表格记录此此次测试的结果
 * snTableName: a 表格名称
 * resultTableName: b 表格名称
 * snTable_resultTabIdField: 在a表格中记录的b表格id号字段
 * resultTableIdField: 在b 表格中记录的b 表格id号字段
 * sortField: 拥有时间先后顺序的字段，如“LAST_UPDATED”
 * snField: 条码所在字段
 * sn: 需要查询的 SN
 * resultField: 结果字段
 * resultData: 结果字段需要匹配的数据
 * ****************************************************/
void MyDatabase::isItemExist(
        const QString &snTableName,
        const QString &resultTableName,
        const QString &snTable_resultTabIdField,
        const QString &resultTableIdField,
        const QString &sortField,
        const QString &snField,
        const QString &SN,
        const QString &resultField,
        const QString &resultData)
{
    QString checkCommand = QString("SELECT b.%1 "
                                   "FROM %2 a, %3 b "
                                   "WHERE a.%4 = b.%5 "
                                   "AND a.%6 = '%7' "
                                   "ORDER BY a.%9 DESC limit 1 ")
                           .arg(resultField,
                                snTableName, resultTableName,
                                snTable_resultTabIdField, resultTableIdField,
                                snField, SN,
                                sortField );

    if (db->isRunning()) {
        waitForFinish();
    }

    if (db->isIdle()) {
        db->readyCheckItemResult();
        db->sigCheckItemResult(checkCommand, resultData);
    } else {
        qWarning()<< "The program has an exception and the database thread is performing other operations, which are ignored!"
                  << "\r\nThe unprocessed data is as follows:\r\n    "
                  << checkCommand
                  << endl;
    }
}

void MyDatabase::deleteData(const QString &tableName, const QString &fidle, const QString &value)
{
    if (db->isRunning()) {
        waitForFinish();
    }

    if (db->isIdle()) {
        db->readyAlter();
        db->sigAlter(QString("DELETE FROM %1 WHERE %2 = '%3'").arg(tableName, fidle, value));
    }
}
