#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>

class QSqlDatabase;

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    bool init(const QString& host,int port,const QString& database_name,const QString& user_name,const QString& password);
    bool write(QString Table_Name, QString Items, QString Values, QString &LastInsertId);
    bool write(QString Table_Name, QStringList Items, QStringList Values, QString &LastInsertId);
    bool write(QString Table_Name, QStringList Items, QStringList Values);
    bool write(QString Table_Name,QString Items,QString Values);
    void Database_Close();

private:
    QSqlDatabase *dbMYSQL;

    void log(QString c);

signals:

public slots:
};

#endif // DATABASE_H
