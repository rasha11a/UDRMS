#include "roleselectionwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScreen>
#include <QGuiApplication>

RoleSelectionWindow::RoleSelectionWindow(University& university, QWidget* parent)
    : QWidget(parent), university(university) {
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(620, 380);
    setStyleSheet("background-color: #141820;");

    if (QScreen* screen = QGuiApplication::primaryScreen()) {
        QRect sg = screen->geometry();
        move((sg.width() - width()) / 2, (sg.height() - height()) / 2);
    }

    setupUI();
}

void RoleSelectionWindow::setupUI() {
    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(48, 40, 48, 48);
    root->setSpacing(0);

    // ── Header ──────────────────────────────────────────
    QLabel* tag = new QLabel("UDRMS  —  ENSIA 2025/2026", this);
    tag->setStyleSheet("color: #52c785; font-size: 11px; font-weight: 600;"
                       "font-family: 'Segoe UI'; letter-spacing: 1px;");
    tag->setAlignment(Qt::AlignCenter);

    QLabel* title = new QLabel("Who are you?", this);
    title->setStyleSheet("color: #ffffff; font-size: 30px; font-weight: 700;"
                         "font-family: 'Segoe UI'; margin-top: 10px;");
    title->setAlignment(Qt::AlignCenter);

    QLabel* subtitle = new QLabel("Choose your role to continue", this);
    subtitle->setStyleSheet("color: #8b909a; font-size: 13px; font-family: 'Segoe UI';");
    subtitle->setAlignment(Qt::AlignCenter);

    // ── Divider ─────────────────────────────────────────
    QFrame* div = new QFrame(this);
    div->setFixedHeight(1);
    div->setStyleSheet("background-color: #2a2f38;");

    // ── Role buttons ────────────────────────────────────
    auto makeRoleCard = [&](const QString& icon, const QString& role,
                            const QString& desc, const QString& color,
                            const QString& hoverColor) -> QPushButton* {
        QPushButton* btn = new QPushButton(this);
        btn->setFixedHeight(110);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setText(icon + "\n" + role + "\n" + desc);
        btn->setStyleSheet(QString(
                               "QPushButton { background-color: #1e2228; border: 2px solid #2a2f38;"
                               "border-radius: 12px; color: #ffffff; font-family: 'Segoe UI';"
                               "padding: 12px; text-align: center; }"
                               "QPushButton:hover { background-color: %1; border: 2px solid %2; }"
                               "QPushButton:pressed { background-color: #1a2030; }"
                               ).arg(hoverColor).arg(color));
        return btn;
    };

    QPushButton* adminBtn = makeRoleCard(
        "🔧", "Administrator",
        "Manage students, dormitories\nand restaurant menus",
        "#2d6a4f", "#1a3d2e"
        );

    QPushButton* studentBtn = makeRoleCard(
        "🎓", "Student",
        "View your room, today's menu\nand contact services",
        "#1b4d5e", "#0f2d38"
        );

    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->setSpacing(20);
    btnRow->addWidget(adminBtn);
    btnRow->addWidget(studentBtn);

    root->addWidget(tag);
    root->addWidget(title);
    root->addWidget(subtitle);
    root->addSpacing(20);
    root->addWidget(div);
    root->addSpacing(24);
    root->addLayout(btnRow);

    connect(adminBtn,   &QPushButton::clicked, this, &RoleSelectionWindow::adminSelected);
    connect(studentBtn, &QPushButton::clicked, this, &RoleSelectionWindow::studentSelected);
}