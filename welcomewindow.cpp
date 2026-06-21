#include "welcomewindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScreen>
#include <QGuiApplication>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>

// ════════════════════════════════════════════════════════════════════════════
//  ILLUSTRATION WIDGET
// ════════════════════════════════════════════════════════════════════════════

IllustrationWidget::IllustrationWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(360, 300);
}

void IllustrationWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    drawScene(p);
}

void IllustrationWidget::drawStars(QPainter& p) {
    p.setBrush(QColor(232, 232, 232, 140));
    p.setPen(Qt::NoPen);
    QList<QPoint> stars = {{30,18},{90,8},{160,22},{240,6},
                           {310,18},{380,10},{440,26},{490,8}};
    for (auto& s : stars)
        p.drawEllipse(s.x(), s.y(), 3, 3);
}

void IllustrationWidget::drawTree(QPainter& p, int cx, int cy, int r1, int r2) {
    p.setBrush(QColor(61, 43, 31));
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(cx - 4, cy, 8, 28, 2, 2);
    p.setBrush(QColor(20, 52, 34));
    p.drawEllipse(cx - r1, cy - r1*2, r1*2, r1*2);
    p.setBrush(QColor(30, 80, 52));
    p.drawEllipse(cx - r2, cy - r2*2 - 6, r2*2, r2*2);
}

void IllustrationWidget::drawLamp(QPainter& p, int x, int y) {
    p.setBrush(QColor(57, 68, 85));
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(x - 3, y, 6, 45, 2, 2);
    p.drawRoundedRect(x - 8, y - 4, 16, 6, 2, 2);
    p.setBrush(QColor(245, 200, 98, 170));
    p.drawEllipse(x - 7, y - 6, 14, 7);
}

void IllustrationWidget::drawStudent(QPainter& p, int x, int y, int variant) {
    QColor skins[3]  = {QColor(232,184,138), QColor(200,147,106), QColor(232,184,138)};
    QColor shirts[3] = {QColor(27,77,94), QColor(82,199,133), QColor(139,58,58)};
    QColor pants     = QColor(42, 48, 64);

    p.setPen(Qt::NoPen);
    p.setBrush(pants);
    p.drawRoundedRect(x - 7, y - 18, 7, 20, 2, 2);
    p.drawRoundedRect(x + 1, y - 18, 7, 20, 2, 2);
    p.setBrush(shirts[variant]);
    p.drawRoundedRect(x - 9, y - 40, 18, 24, 3, 3);
    p.setBrush(skins[variant]);
    p.drawEllipse(x - 9, y - 52, 18, 18);
    p.setBrush(QColor(42, 31, 26));
    p.drawEllipse(x - 9, y - 58, 18, 12);

    if (variant == 0) {
        p.setBrush(QColor(224,158,58));
        p.drawRoundedRect(x + 10, y - 36, 12, 16, 1, 1);
    } else if (variant == 1) {
        p.setBrush(QColor(27,77,94));
        p.drawRoundedRect(x + 8, y - 42, 13, 18, 3, 3);
    }
}

void IllustrationWidget::drawWing(QPainter& p, int x, int y, int w, int h,
                                  const QString& label, const QColor& signColor) {
    p.setBrush(QColor(38, 46, 61));
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(x, y, w, h, 3, 3);
    p.setBrush(QColor(50, 60, 75));
    p.drawRoundedRect(x - 4, y - 4, w + 8, 8, 2, 2);
    p.setBrush(QColor(82, 199, 133, 110));
    p.drawRoundedRect(x + 8, y + 14, 34, 26, 2, 2);
    p.drawRoundedRect(x + w - 42, y + 14, 34, 26, 2, 2);
    p.setBrush(QColor(22, 28, 40));
    p.drawRoundedRect(x + w/2 - 14, y + h - 44, 28, 44, 2, 2);
    p.setBrush(signColor);
    p.drawRoundedRect(x + 6, y - 18, w - 12, 15, 3, 3);
    p.setPen(QColor(255, 255, 255));
    p.setFont(QFont("Segoe UI", 6, QFont::Bold));
    p.drawText(QRect(x + 6, y - 18, w - 12, 15), Qt::AlignCenter, label);
    p.setPen(Qt::NoPen);
}

