#include "database.h"
#include "qDebug"
#include <QtSql>
#include <QDateTime>
#include <QCoreApplication>

Database::Database(QObject *parent) : QObject(parent)
{
    dbMYSQL = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));
}

Database::~Database()
{
    delete dbMYSQL;
}

bool Database::init(const QString& host,int port,const QString& database_name,const QString& user_name,const QString& password)
{
    dbMYSQL->setHostName(host);

    dbMYSQL->setPort(port);

    dbMYSQL->setDatabaseName(database_name);

    dbMYSQL->setUserName(user_name);

    dbMYSQL->setPassword(password);

    if (!dbMYSQL->open())
        return true;//failure
    else
        return false;//success
}

bool Database::write(QString Table_Name,QString Items,QString Values)
{
    bool Status;
    QSqlQuery query_insert;
    QString Insert_string = ((QString)"INSERT INTO %1 (%2)VALUES(%3)").arg(Table_Name,Items,Values);
    //    qDebug()<<"Insert_string = "<< Insert_string<<endl;

    dbMYSQL->transaction();
    Status = query_insert.exec(Insert_string);
    log(Insert_string+"\n\nError:"+query_insert.lastError().text());
    //    qDebug() << query_insert.lastError() << endl;
    if (Status) {
        dbMYSQL->commit();
        return false;//success
    }
    else {
        dbMYSQL->rollback();
        return true;//failure
    }
}

bool Database::write(QString Table_Name,QString Items,QString Values,QString &LastInsertId)
{
    bool Status;
    QSqlQuery query_insert;
    QString Insert_string = ((QString)"INSERT INTO %1 (%2)VALUES(%3)").arg(Table_Name,Items,Values);
    //    qDebug()<<"Insert_string = "<< Insert_string<<endl;

    dbMYSQL->transaction();
    Status = query_insert.exec(Insert_string);
    log(Insert_string+"\n\nError:"+query_insert.lastError().text());
    //    qDebug() << query_insert.lastError() << endl;
    LastInsertId = query_insert.lastInsertId().toString();
    if (Status) {
        dbMYSQL->commit();
        return false;//success
    }
    else {
        dbMYSQL->rollback();
        return true;//failure
    }
}

bool Database::write(QString Table_Name, QStringList Items, QStringList Values, QString &LastInsertId)
{
    bool Status;
    QSqlQuery query_insert;

    QString strItem;
    QString strValue = "'";

    QStringList::iterator value = Values.begin();
    QStringList::iterator item = Items.begin();
    for(; item != Items.end(); ++value, ++item) {
        if(!(QString(*value).isEmpty())) {
            strItem += *item + ",";
            strValue += *value + "','";
        }
    }
    strItem.truncate(strItem.length()-1);
    strValue.truncate(strValue.length()-2);

    QString Insert_string = ((QString)"INSERT INTO %1 (%2)VALUES(%3)").arg(Table_Name,strItem,strValue);
    //qDebug()<<"Insert_string = "<< Insert_string<<endl;

    dbMYSQL->transaction();
    Status = query_insert.exec(Insert_string);
    log(Insert_string+"\n\nError:"+query_insert.lastError().text());
    //qDebug() << query_insert.lastError() << endl;
    LastInsertId = query_insert.lastInsertId().toString();
    if (Status) {
        dbMYSQL->commit();
        return false;//success
    }
    else {
        dbMYSQL->rollback();
        return true;//failure
    }
}

bool Database::write(QString Table_Name, QStringList Items, QStringList Values)
{
    bool Status;
    QSqlQuery query_insert;

    QString strItem;
    QString strValue = "'";

    QStringList::iterator value = Values.begin();
    QStringList::iterator item = Items.begin();
    for(; item != Items.end(); ++value, ++item) {
        if(!(QString(*value).isEmpty())) {
            strItem += *item + ",";
            strValue += *value + "','";
        }
    }
    strItem.truncate(strItem.length()-1);
    strValue.truncate(strValue.length()-2);

    QString Insert_string = ((QString)"INSERT INTO %1 (%2)VALUES(%3)").arg(Table_Name,strItem,strValue);

    //qDebug()<<"Insert_string = "<< Insert_string<<endl;

    dbMYSQL->transaction();
    Status = query_insert.exec(Insert_string);
    log(Insert_string+"\n\nError:"+query_insert.lastError().text());
    //qDebug() << query_insert.lastError() << endl;

    if (Status) {
        dbMYSQL->commit();
        return false;//success
    }
    else {
        dbMYSQL->rollback();
        return true;//failure
    }
}

void Database::Database_Close()
{
    dbMYSQL->close();
}

void Database::log(QString c)
{
    QDir dir;
    dir.mkpath(QCoreApplication::applicationDirPath()+"/log/DatabaseLog/");

    QFile file(QCoreApplication::applicationDirPath()+"/log/DatabaseLog/"+QDateTime::currentDateTime().toString("yyyy-MM-dd")+".txt");
    if (file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append)) {  //以追加的模式打开文件
        file.write("\nDatabase start-----------------------------------------------------------------------------\n\n"
                   + c.toLocal8Bit()+
                   "\nDatabase end-------------------------------------------------------------------------------\n\n"
                   );
    }
    file.close();
}
