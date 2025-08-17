#include "database.h"

DataBase::DataBase() {}

DataBase::~DataBase() {
    if (db.isOpen()) {
        db.close();
    }
}

bool DataBase::connectToDatabase(const QString &path) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open()) {
        qDebug() << "Error: Could not open database -" << db.lastError().text();
        return false;
    }
    return true;
}

bool DataBase::createTable() {
    QSqlQuery query;
    QString createTableSQL =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE, "
        "password TEXT, "
        "credits INTEGER"
        ");";

    if (!query.exec(createTableSQL)) {
        qDebug() << "Error creating table:" << query.lastError().text();
        return false;
    }
    return true;
}


bool DataBase::addUser(const QString &username, const QString &password) {
    if (userExists(username)) {
        qDebug() << "User already exists!";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, credits) "
                  "VALUES (:username, :password, :credits)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);  // In production, store hashed passwords!
    query.bindValue(":credits", 10);         // Give 10 credits on signup

    if (!query.exec()) {
        qDebug() << "Error adding user:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DataBase::userExists(const QString &username) {
    QSqlQuery query;
    query.prepare("SELECT username FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error checking user:" << query.lastError().text();
        return false;
    }

    return query.next();
}

bool DataBase::checkLogin(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error during login check:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        return storedPassword == password; // TODO: compare hashed values in production
    }

    return false; // No such user
}

bool DataBase::deductCredit(const QString &username, int amount)
{
    QSqlQuery query;

    // First, check current credits
    query.prepare("SELECT credits FROM users WHERE username = :username");
    query.bindValue(":username", username);
    if(!query.exec()) { qDebug() << query.lastError().text(); return false; }

    if(query.next()) {
        int credits = query.value(0).toInt();
        if(credits < amount) {
            qDebug() << "Not enough credits!";
            return false;
        }

        // Deduct credits
        query.prepare("UPDATE users SET credits = credits - :amount WHERE username = :username");
        query.bindValue(":amount", amount);
        query.bindValue(":username", username);
        if(!query.exec()) { qDebug() << query.lastError().text(); return false; }

        return true;
    }
    return false; // User not found
}

bool DataBase::addCredit(const QString &username, int amount) {
    if (!db.isOpen()) {
        qDebug() << "Database not open!";
        return false;
    }

    // Check if the user exists
    if (!userExists(username)) {
        qDebug() << "Error adding credits: User '" << username << "' not found.";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE users SET credits = credits + :amount WHERE username = :username");
    query.bindValue(":amount", amount);
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error adding credits:" << query.lastError().text();
        return false;
    }

    return true;
}
int DataBase::getCredits(const QString &username) {
    if (!db.isOpen()) {
        qDebug() << "Database not open!";
        return -1;
    }

    QSqlQuery query;
    query.prepare("SELECT credits FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error getting credits:" << query.lastError().text();
        return -1;
    }

    if (query.next()) {
        return query.value(0).toInt();
    }

    qDebug() << "User not found!";
    return -1;
}