void IllustrationWidget::drawBuilding(QPainter& p) {
    int W = width();
    int cx = W / 2;

    p.setBrush(QColor(38, 46, 61));
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(cx - 120, 80, 240, 185, 3, 3);
    p.setBrush(QColor(50, 60, 75));
    p.drawRoundedRect(cx - 112, 88, 224, 175, 2, 2);

    QPainterPath roof;
    roof.moveTo(cx - 135, 83);
    roof.lineTo(cx, 38);
    roof.lineTo(cx + 135, 83);
    roof.closeSubpath();
    p.setBrush(QColor(30, 80, 58));
    p.drawPath(roof);
    p.setBrush(QColor(45, 106, 79));
    QPainterPath roof2;
    roof2.moveTo(cx - 130, 83);
    roof2.lineTo(cx, 44);
    roof2.lineTo(cx + 130, 83);
    roof2.closeSubpath();
    p.drawPath(roof2);

    p.setBrush(QColor(50, 60, 75));
    p.drawRoundedRect(cx - 9, 40, 18, 30, 2, 2);
    p.drawRoundedRect(cx - 13, 36, 26, 8, 2, 2);

    // windows row 1 (amber)
    QList<int> wx1 = {cx - 100, cx - 50, cx + 8, cx + 52};
    p.setBrush(QColor(245, 200, 98, 160));
    for (int wx : wx1) p.drawRoundedRect(wx, 100, 32, 26, 2, 2);
    p.setPen(QPen(QColor(22, 28, 40), 1.2));
    for (int wx : wx1) {
        p.drawLine(wx+16, 100, wx+16, 126);
        p.drawLine(wx, 113, wx+32, 113);
    }
    p.setPen(Qt::NoPen);

    // windows row 2 (green)
    p.setBrush(QColor(82, 199, 133, 90));
    for (int wx : wx1) p.drawRoundedRect(wx, 142, 32, 26, 2, 2);

    // door
    p.setBrush(QColor(22, 28, 40));
    p.drawRoundedRect(cx - 28, 202, 56, 63, 3, 3);
    p.setBrush(QColor(82, 199, 133, 90));
    p.drawRoundedRect(cx - 22, 208, 20, 48, 2, 2);
    p.drawRoundedRect(cx + 2, 208, 20, 48, 2, 2);
    p.setBrush(QColor(245, 200, 98));
    p.drawEllipse(cx - 4, 232, 5, 5);

    // door sign
    p.setBrush(QColor(45, 106, 79));
    p.drawRoundedRect(cx - 40, 193, 80, 12, 3, 3);
    p.setPen(QColor(255, 255, 255));
    p.setFont(QFont("Segoe UI", 6, QFont::Bold));
    p.drawText(QRect(cx - 40, 193, 80, 12), Qt::AlignCenter, "DORMITORY  A");
    p.setPen(Qt::NoPen);

    // steps
    p.setBrush(QColor(50, 60, 75));
    p.drawRoundedRect(cx - 40, 262, 80, 7, 1, 1);
    p.drawRoundedRect(cx - 32, 257, 64, 6, 1, 1);
}

void IllustrationWidget::drawScene(QPainter& p) {
    int W = width();
    int H = height();
    int cx = W / 2;

    // background — rounded card
    p.setBrush(QColor(22, 28, 40));
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(0, 0, W, H, 14, 14);

    drawStars(p);

    // moon
    p.setBrush(QColor(50, 62, 80));
    p.drawEllipse(W - 70, 14, 38, 38);
    p.setBrush(QColor(22, 28, 40));
    p.drawEllipse(W - 58, 8, 34, 34);

    // ground
    p.setBrush(QColor(18, 24, 36));
    p.drawRect(0, 262, W, H - 262);

    // pathway
    p.setBrush(QColor(30, 38, 52));
    p.drawRoundedRect(cx - 50, 262, 100, H - 262, 2, 2);

    // wings
    drawWing(p, 12, 148, 118, 118, "RESTAURANT",  QColor(180, 120, 20));
    drawWing(p, W - 130, 148, 118, 118, "STUDY ROOM", QColor(22, 68, 90));

    // main building
    drawBuilding(p);

    // trees
    drawTree(p, 8,  254, 20, 15);
    drawTree(p, W - 8, 254, 20, 15);
    drawTree(p, 136, 262, 14, 10);
    drawTree(p, W - 136, 262, 14, 10);

    // bushes
    p.setBrush(QColor(20, 52, 34));
    p.drawEllipse(cx - 135, 262, 30, 14);
    p.drawEllipse(cx + 105, 262, 30, 14);

    // lamps
    drawLamp(p, cx - 78, 222);
    drawLamp(p, cx + 78, 222);

    // students
    drawStudent(p, 118, 270, 0);
    drawStudent(p, cx + 44, 272, 1);
    drawStudent(p, W - 110, 270, 2);
}

// ════════════════════════════════════════════════════════════════════════════
//  WELCOME WINDOW
// ════════════════════════════════════════════════════════════════════════════

WelcomeWindow::WelcomeWindow(QWidget* parent)
    : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(920, 560);

    if (QScreen* screen = QGuiApplication::primaryScreen()) {
        QRect sg = screen->geometry();
        move((sg.width() - width()) / 2, (sg.height() - height()) / 2);
    }

    setWindowOpacity(0.0);
    setStyleSheet("background-color: #141820;");
    setupUI();

    QPropertyAnimation* fadeIn = new QPropertyAnimation(this, "windowOpacity", this);
    fadeIn->setDuration(500);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);
    fadeIn->start();
}

