#include "welcomepage.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QLinearGradient>
#include <QSvgRenderer>
#include <QFont>
#include <QFontDatabase>
#include <QScreen>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

// ──────────────────────────────────────────────────────────────
//  Inline SVG: dormitory night-scene (your UDRMS illustration)
// ──────────────────────────────────────────────────────────────
static const char* DORM_SVG = R"SVG(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 500 420">
  <!-- Night sky -->
  <rect width="500" height="420" fill="#0f1923"/>

  <!-- Stars -->
  <g fill="white" opacity="0.7">
    <circle cx="40"  cy="30"  r="1.2"/>
    <circle cx="90"  cy="18"  r="1"/>
    <circle cx="150" cy="40"  r="1.5"/>
    <circle cx="220" cy="15"  r="1"/>
    <circle cx="300" cy="25"  r="1.2"/>
    <circle cx="370" cy="12"  r="1"/>
    <circle cx="430" cy="35"  r="1.5"/>
    <circle cx="470" cy="20"  r="1"/>
    <circle cx="60"  cy="65"  r="1"/>
    <circle cx="480" cy="60"  r="1.2"/>
    <circle cx="250" cy="50"  r="0.8"/>
  </g>

  <!-- Moon -->
  <circle cx="440" cy="55" r="28" fill="#1e3a5f"/>
  <circle cx="452" cy="45" r="22" fill="#0f1923"/>

  <!-- Ground -->
  <rect x="0" y="340" width="500" height="80" fill="#0d1f2d"/>

  <!-- Trees left -->
  <ellipse cx="52" cy="290" rx="30" ry="40" fill="#1a4731"/>
  <rect    x="46" y="318"   width="12" height="30" fill="#3d2b1f"/>
  <ellipse cx="85" cy="305" rx="22" ry="30" fill="#1d5238"/>
  <rect    x="79" y="325"   width="10" height="22" fill="#3d2b1f"/>

  <!-- Trees right -->
  <ellipse cx="448" cy="290" rx="30" ry="40" fill="#1a4731"/>
  <rect    x="442" y="318"  width="12" height="30" fill="#3d2b1f"/>
  <ellipse cx="415" cy="305" rx="22" ry="30" fill="#1d5238"/>
  <rect    x="409" y="325"  width="10" height="22" fill="#3d2b1f"/>

  <!-- Main building body -->
  <rect x="110" y="160" width="280" height="185" fill="#162535"/>

  <!-- Roof -->
  <polygon points="100,160 250,80 400,160" fill="#1f7a5c"/>
  <!-- Chimney -->
  <rect x="240" y="88" width="20" height="40" fill="#243447"/>

  <!-- Roof ridge cap -->
  <rect x="100" y="155" width="300" height="10" fill="#1a6a4f"/>

  <!-- Left wing -->
  <rect x="55"  y="210" width="60" height="135" fill="#162535"/>
  <!-- Right wing -->
  <rect x="385" y="210" width="60" height="135" fill="#162535"/>

  <!-- Building base ledge -->
  <rect x="50"  y="338" width="400" height="8" fill="#0f2030"/>

  <!-- ── Windows – Main building ── -->
  <!-- Row 1 -->
  <rect x="148" y="178" width="42" height="32" rx="2" fill="#c8960a" opacity="0.85"/>
  <line x1="169" y1="178" x2="169" y2="210" stroke="#162535" stroke-width="2"/>
  <line x1="148" y1="194" x2="190" y2="194" stroke="#162535" stroke-width="2"/>

  <rect x="229" y="178" width="42" height="32" rx="2" fill="#c8960a" opacity="0.85"/>
  <line x1="250" y1="178" x2="250" y2="210" stroke="#162535" stroke-width="2"/>
  <line x1="229" y1="194" x2="271" y2="194" stroke="#162535" stroke-width="2"/>

  <rect x="310" y="178" width="42" height="32" rx="2" fill="#c8960a" opacity="0.85"/>
  <line x1="331" y1="178" x2="331" y2="210" stroke="#162535" stroke-width="2"/>
  <line x1="310" y1="194" x2="352" y2="194" stroke="#162535" stroke-width="2"/>

  <!-- Row 2 -->
  <rect x="148" y="228" width="42" height="32" rx="2" fill="#1a6a4f" opacity="0.9"/>
  <rect x="229" y="228" width="42" height="32" rx="2" fill="#c8960a" opacity="0.7"/>
  <rect x="310" y="228" width="42" height="32" rx="2" fill="#1a6a4f" opacity="0.9"/>

  <!-- Row 3 -->
  <rect x="148" y="278" width="42" height="32" rx="2" fill="#c8960a" opacity="0.6"/>
  <rect x="310" y="278" width="42" height="32" rx="2" fill="#c8960a" opacity="0.6"/>

  <!-- ── Windows – Left wing ── -->
  <rect x="65"  y="230" width="38" height="28" rx="2" fill="#c8960a" opacity="0.75"/>
  <rect x="65"  y="275" width="38" height="28" rx="2" fill="#1a6a4f" opacity="0.8"/>

  <!-- ── Windows – Right wing ── -->
  <rect x="397" y="230" width="38" height="28" rx="2" fill="#1a6a4f" opacity="0.8"/>
  <rect x="397" y="275" width="38" height="28" rx="2" fill="#c8960a" opacity="0.75"/>

  <!-- ── Entrance / Door ── -->
  <rect x="217" y="285" width="66" height="60" rx="3" fill="#0d1f2d"/>
  <rect x="223" y="290" width="24" height="55" rx="2" fill="#1a3a55"/>
  <rect x="253" y="290" width="24" height="55" rx="2" fill="#1a3a55"/>
  <!-- Door handles -->
  <circle cx="246" cy="320" r="3" fill="#c8960a"/>
  <circle cx="254" cy="320" r="3" fill="#c8960a"/>
  <!-- Door step -->
  <rect x="207" y="340" width="86" height="6" rx="1" fill="#0f2030"/>

  <!-- Dormitory label -->
  <rect x="190" y="270" width="120" height="18" rx="3" fill="#1f7a5c"/>
  <text x="250" y="283" text-anchor="middle" font-family="Arial" font-size="9"
        font-weight="bold" fill="white">DORMITORY A</text>

  <!-- ── Label tags ── -->
  <!-- Restaurant tag (left) -->
  <rect x="55" y="207" width="90" height="18" rx="3" fill="#e8a020"/>
  <text x="100" y="220" text-anchor="middle" font-family="Arial" font-size="8"
        font-weight="bold" fill="white">🍽 RESTAURANT</text>

  <!-- Study Room tag (right) -->
  <rect x="355" y="207" width="90" height="18" rx="3" fill="#2563a8"/>
  <text x="400" y="220" text-anchor="middle" font-family="Arial" font-size="8"
        font-weight="bold" fill="white">📚 STUDY ROOM</text>

  <!-- ── Students (stick figures) ── -->
  <!-- Student 1: left of entrance -->
  <circle cx="195" cy="295" r="10" fill="#4a90d9"/>
  <rect   x="189" cy="305" width="12" height="24" rx="2" fill="#2e6baa"/>
  <line   x1="195" y1="309" x2="183" y2="322" stroke="#2e6baa" stroke-width="3" stroke-linecap="round"/>
  <line   x1="195" y1="309" x2="207" y2="322" stroke="#2e6baa" stroke-width="3" stroke-linecap="round"/>
  <line   x1="195" y1="329" x2="190" y2="342" stroke="#2e6baa" stroke-width="3" stroke-linecap="round"/>
  <line   x1="195" y1="329" x2="200" y2="342" stroke="#2e6baa" stroke-width="3" stroke-linecap="round"/>
  <!-- Backpack -->
  <rect   x="205" y="308" width="9" height="13" rx="2" fill="#e8a020"/>

  <!-- Student 2: right of entrance -->
  <circle cx="305" cy="295" r="10" fill="#c0392b"/>
  <rect   x="299" cy="305" width="12" height="24" rx="2" fill="#922b21"/>
  <line   x1="305" y1="309" x2="293" y2="322" stroke="#922b21" stroke-width="3" stroke-linecap="round"/>
  <line   x1="305" y1="309" x2="317" y2="322" stroke="#922b21" stroke-width="3" stroke-linecap="round"/>
  <line   x1="305" y1="329" x2="300" y2="342" stroke="#922b21" stroke-width="3" stroke-linecap="round"/>
  <line   x1="305" y1="329" x2="310" y2="342" stroke="#922b21" stroke-width="3" stroke-linecap="round"/>
  <!-- Book -->
  <rect   x="286" y="312" width="9" height="12" rx="1" fill="#1f7a5c"/>

  <!-- Path / walkway -->
  <rect x="220" y="346" width="60" height="8"  fill="#1a3040" rx="1"/>
  <!-- Lamp posts -->
  <rect x="183" y="308" width="4" height="40" fill="#243447"/>
  <circle cx="185" cy="305" r="5" fill="#c8960a" opacity="0.9"/>
  <rect x="313" y="308" width="4" height="40" fill="#243447"/>
  <circle cx="315" cy="305" r="5" fill="#c8960a" opacity="0.9"/>
