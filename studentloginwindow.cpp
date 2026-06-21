#include "studentloginwindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QScreen>
#include <QGuiApplication>
#include <QString>

StudentLoginWindow::StudentLoginWindow(University& university, QWidget* parent)
    : QWidget(parent), university(university) {
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(440, 380);
    setStyleSheet("background-color: #141820;");

    if (QScreen* screen = QGuiApplication::primaryScreen()) {
        QRect sg = screen->geometry();
        move((sg.width() - width()) / 2, (sg.height() - height()) / 2);
    }

    setupUI();
}

void StudentLoginWindow::setupUI() {
    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(44, 40, 44, 44);
    root->setSpacing(0);

    // back button top-left
    QPushButton* backBtn = new QPushButton("← Back", this);
    backBtn->setStyleSheet(
        "QPushButton { background: transparent; color: #8b909a; border: none;"
        "font-size: 12px; font-family: 'Segoe UI'; text-align: left; }"
        "QPushButton:hover { color: #52c785; }"
        );
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setFixedWidth(80);

    QLabel* icon = new QLabel("🎓", this);
    icon->setStyleSheet("font-size: 36px;");
    icon->setAlignment(Qt::AlignCenter);

    QLabel* title = new QLabel("Student Login", this);
    title->setStyleSheet("color: #ffffff; font-size: 24px; font-weight: 700;"
                         "font-family: 'Segoe UI'; margin-top: 6px;");
    title->setAlignment(Qt::AlignCenter);

    QLabel* subtitle = new QLabel("Enter your full name and student ID", this);
    subtitle->setStyleSheet("color: #8b909a; font-size: 12px; font-family: 'Segoe UI';");
    subtitle->setAlignment(Qt::AlignCenter);

    // inputs
    nameInput = new QLineEdit(this);
    nameInput->setPlaceholderText("e.g. Racha Brahimi");
    nameInput->setStyleSheet(
        "QLineEdit { background-color: #1e2228; border: 1px solid #3a4150;"
        "border-radius: 6px; padding: 10px; color: #ffffff; font-family: 'Segoe UI'; }"
        "QLineEdit:focus { border: 1px solid #52c785; }"
        );

    idInput = new QLineEdit(this);
    idInput->setPlaceholderText("e.g. S001");
    idInput->setStyleSheet(nameInput->styleSheet());

    QFormLayout* form = new QFormLayout();
    form->setSpacing(10);
    form->setContentsMargins(0, 16, 0, 0);
    form->addRow("Full Name:", nameInput);
    form->addRow("Student ID:", idInput);

    // style the form labels
    form->labelForField(nameInput)->setStyleSheet(
        "color: #b8bcc4; font-family: 'Segoe UI'; font-size: 12px;");
    form->labelForField(idInput)->setStyleSheet(
        "color: #b8bcc4; font-family: 'Segoe UI'; font-size: 12px;");

    errorLabel = new QLabel("", this);
    errorLabel->setStyleSheet("color: #e05555; font-size: 11px;"
                              "font-family: 'Segoe UI';");
    errorLabel->setAlignment(Qt::AlignCenter);

    QPushButton* loginBtn = new QPushButton("Sign In  →", this);
    loginBtn->setFixedHeight(44);
    loginBtn->setCursor(Qt::PointingHandCursor);
    loginBtn->setStyleSheet(
        "QPushButton { background-color: #1b4d5e; color: #ffffff; border: none;"
        "border-radius: 8px; font-size: 14px; font-weight: 700;"
        "font-family: 'Segoe UI'; }"
        "QPushButton:hover { background-color: #2a7a93; }"
        "QPushButton:pressed { background-color: #143a47; }"
        );

    root->addWidget(backBtn, 0, Qt::AlignLeft);
    root->addSpacing(8);
    root->addWidget(icon);
    root->addWidget(title);
    root->addWidget(subtitle);
    root->addLayout(form);
    root->addSpacing(8);
    root->addWidget(errorLabel);
    root->addSpacing(12);
    root->addWidget(loginBtn);
    root->addStretch(1);

    connect(loginBtn, &QPushButton::clicked, this, &StudentLoginWindow::onLoginClicked);
    connect(backBtn,  &QPushButton::clicked, this, &StudentLoginWindow::backRequested);
    // also allow pressing Enter to login
    connect(idInput,  &QLineEdit::returnPressed, this, &StudentLoginWindow::onLoginClicked);
}

void StudentLoginWindow::onLoginClicked() {
    QString name = nameInput->text().trimmed();
    QString id   = idInput->text().trimmed();

    if (name.isEmpty() || id.isEmpty()) {
        errorLabel->setText("Please enter both your name and student ID.");
        return;
    }

    // Find student by ID across all dormitories
    Student* found = university.findStudent(id.toStdString());

    if (!found) {
        errorLabel->setText("Student ID not found. Please contact the administrator.");
        return;
    }

    // Verify the name matches (case-insensitive)
    QString storedName = QString::fromStdString(found->getFullName()).toLower();
    QString enteredName = name.toLower();

    if (storedName != enteredName) {
        errorLabel->setText("Name does not match our records. Please try again.");
        return;
    }

    errorLabel->clear();
    emit loginSuccess(found);
}