#ifndef OBJEXPORTER_H
#define OBJEXPORTER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "database.h" // Include your DataBase header

class ObjExporter : public QObject
{
    Q_OBJECT
public:
    explicit ObjExporter(DataBase* dbPtr, QObject *parent = nullptr);

    Q_INVOKABLE bool exportAndDeduct(const QString &username, const QString &filePath, float size);

private:
    DataBase* db;
    void writeCube(QTextStream &out, float size);
};

#endif // OBJEXPORTER_H
