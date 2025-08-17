#include "avesengine.h"
#include "ui_avesengine.h"
#include <QQuickWidget>
#include <QQuickItem>
#include <QVector3D>
#include <QVariant>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

AvesEngine::AvesEngine(DataBase* dbPtr, const QString &username, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AvesEngine), db(dbPtr), currentUser(username)
{
    ui->setupUi(this);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    setup3DEngine(mainLayout);

    creditsLabel = new QLabel("Credits: " + QString::number(db->getCredits(currentUser)));
    mainLayout->addWidget(creditsLabel);

    setupMenuBar();
    cubeGeometry = new CustomGeometry();
}

AvesEngine::~AvesEngine()
{
    delete ui;
}

void AvesEngine::setupMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu("File");

    importAction = new QAction("Import", this);
    exportAction = new QAction("Export", this);
    exitAction   = new QAction("Exit", this);

    fileMenu->addAction(importAction);
    fileMenu->addAction(exportAction);
    fileMenu->addAction(exitAction);

    connect(importAction, &QAction::triggered, this, [=]() {
        qDebug() << "Import clicked";
        QMessageBox::information(this, "Import", "Import clicked!");
    });

    connect(exportAction, &QAction::triggered, this, [=]() {
        // Here is the logic you requested.
        // Check if the user has 0 or fewer credits.
        if (db->getCredits(currentUser) <= 0) {
            QMessageBox::warning(this, "Export Failed", "You have 0 credits. Please add more credits to export.");
            return; // Exit the function, preventing export.
        }

        QString filePath = QFileDialog::getSaveFileName(this, "Save Cube OBJ", QDir::homePath(), "OBJ Files (*.obj)");
        if (filePath.isEmpty())
            return;

        /*if (cubeGeometry->saveOBJ(filePath)) {
            QMessageBox::information(this, "Export", "Cube saved as OBJ successfully!");
            db->deductCredit(currentUser, 1);
            creditsLabel->setText("Credits: " + QString::number(db->getCredits(currentUser)));
        } else {
            QMessageBox::warning(this, "Export Failed", "Failed to save the OBJ file.");
        }*/
    });

    connect(exitAction, &QAction::triggered, this, [=]() {
        close();
    });
}

void AvesEngine::printQmlItems(QQuickItem* parent, int indent)
{
    if (!parent)
        return;

    QString indentStr = QString(indent, ' ');
    qDebug() << indentStr << "Item Name:" << parent->objectName() << " (Type:" << parent->metaObject()->className() << ")";

    QList<QQuickItem*> children = parent->childItems();
    for (QQuickItem* child : children) {
        printQmlItems(child, indent + 4);
    }
}

void AvesEngine::setup3DEngine(QVBoxLayout* layout)
{
    QQuickWidget *quickWidget = new QQuickWidget(this);
    quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    quickWidget->setSource(QUrl("qrc:/editor3d.qml"));
    layout->insertWidget(0, quickWidget, 1);
}
