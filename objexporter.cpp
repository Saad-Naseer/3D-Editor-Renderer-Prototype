#include "objexporter.h"
#include <QDir>

ObjExporter::ObjExporter(DataBase* dbPtr, QObject *parent) : QObject(parent), db(dbPtr)
{
}

bool ObjExporter::exportAndDeduct(const QString &username, const QString &filePath, float size)
{
    if (db->deductCredit(username, 1)) {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << "Could not open file for writing:" << filePath;
            db->addCredit(username, 1); // Refund the credit
            return false;
        }

        QTextStream out(&file);
        writeCube(out, size);
        file.close();

        qDebug() << "Export successful, 1 credit deducted!";
        return true;
    } else {
        qDebug() << "Export failed: not enough credits!";
        return false;
    }
}

void ObjExporter::writeCube(QTextStream &out, float size)
{
    // The "size" parameter will scale the cube
    float s = size / 2.0f;

    // Write vertex data
    out << "v " << -s << " " << s << " " << s << "\n";
    out << "v " << s << " " << s << " " << s << "\n";
    out << "v " << s << " " << -s << " " << s << "\n";
    out << "v " << -s << " " << -s << " " << s << "\n";
    out << "v " << -s << " " << s << " " << -s << "\n";
    out << "v " << s << " " << s << " " << -s << "\n";
    out << "v " << s << " " << -s << " " << -s << "\n";
    out << "v " << -s << " " << -s << " " << -s << "\n";

    // Write face data
    out << "f 1 2 3 4\n";
    out << "f 2 6 7 3\n";
    out << "f 6 5 8 7\n";
    out << "f 5 1 4 8\n";
    out << "f 5 6 2 1\n";
    out << "f 4 3 7 8\n";
}
