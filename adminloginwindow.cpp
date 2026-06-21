#include "adminloginwindow.h"
#include <QVBoxLayout>
#include <QScreen>
#include <QGuiApplication>

AdminLoginWindow::AdminLoginWindow(QWidget* parent) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(400, 320);
    setStyleSheet("background-color: #141820;");

    if (QScreen* screen = QGuiApplication::primaryScreen()) {
        QRect sg = screen->geometry();
        move((sg.width()-width())/2, (sg.height()-height())/2);
    }
    setupUI();
}

void AdminLoginWindow::setupUI() {
    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(44, 36, 44, 44);
    root->setSpacing(0);

    QPushButton* backBtn = new QPushButton("← Back", this);
    backBtn->setStyleSheet(
        "QPushButton { background:transparent; color:#8b909a; border:none;"
        "font-size:12px; font-family:'Segoe UI'; text-align:left; }"
        "QPushButton:hover { color:#52c785; }");
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setFixedWidth(80);

    QLabel* icon = new QLabel("🔧", this);
    icon->setStyleSheet("font-size:36px;");
    icon->setAlignment(Qt::AlignCenter);

    QLabel* title = new QLabel("Administrator Login", this);
    title->setStyleSheet("color:#ffffff; font-size:22px; font-weight:700;"
                         "font-family:'Segoe UI'; margin-top:6px;");
    title->setAlignment(Qt::AlignCenter);

    QLabel* subtitle = new QLabel("Enter the administrator password", this);
    subtitle->setStyleSheet("color:#8b909a; font-size:12px; font-family:'Segoe UI';");
    subtitle->setAlignment(Qt::AlignCenter);

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Password (hint: admin123)");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet(
        "QLineEdit { background-color:#1e2228; border:1px solid #3a4150;"
        "border-radius:6px; padding:10px; color:#ffffff; font-family:'Segoe UI'; }"
        "QLineEdit:focus { border:1px solid #52c785; }");

    errorLabel = new QLabel("", this);
    errorLabel->setStyleSheet("color:#e05555; font-size:11px; font-family:'Segoe UI';");
    errorLabel->setAlignment(Qt::AlignCenter);

    QPushButton* loginBtn = new QPushButton("Sign In  →", this);
    loginBtn->setFixedHeight(44);
    loginBtn->setCursor(Qt::PointingHandCursor);
    loginBtn->setStyleSheet(
        "QPushButton { background-color:#2d6a4f; color:#ffffff; border:none;"
        "border-radius:8px; font-size:14px; font-weight:700; font-family:'Segoe UI'; }"
        "QPushButton:hover { background-color:#36805f; }");

    root->addWidget(backBtn, 0, Qt::AlignLeft);
    root->addSpacing(8);
    root->addWidget(icon);
    root->addWidget(title);
    root->addWidget(subtitle);
    root->addSpacing(20);
    root->addWidget(passwordInput);
    root->addSpacing(8);
    root->addWidget(errorLabel);
    root->addSpacing(12);
    root->addWidget(loginBtn);
    root->addStretch(1);

    connect(loginBtn, &QPushButton::clicked, this, &AdminLoginWindow::onLoginClicked);
    connect(backBtn,  &QPushButton::clicked, this, &AdminLoginWindow::backRequested);
    connect(passwordInput, &QLineEdit::returnPressed, this, &AdminLoginWindow::onLoginClicked);
}

void AdminLoginWindow::onLoginClicked() {
    if (passwordInput->text() == "admin123") {
        errorLabel->clear();
        passwordInput->clear();
        emit loginSuccess();
    } else {
        errorLabel->setText("Incorrect password. Please try again.");
        passwordInput->clear();
    }
}