</svg>
)SVG";

// ──────────────────────────────────────────────────────────────

WelcomePage::WelcomePage(QWidget *parent)
    : QWidget(parent)
    , m_countdown(8)
{
    setObjectName("WelcomePage");
    setupUI();
    startAnimations();

    // Auto-advance timer (8 seconds)
    m_autoAdvanceTimer = new QTimer(this);
    m_autoAdvanceTimer->setSingleShot(true);
    m_autoAdvanceTimer->setInterval(8000);
    connect(m_autoAdvanceTimer, &QTimer::timeout,
            this, &WelcomePage::onAutoAdvance);
    m_autoAdvanceTimer->start();

    // Countdown display (every 1 second)
    m_countdownTimer = new QTimer(this);
    m_countdownTimer->setInterval(1000);
    connect(m_countdownTimer, &QTimer::timeout,
            this, &WelcomePage::updateCountdown);
    m_countdownTimer->start();
}

WelcomePage::~WelcomePage() {}

// ── UI Setup ──────────────────────────────────────────────────

void WelcomePage::setupUI()
{
    QHBoxLayout *root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    setupLeftPanel();
    setupRightPanel();

    root->addWidget(m_leftPanel,  5);   // 50% width
    root->addWidget(m_rightPanel, 5);   // 50% width
}

