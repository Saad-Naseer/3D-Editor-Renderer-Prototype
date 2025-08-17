#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    login_page_layout  = new QGridLayout();
    ui->centralwidget->setLayout(login_page_layout);

    user = new QLabel(this);
    pass = new QLabel(this);
    username = new QLineEdit(this);
    password = new QLineEdit(this);

    h_left = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
    h_right = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
    h_top = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    h_bottom = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    login_page_layout->addItem(h_top,0,0);
    login_page_layout->addItem(h_left,1,0);
    login_page_layout->addWidget(user,1,1);
    login_page_layout->addWidget(username,1,2);
    login_page_layout->addWidget(pass,2,1);
    login_page_layout->addWidget(password,2,2);
    login_page_layout->addItem(h_right,0,5);
    login_page_layout->addItem(h_bottom,5,0);


    user->setText("Username: ");
    pass->setText("Password: ");

    sign_up = new QPushButton(this);
    sign_up->setText("Sign Up");
    login = new QPushButton(this);
    login->setText("Login");

    btns_layout = new QHBoxLayout();
    login_page_layout->addLayout(btns_layout,3,2);
    btns_layout->addWidget(sign_up);
    btns_layout->addWidget(login);

    db.connectToDatabase("users.db");
    db.createTable();

    connect(sign_up, &QPushButton::clicked, this, &MainWindow::add_user);
    connect(login, &QPushButton::clicked, this, &MainWindow::login_user);

    out = new QLabel();
    login_page_layout->addWidget(out,4,2);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add_user() // ✅ Slot definition
{
    QString username_string = username->text();
    QString password_string = password->text();

    if (db.addUser(username_string, password_string)) {
        qDebug() << "User added successfully!";
        out->setText("User added successfully!");
    } else {
        qDebug() << "Failed to add user (maybe already exists)";
        out->setText("Failed to add user (maybe already exists)");

    }
}


void MainWindow::login_user() // ✅ Slot definition
{
    QString username_string = username->text();
    QString password_string = password->text();
    if (db.checkLogin(username_string, password_string)) {
        qDebug() << "Login successful!";
        out->setText("Login successful!");

        // Open main app page here
        // Create the main app window
        appWindow = new AvesEngine(&db, username_string);
        appWindow->resize(800, 600);  // Optional: set size
        appWindow->show();

        // Close login window
        this->close();
    } else {
        qDebug() << "Invalid username or password!";
        out->setText("Invalid username or password!");

        // Optionally show QMessageBox warning
    }
}