void WelcomeWindow::setupUI() {
    QHBoxLayout* root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);
    root->addWidget(buildLeftPanel(), 1);
    root->addWidget(buildRightPanel());
}

QWidget* WelcomeWindow::buildLeftPanel() {
    QWidget* panel = new QWidget(this);
    panel->setStyleSheet("background-color: #141820;");

    QVBoxLayout* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(36, 36, 24, 36);
    layout->setSpacing(20);

    // UDRMS title top-left in spaced letters
    QLabel* topTitle = new QLabel("U D R M S", panel);
    topTitle->setStyleSheet(
        "color: #52c785; font-size: 28px; font-weight: 700;"
        "font-family: 'Segoe UI'; letter-spacing: 8px;"
        );

    // illustration inside a rounded card
    QWidget* card = new QWidget(panel);
    card->setStyleSheet(
        "background-color: #1a2030; border-radius: 14px;"
        "border: 1px solid #2a3040;"
        );
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(0, 0, 0, 0);

    IllustrationWidget* illus = new IllustrationWidget(card);
    illus->setMinimumHeight(320);
    cardLayout->addWidget(illus);

    layout->addWidget(topTitle);
    layout->addWidget(card, 1);

    return panel;
}

QWidget* WelcomeWindow::buildRightPanel() {
    QWidget* panel = new QWidget(this);
    panel->setFixedWidth(400);
    panel->setStyleSheet("background-color: #1a2030;");

    QVBoxLayout* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(40, 48, 40, 36);
    layout->setSpacing(0);

    // "Welcome to"
    QLabel* welcomeTo = new QLabel("Welcome to", panel);
    welcomeTo->setStyleSheet(
        "color: #8b909a; font-size: 14px; font-family: 'Segoe UI';"
        );

    // Big UDRMS
    QLabel* bigTitle = new QLabel("UDRMS", panel);
    bigTitle->setStyleSheet(
        "color: #52c785; font-size: 46px; font-weight: 700;"
        "font-family: 'Segoe UI'; margin-top: 2px;"
        );

    // Green underline divider
    QFrame* divider = new QFrame(panel);
    divider->setFixedSize(60, 3);
    divider->setStyleSheet("background-color: #52c785; border-radius: 2px;");

    // Description
    QLabel* desc = new QLabel(
        "A smart management platform for ENSIA's\n"
        "student dormitories and restaurant services.", panel);
    desc->setStyleSheet(
        "color: #b8bcc4; font-size: 13px; font-family: 'Segoe UI';"
        "line-height: 1.7; margin-top: 16px;"
        );
    desc->setWordWrap(true);

    // Feature bullets
    auto makeBullet = [&](const QString& text) {
        QLabel* lbl = new QLabel("✦  " + text, panel);
        lbl->setStyleSheet(
            "color: #52c785; font-size: 12px; font-family: 'Segoe UI';"
            );
        return lbl;
    };

    // Enter System button
    startButton = new QPushButton("Enter System  →", panel);
    startButton->setFixedHeight(48);
    startButton->setCursor(Qt::PointingHandCursor);
    startButton->setStyleSheet(
        "QPushButton { background-color: #2d6a4f; color: #ffffff; border: none;"
        "border-radius: 8px; font-size: 15px; font-weight: 700;"
        "font-family: 'Segoe UI'; letter-spacing: 0.3px; }"
        "QPushButton:hover { background-color: #36805f; }"
        "QPushButton:pressed { background-color: #245a42; }"
        );
    connect(startButton, &QPushButton::clicked, this, &WelcomeWindow::onGetStarted);

    layout->addWidget(welcomeTo);
    layout->addWidget(bigTitle);
    layout->addSpacing(10);
    layout->addWidget(divider);
    layout->addSpacing(16);
    layout->addWidget(desc);
    layout->addSpacing(20);
    layout->addWidget(makeBullet("Student registration & room assignment"));
    layout->addSpacing(6);
    layout->addWidget(makeBullet("Dormitory availability tracking"));
    layout->addSpacing(6);
    layout->addWidget(makeBullet("Restaurant meal management"));
    layout->addSpacing(6);
    layout->addWidget(makeBullet("University-wide reporting"));
    layout->addStretch(1);
    layout->addWidget(startButton);

    return panel;
}

void WelcomeWindow::onTick() { /* unused — kept for moc compatibility */ }

void WelcomeWindow::onGetStarted() {
    QPropertyAnimation* fadeOut = new QPropertyAnimation(this, "windowOpacity", this);
    fadeOut->setDuration(300);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    connect(fadeOut, &QPropertyAnimation::finished, this, [this]() {
        emit finished();
        this->close();
    });
    fadeOut->start();
}