void WelcomePage::setupLeftPanel()
{
    m_leftPanel = new QWidget(this);
    m_leftPanel->setObjectName("LeftPanel");

    QVBoxLayout *lay = new QVBoxLayout(m_leftPanel);
    lay->setContentsMargins(30, 40, 30, 30);
    lay->setSpacing(12);

    // ── Logo / title ──
    m_logoLabel = new QLabel(this);
    m_logoLabel->setObjectName("LogoLabel");
    m_logoLabel->setText("UDRMS");
    m_logoLabel->setAlignment(Qt::AlignCenter);

    // ── SVG illustration ──
    m_illustrationLabel = new QLabel(this);
    m_illustrationLabel->setObjectName("IllustrationLabel");
    m_illustrationLabel->setMinimumSize(300, 260);

    // Render SVG to pixmap
    QSvgRenderer renderer(QByteArray(DORM_SVG));
    QPixmap px(400, 340);
    px.fill(Qt::transparent);
    QPainter p(&px);
    renderer.render(&p);
    p.end();
    m_illustrationLabel->setPixmap(px.scaled(
        m_illustrationLabel->minimumSize(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation));
    m_illustrationLabel->setAlignment(Qt::AlignCenter);

    // ── Tagline ──
    m_taglineLabel = new QLabel("University Dormitory & Restaurant\nManagement System", this);
    m_taglineLabel->setObjectName("TaglineLabel");
    m_taglineLabel->setAlignment(Qt::AlignCenter);
    m_taglineLabel->setWordWrap(true);

    lay->addWidget(m_logoLabel);
    lay->addStretch(1);
    lay->addWidget(m_illustrationLabel);
    lay->addStretch(1);
    lay->addWidget(m_taglineLabel);
}

void WelcomePage::setupRightPanel()
{
    m_rightPanel = new QWidget(this);
    m_rightPanel->setObjectName("RightPanel");

    // Fade-in effect for right panel
    m_rightPanelEffect = new QGraphicsOpacityEffect(m_rightPanel);
    m_rightPanelEffect->setOpacity(0.0);
    m_rightPanel->setGraphicsEffect(m_rightPanelEffect);

    QVBoxLayout *lay = new QVBoxLayout(m_rightPanel);
    lay->setContentsMargins(50, 60, 50, 40);
    lay->setSpacing(0);

    // Greeting
    m_greetingLabel = new QLabel("Welcome to", m_rightPanel);
    m_greetingLabel->setObjectName("GreetingLabel");
    m_greetingLabel->setAlignment(Qt::AlignLeft);

    // System name
    m_systemNameLabel = new QLabel("UDRMS", m_rightPanel);
    m_systemNameLabel->setObjectName("SystemNameLabel");
    m_systemNameLabel->setAlignment(Qt::AlignLeft);

    // Divider
    QFrame *divider = new QFrame(m_rightPanel);
    divider->setObjectName("Divider");
    divider->setFixedHeight(3);
    divider->setFrameShape(QFrame::HLine);

    // Description
    m_descriptionLabel = new QLabel(
        "A smart management platform for ENSIA's student\n"
        "dormitories and restaurant services.",
        m_rightPanel);
    m_descriptionLabel->setObjectName("DescriptionLabel");
    m_descriptionLabel->setWordWrap(true);
    m_descriptionLabel->setAlignment(Qt::AlignLeft);

    // Features list
    m_featuresLabel = new QLabel(
        "✦  Student registration & room assignment\n"
        "✦  Dormitory availability tracking\n"
        "✦  Restaurant meal management\n"
        "✦  University-wide reporting",
        m_rightPanel);
    m_featuresLabel->setObjectName("FeaturesLabel");
    m_featuresLabel->setWordWrap(true);

    // Enter button
    m_enterBtn = new QPushButton("Enter System  →", m_rightPanel);
    m_enterBtn->setObjectName("EnterButton");
    m_enterBtn->setCursor(Qt::PointingHandCursor);
    connect(m_enterBtn, &QPushButton::clicked,
            this, &WelcomePage::onEnterClicked);

    // Countdown
    m_countdownLabel = new QLabel(QString("Auto-entering in %1 s…").arg(m_countdown),
                                  m_rightPanel);
    m_countdownLabel->setObjectName("CountdownLabel");
    m_countdownLabel->setAlignment(Qt::AlignCenter);

    // Version
    m_versionLabel = new QLabel("ENSIA  ·  2025–2026  ·  v1.0", m_rightPanel);
    m_versionLabel->setObjectName("VersionLabel");
    m_versionLabel->setAlignment(Qt::AlignCenter);

    lay->addWidget(m_greetingLabel);
    lay->addSpacing(4);
    lay->addWidget(m_systemNameLabel);
    lay->addSpacing(16);
    lay->addWidget(divider);
    lay->addSpacing(20);
    lay->addWidget(m_descriptionLabel);
    lay->addSpacing(24);
    lay->addWidget(m_featuresLabel);
    lay->addStretch(1);
    lay->addWidget(m_enterBtn);
    lay->addSpacing(10);
    lay->addWidget(m_countdownLabel);
    lay->addSpacing(14);
    lay->addWidget(m_versionLabel);
}

// ── Animations ────────────────────────────────────────────────

void WelcomePage::startAnimations()
{
    m_fadeInAnim = new QPropertyAnimation(m_rightPanelEffect, "opacity", this);
    m_fadeInAnim->setDuration(900);
    m_fadeInAnim->setStartValue(0.0);
    m_fadeInAnim->setEndValue(1.0);
    m_fadeInAnim->setEasingCurve(QEasingCurve::InOutQuad);
    m_fadeInAnim->start();
}

// ── Paint ─────────────────────────────────────────────────────

void WelcomePage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    // Subtle diagonal separator between panels (painted on top of layouts)
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    int mid = width() / 2;
    p.setPen(QPen(QColor(0x1f, 0x7a, 0x5c, 180), 2));
    p.drawLine(mid, 0, mid, height());
}

// ── Slots ─────────────────────────────────────────────────────

void WelcomePage::onEnterClicked()
{
    m_autoAdvanceTimer->stop();
    m_countdownTimer->stop();
    emit enterRequested();
}

void WelcomePage::onAutoAdvance()
{
    m_countdownTimer->stop();
    emit enterRequested();
}

void WelcomePage::updateCountdown()
{
    --m_countdown;
    if (m_countdown > 0) {
        m_countdownLabel->setText(
            QString("Auto-entering in %1 s…").arg(m_countdown));
    } else {
        m_countdownLabel->setText("Loading…");
    }
}
