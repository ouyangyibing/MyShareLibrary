#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>
#include <QSqlDatabase>

class QThread;
class MyDatabasePriveData;

class MyDatabasePrivate : public QObject
{
    Q_OBJECT
    explicit MyDatabasePrivate(QObject *parent = nullptr);
    friend class MyDatabase;
public:
    ~MyDatabasePrivate();

    void init(const QString &host, int port, const QString &databaseName, const QString &user_name, const QString &password);
    inline void setHostName(const QString& host)     { db->setHostName(host);      }
    inline void setPort(int port)                    { db->setPort(port);          }
    inline void setDatabaseName(const QString& name) { db->setDatabaseName(name);  }
    inline void setUserName(const QString& name)     { db->setUserName(name);      }
    inline void setPassword(const QString& password) { db->setPassword(password);  }
    inline QString databaseName() const              { return db->databaseName();  }
    inline QString userName() const                  { return db->userName();      }
    inline QString password() const                  { return db->password();      }
    inline QString hostName() const                  { return db->hostName();      }
    inline QString driverName() const                { return db->driverName();    }
    inline int port() const                          { return db->port();          }
    inline void close()                              { db->close();                }
    inline QSqlDatabase *getDB() const               { return db;                  }
    inline MyDatabasePriveData *getData() const      { return d;                   }

    inline bool result();
    inline bool isRunning();
    inline bool isIdle();
    inline bool isFinish();
    inline bool isRunOpen();
    inline bool isOpenFinish();
    inline bool isRunAlter();
    inline bool isAlterFinish();
    inline bool isRunCheck();
    inline bool isCheckFinish();

signals:
    void sigOpen();
    void sigAlter(const QString &alter_command, QString *lastInsertId = nullptr);
    void sigCheckItemResult(const QString &checkCommand, const QString &checkValue);
    void sigOpenFinish();
    void sigAlterFinish();
    void sigCheckFinish();
    void sigFinish();

public slots:
    void slotOpen();
    void slotAlter(const QString &alterCommand, QString *lastInsertId = nullptr);
    void slotCheckItemResult(const QString &checkCommand, const QString &checkValue);

private:
    QSqlDatabase *db;
    MyDatabasePriveData *d;

    void readyOpen();
    void readyAlter();
    void readyCheckItemResult();
    void openFinish();
    void alterFinish();
    void checkItemResultFinish();
    void resetAllData();
    bool exec_alter(const QString &alter_command, QString * const lastInsertId);
    bool checkItemResult(const QString &checkCommand, const QString &checkValue);
};

class MyDatabase : public QObject
{
    Q_OBJECT
public:
    explicit MyDatabase(QObject *parent = nullptr);
    ~MyDatabase();

    void init(const QString &host, int port, const QString &databaseName, const QString &user_name, const QString &password);
    inline void setHostName(const QString& host)     { db->setHostName(host);      }
    inline void setPort(int port)                    { db->setPort(port);          }
    inline void setDatabaseName(const QString& name) { db->setDatabaseName(name);  }
    inline void setUserName(const QString& name)     { db->setUserName(name);      }
    inline void setPassword(const QString& password) { db->setPassword(password);  }
    inline QString databaseName() const              { return db->databaseName();  }
    inline QString userName() const                  { return db->userName();      }
    inline QString password() const                  { return db->password();      }
    inline QString hostName() const                  { return db->hostName();      }
    inline QString driverName() const                { return db->driverName();    }
    inline int port() const                          { return db->port();          }
    inline void close()                              { db->close();                }
    inline QSqlDatabase *getDB() const               { return db->getDB();         }
    //除了 open() 的结果，获取其他操作结果统一调用函数 resutl()
    inline bool isOpen() const                       { return db->getDB()->isOpen();}
    bool result() const;

    void open();
    void alter(const QString &alterCommand, QString * const lastInsertId = nullptr);
    void write(const QString &tableName, const QString &items, const QString &values, QString * const lastInsertId = nullptr);
    void write(const QString &tableName, const QStringList &items, const QStringList &values, QString * const lastInsertId = nullptr);
    void update(const QString &tableName, const QStringList &items, const QStringList &values,
                const QStringList &whereItems, const QStringList &whereValues);
    void update(const QString &tableName, const QStringList &items, const QStringList &values,
                const QString &whereItem, const QString &whereValue);
    void isItemExist(
            const QString &snTableName,
            const QString &resultTableName,
            const QString &snTable_resultTabIdField,
            const QString &resultTableIdField,
            const QString &sortField,
            const QString &snField,
            const QString &SN,
            const QString &resultField,
            const QString &resultData);
    void deleteData(const QString &tableName, const QString &fidle, const QString &value);
    void waitOpenFinish();
    //所有会改动数据库的操作，均调用此函数
    void waitAlterFinish();
    void waitCheckItemResultFinish();
    void waitForFinish();

signals:
    void sigOpenFinish();
    void sigAlterFinish();

private:
    MyDatabasePrivate *db;
    QThread *thread;
};

#endif // MYDATABASE_H
