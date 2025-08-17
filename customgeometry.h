#pragma once
#include <QQuick3DGeometry>
#include <QFile>
#include <QTextStream>

class CustomGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CustomGeometry)

public:
    CustomGeometry();
    bool saveOBJ(const QString& filePath) const;
    bool saveCube(const QString& filePath) const;
    void updateData();




private:

};
