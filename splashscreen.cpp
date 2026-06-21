#include "splashscreen.h"
#include <QVBoxLayout>
#include <QTimer>
#include <QScreen>
#include <QGuiApplication>

SplashScreen::SplashScreen(QWidget* parent) : QWidget(parent) {
    // Frameless, centered popup window (no title bar, no min/max/close buttons)
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setFixedSize(420, 260);
    setStyleSheet("background-color: #1e2228;");

    // ── Content ──────────────────────────────────────────
    titleLabel = new QLabel("UDRMS", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "color: #52c785; font-size: 42px; font-weight: 700; font-family: 'Segoe UI';"
        );

    subtitleLabel = new QLabel("University Dormitory & Restaurant\nManagement System", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet(
        "color: #b8bcc4; font-size: 13px; font-family: 'Segoe UI';"
        );

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addStretch(1);
    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addStretch(1);

    // ── Center the splash on screen ─────────────────────
    if (QScreen* screen = QGuiApplication::primaryScreen()) {
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - width()) / 2;
        int y = (screenGeometry.height() - height()) / 2;
        move(x, y);
    }

    // ── Fade-in animation ────────────────────────────────
    // QWidget has a "windowOpacity" property we can animate from 0.0 to 1.0
    setWindowOpacity(0.0);

    fadeInAnim = new QPropertyAnimation(this, "windowOpacity", this);
    fadeInAnim->setDuration(600);       // 600 ms fade in
    fadeInAnim->setStartValue(0.0);
    fadeInAnim->setEndValue(1.0);

    fadeOutAnim = new QPropertyAnimation(this, "windowOpacity", this);
    fadeOutAnim->setDuration(400);      // 400 ms fade out
    fadeOutAnim->setStartValue(1.0);
    fadeOutAnim->setEndValue(0.0);

    startSequence();
}

void SplashScreen::startSequence() {
    fadeInAnim->start();

    // After fade-in finishes, wait 1200ms, then start fade-out
    connect(fadeInAnim, &QPropertyAnimation::finished, this, [this]() {
        QTimer::singleShot(1200, this, [this]() {
            fadeOutAnim->start();
        });
    });

    // After fade-out finishes, tell whoever is listening that we're done
    connect(fadeOutAnim, &QPropertyAnimation::finished, this, [this]() {
        emit finished();
        this->close();
    });
}