#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

class DataBase
{
public:
    DataBase();
    ~DataBase();

    bool connectToDatabase(const QString &path);
    bool createTable();
    bool addUser(const QString &username, const QString &password);
    bool userExists(const QString &username);

    bool checkLogin(const QString &username, const QString &password);
    bool deductCredit(const QString &username, int amount); // Returns true if successful
    bool addCredit(const QString &username, int amount);

    int getCredits(const QString &username);


private:
    QSqlDatabase db;
};

#endif // DATABASE_H
