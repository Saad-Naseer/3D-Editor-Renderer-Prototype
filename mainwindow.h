#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlabel.h>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSpacerItem>
#include <QPushButton>
#include "database.h"
#include "avesengine.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DataBase db;

    QGridLayout *login_page_layout;
    QSpacerItem *h_left, *h_right, *h_top, *h_bottom;

    QLabel *user;
    QLineEdit *username;

    QLabel *pass;
    QLineEdit *password;
    QLabel *out;

    QHBoxLayout *btns_layout;
    QPushButton *sign_up, *login;

    AvesEngine *appWindow;

private slots:
    void add_user();
    void login_user();

};
#endif // MAINWINDOW_H
