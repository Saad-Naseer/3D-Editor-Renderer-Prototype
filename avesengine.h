#ifndef AVESENGINE_H
#define AVESENGINE_H

#include <QMainWindow>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QLabel>
#include <QGridLayout>
#include <QQuickWidget>
#include <QQmlContext>
#include <QUrl>
#include "customgeometry.h" // Your exporter class


#include "database.h"

namespace Ui {
class AvesEngine;
}

class AvesEngine : public QMainWindow
{
    Q_OBJECT

public:
    explicit AvesEngine(DataBase* dbPtr, const QString &username, QWidget *parent = nullptr);
    ~AvesEngine();

private:
    Ui::AvesEngine *ui;
    CustomGeometry* cubeGeometry; // pointer to your cube geometry

    DataBase* db;
    QString currentUser;

    QAction *importAction;
    QAction *exportAction;
    QAction *exitAction;

    QLabel *creditsLabel;

    void setupMenuBar();
    void setup3DEngine(QVBoxLayout* layout);
    void printQmlItems(QQuickItem* parent, int indent);

};

#endif // AVESENGINE_H
