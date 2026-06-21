#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QGroupBox>
#include <QCloseEvent>
#include <QDate>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
MainWindow::MainWindow(University& university, QWidget* parent)
    : QMainWindow(parent), university(university) {
    setupUI();
}

MainWindow::~MainWindow() {
}

void MainWindow::closeEvent(QCloseEvent* event) {
    FileManager::saveData(university, "udrms_data.txt");
    event->accept();
}
void MainWindow::setupUI() {
    setWindowTitle("UDRMS — University Dormitory & Restaurant Management");
    resize(960, 600);

    QWidget* central = new QWidget(this);
    QHBoxLayout* rootLayout = new QHBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    stack = new QStackedWidget(this);
    stack->addWidget(buildDashboardPage());    // index 0
    stack->addWidget(buildStudentsPage());     // index 1
    stack->addWidget(buildDormitoriesPage());  // index 2
    stack->addWidget(buildRestaurantPage());   // index 3
    stack->addWidget(buildMessagesPage());     // index 4

    rootLayout->addWidget(buildSidebar());
    rootLayout->addWidget(stack, 1);

    setCentralWidget(central);

    refreshAllCombos();
    setActiveNavButton(navDashboardBtn);
    stack->setCurrentIndex(0);
    refreshDashboard();
}

// ════════════════════════════════════════════════════════
// SIDEBAR
// ════════════════════════════════════════════════════════
QWidget* MainWindow::buildSidebar() {
    QWidget* sidebar = new QWidget(this);
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(200);

    QVBoxLayout* layout = new QVBoxLayout(sidebar);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    // Logo widget
    QWidget* logoWidget = new QWidget(sidebar);
    logoWidget->setStyleSheet("background:transparent;");
    QHBoxLayout* logoRow = new QHBoxLayout(logoWidget);
    logoRow->setContentsMargins(14, 16, 14, 8);
    logoRow->setSpacing(10);

    QLabel* logoIcon = new QLabel("🏛", sidebar);
    logoIcon->setFixedSize(36, 36);
    logoIcon->setAlignment(Qt::AlignCenter);
    logoIcon->setStyleSheet(
        "background-color:#2d6a4f;border-radius:8px;font-size:20px;");

    QVBoxLayout* logoText = new QVBoxLayout();
    logoText->setSpacing(1);
    QLabel* logoTitle = new QLabel("UDRMS", sidebar);
    logoTitle->setStyleSheet(
        "color:#ffffff;font-size:15px;font-weight:700;font-family:'Segoe UI';");
    QLabel* logoSub = new QLabel("ENSIA 2025–2026", sidebar);
    logoSub->setStyleSheet(
        "color:#52c785;font-size:9px;font-family:'Segoe UI';letter-spacing:0.3px;");
    logoText->addWidget(logoTitle);
    logoText->addWidget(logoSub);

    logoRow->addWidget(logoIcon);
    logoRow->addLayout(logoText);

    QFrame* logoDivider = new QFrame(sidebar);
    logoDivider->setFixedHeight(1);
    logoDivider->setStyleSheet("background-color:#2a2f38;");

    navDashboardBtn   = new QPushButton("  Dashboard",   sidebar);
    navStudentsBtn    = new QPushButton("  Students",    sidebar);
    navDormitoriesBtn = new QPushButton("  Dormitories", sidebar);
    navRestaurantBtn  = new QPushButton("  Restaurant",  sidebar);
    navMessagesBtn    = new QPushButton("  Messages",    sidebar);

    for (QPushButton* btn : {navDashboardBtn, navStudentsBtn,
                             navDormitoriesBtn, navRestaurantBtn,
                             navMessagesBtn}) {
        btn->setProperty("class", "navButton");
        btn->setCheckable(true);
        btn->setCursor(Qt::PointingHandCursor);
    }

    layout->addWidget(logoWidget);
    layout->addWidget(logoDivider);
    layout->addWidget(navDashboardBtn);
    layout->addWidget(navStudentsBtn);
    layout->addWidget(navDormitoriesBtn);
    layout->addWidget(navRestaurantBtn);
    layout->addWidget(navMessagesBtn);
    layout->addStretch(1);

    // Logout button at the bottom
    QPushButton* logoutBtn = new QPushButton("  Logout", sidebar);
    logoutBtn->setCursor(Qt::PointingHandCursor);
    logoutBtn->setStyleSheet(
        "QPushButton { background-color:transparent; color:#e05555; border:none;"
        "text-align:left; padding:12px 20px; font-size:14px; font-family:'Segoe UI'; }"
        "QPushButton:hover { background-color:#2d1a1a; color:#ff6b6b; }");
    layout->addWidget(logoutBtn);
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    connect(navDashboardBtn,   &QPushButton::clicked, this, &MainWindow::showDashboardPage);
    connect(navStudentsBtn,    &QPushButton::clicked, this, &MainWindow::showStudentsPage);
    connect(navDormitoriesBtn, &QPushButton::clicked, this, &MainWindow::showDormitoriesPage);
    connect(navRestaurantBtn,  &QPushButton::clicked, this, &MainWindow::showRestaurantPage);
    connect(navMessagesBtn, &QPushButton::clicked, this, &MainWindow::showMessagesPage);
    return sidebar;
}

void MainWindow::setActiveNavButton(QPushButton* active) {
    for (QPushButton* btn : {navDashboardBtn, navStudentsBtn,
                             navDormitoriesBtn, navRestaurantBtn,
                             navMessagesBtn})
        btn->setChecked(btn == active);
}

void MainWindow::showDashboardPage() {
    stack->setCurrentIndex(0);
    setActiveNavButton(navDashboardBtn);
    refreshDashboard();
}

void MainWindow::showStudentsPage() {
    stack->setCurrentIndex(1);
    setActiveNavButton(navStudentsBtn);
    refreshAllCombos();
    refreshStudentList();
}

void MainWindow::showDormitoriesPage() {
    stack->setCurrentIndex(2);
    setActiveNavButton(navDormitoriesBtn);
    refreshAllCombos();
    refreshDormitoryList();
}

void MainWindow::showRestaurantPage() {
    stack->setCurrentIndex(3);
    setActiveNavButton(navRestaurantBtn);
    refreshAllCombos();
}

// ════════════════════════════════════════════════════════
// DASHBOARD PAGE
// ════════════════════════════════════════════════════════
QWidget* MainWindow::buildStatCard(const QString& label, QLabel** out) {
    QWidget* card = new QWidget(this);
    card->setProperty("class", "card");
    card->setFixedHeight(90);

    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setContentsMargins(16, 14, 16, 14);

    QLabel* lbl = new QLabel(label, card);
    lbl->setProperty("class", "cardLabel");

    QLabel* val = new QLabel("0", card);
    val->setProperty("class", "cardValue");

    layout->addWidget(lbl);
    layout->addWidget(val);
    layout->addStretch(1);

    *out = val;
    return card;
}

QWidget* MainWindow::buildDashboardPage() {
    QWidget* page = new QWidget(this);
    page->setObjectName("contentArea");

    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->setSpacing(0);

    // ── Header row: title + date ──────────────────────────
    QHBoxLayout* headerRow = new QHBoxLayout();

    QVBoxLayout* titleCol = new QVBoxLayout();
    QLabel* title = new QLabel("Dashboard", page);
    title->setProperty("class", "pageTitle");
    QLabel* subtitle = new QLabel("Overview of dormitory operations", page);
    subtitle->setProperty("class", "pageSubtitle");
    titleCol->addWidget(title);
    titleCol->addWidget(subtitle);
    titleCol->setSpacing(2);

    QVBoxLayout* dateCol = new QVBoxLayout();
    dateCol->setAlignment(Qt::AlignRight | Qt::AlignTop);
    QString dateStr = QDate::currentDate().toString("dddd, MMMM d yyyy");
    QLabel* dateLabel = new QLabel("📅  " + dateStr, page);
    dateLabel->setStyleSheet(
        "color: #52c785; font-size: 12px; font-weight: 600; font-family: 'Segoe UI';");
    QLabel* yearLabel = new QLabel("Academic Year 2025 – 2026", page);
    yearLabel->setStyleSheet(
        "color: #8b909a; font-size: 11px; font-family: 'Segoe UI';");
    yearLabel->setAlignment(Qt::AlignRight);
    dateCol->addWidget(dateLabel);
    dateCol->addWidget(yearLabel);

    headerRow->addLayout(titleCol);
    headerRow->addStretch(1);
    headerRow->addLayout(dateCol);

    // ── Divider ───────────────────────────────────────────
    QFrame* div = new QFrame(page);
    div->setFixedHeight(1);
    div->setStyleSheet("background-color: #2a2f38;");

    // ── Stat cards row ────────────────────────────────────
    QHBoxLayout* cardsRow = new QHBoxLayout();
    cardsRow->setSpacing(12);

    auto makeCard = [&](const QString& label, QLabel** out,
                        const QString& icon, const QString& iconColor,
                        const QString& iconBg) -> QWidget* {
        QWidget* card = new QWidget(page);
        card->setStyleSheet(
            "QWidget { background-color: #262b33; border-radius: 10px;"
            "border: 1px solid #313742; }");
        card->setFixedHeight(90);

        QHBoxLayout* row = new QHBoxLayout(card);
        row->setContentsMargins(14, 12, 14, 12);

        QVBoxLayout* textCol = new QVBoxLayout();
        QLabel* lbl = new QLabel(label, card);
        lbl->setStyleSheet("color:#8b909a;font-size:11px;font-family:'Segoe UI';"
                           "border:none;background:transparent;");
        QLabel* val = new QLabel("0", card);
        val->setStyleSheet(
            QString("color:#ffffff;font-size:26px;font-weight:700;"
                    "font-family:'Segoe UI';border:none;background:transparent;"));
        textCol->addWidget(lbl);
        textCol->addWidget(val);
        textCol->setSpacing(4);

        QLabel* iconLbl = new QLabel(icon, card);
        iconLbl->setFixedSize(38, 38);
        iconLbl->setAlignment(Qt::AlignCenter);
        iconLbl->setStyleSheet(
            QString("background-color:%1;border-radius:8px;font-size:20px;"
                    "border:none;").arg(iconBg));

        row->addLayout(textCol);
        row->addStretch(1);
        row->addWidget(iconLbl);

        *out = val;
        return card;
    };

    cardsRow->addWidget(makeCard("Total Students",  &totalStudentsValueLabel,
                                 "🎓", "#52c785", "#1a3d2e"));
    cardsRow->addWidget(makeCard("Dormitories",     &totalDormitoriesValueLabel,
                                 "🏠", "#2a9fc9", "#0f2d38"));
    cardsRow->addWidget(makeCard("Total Rooms",     &totalRoomsValueLabel,
                                 "🚪", "#e09e3a", "#2a2010"));
    cardsRow->addWidget(makeCard("Available Rooms", &availableRoomsValueLabel,
                                 "✅", "#52c785", "#1a3d2e"));

    // ── Bottom row: occupancy bars + system info ──────────
    QHBoxLayout* bottomRow = new QHBoxLayout();
    bottomRow->setSpacing(14);

    // Occupancy per dormitory panel
    occupancyWidget = new QWidget(page);
    occupancyWidget->setStyleSheet(
        "QWidget { background-color: #262b33; border-radius: 10px;"
        "border: 1px solid #313742; }");
    occupancyLayout = new QVBoxLayout(occupancyWidget);
    occupancyLayout->setContentsMargins(16, 14, 16, 14);
    occupancyLayout->setSpacing(10);

    QLabel* occTitle = new QLabel("🏢  Occupancy per Dormitory", occupancyWidget);
    occTitle->setStyleSheet("color:#ffffff;font-size:13px;font-weight:600;"
                            "font-family:'Segoe UI';border:none;background:transparent;");
    occupancyLayout->addWidget(occTitle);

    // System info panel
    QWidget* infoPanel = new QWidget(page);
    infoPanel->setStyleSheet(
        "QWidget { background-color: #262b33; border-radius: 10px;"
        "border: 1px solid #313742; }");
    QVBoxLayout* infoLayout = new QVBoxLayout(infoPanel);
    infoLayout->setContentsMargins(16, 14, 16, 14);
    infoLayout->setSpacing(8);

    QLabel* infoTitle = new QLabel("ℹ️  System Information", infoPanel);
    infoTitle->setStyleSheet("color:#ffffff;font-size:13px;font-weight:600;"
                             "font-family:'Segoe UI';border:none;background:transparent;");
    infoLayout->addWidget(infoTitle);

    auto makeInfoRow = [&](const QString& lbl, const QString& val,
                           const QString& valColor = "#ffffff") {
        QWidget* row = new QWidget(infoPanel);
        row->setStyleSheet("background-color:#1e2228;border-radius:6px;border:none;");
        QHBoxLayout* r = new QHBoxLayout(row);
        r->setContentsMargins(10, 7, 10, 7);
        QLabel* l = new QLabel(lbl, row);
        l->setStyleSheet("color:#8b909a;font-size:12px;font-family:'Segoe UI';"
                         "border:none;background:transparent;");
        QLabel* v = new QLabel(val, row);
        v->setStyleSheet(QString("color:%1;font-size:12px;font-weight:600;"
                                 "font-family:'Segoe UI';border:none;background:transparent;")
                             .arg(valColor));
        r->addWidget(l);
        r->addStretch(1);
        r->addWidget(v);
        infoLayout->addWidget(row);
    };

    makeInfoRow("University",     "ENSIA");
    makeInfoRow("Academic Year",  "2025 – 2026");
    makeInfoRow("System Version", "UDRMS v1.0");
    occupancyRateValueLabel = new QLabel("0%", infoPanel);
    // we'll add a dynamic occupancy rate row
    QWidget* rateRow = new QWidget(infoPanel);
    rateRow->setStyleSheet("background-color:#1e2228;border-radius:6px;border:none;");
    QHBoxLayout* rr = new QHBoxLayout(rateRow);
    rr->setContentsMargins(10, 7, 10, 7);
    QLabel* rl = new QLabel("Overall Occupancy", rateRow);
    rl->setStyleSheet("color:#8b909a;font-size:12px;font-family:'Segoe UI';"
                      "border:none;background:transparent;");
    occupancyRateValueLabel->setStyleSheet(
        "color:#52c785;font-size:12px;font-weight:600;"
        "font-family:'Segoe UI';border:none;background:transparent;");
    rr->addWidget(rl);
    rr->addStretch(1);
    rr->addWidget(occupancyRateValueLabel);
    infoLayout->addWidget(rateRow);
    infoLayout->addStretch(1);

    bottomRow->addWidget(occupancyWidget, 1);
    bottomRow->addWidget(infoPanel, 1);

    layout->addLayout(headerRow);
    layout->addSpacing(10);
    layout->addWidget(div);
    layout->addSpacing(14);
    layout->addLayout(cardsRow);
    layout->addSpacing(14);
    layout->addLayout(bottomRow);
    layout->addStretch(1);

    return page;
}

void MainWindow::refreshDashboard() {
    int totalRooms = 0, availableRooms = 0, totalCapacity = 0;
    int totalOccupancy = 0;

    for (Dormitory* d : university.getDormitories()) {
        totalCapacity  += d->getCapacity();
        totalOccupancy += d->getTotalOccupancy();
        for (Room* r : d->getRooms()) {
            totalRooms++;
            if (!r->isFull()) availableRooms++;
        }
    }

    totalStudentsValueLabel   ->setText(QString::number(university.getTotalStudents()));
    totalDormitoriesValueLabel->setText(QString::number(university.getDormitories().size()));
    totalRoomsValueLabel      ->setText(QString::number(totalRooms));
    availableRoomsValueLabel  ->setText(QString::number(availableRooms));

    int rate = totalCapacity > 0 ? (totalOccupancy * 100 / totalCapacity) : 0;
    occupancyRateValueLabel->setText(QString::number(rate) + "%");

    // Rebuild occupancy bars
    // Remove old bars (keep the title label at index 0)
    while (occupancyLayout->count() > 1) {
        QLayoutItem* item = occupancyLayout->takeAt(1);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    if (university.getDormitories().empty()) {
        QLabel* none = new QLabel("No dormitories added yet.", occupancyWidget);
        none->setStyleSheet("color:#8b909a;font-size:12px;font-family:'Segoe UI';");
        occupancyLayout->addWidget(none);
    }

    for (Dormitory* d : university.getDormitories()) {
        int cap  = d->getCapacity();
        int occ  = d->getTotalOccupancy();
        int pct  = cap > 0 ? (occ * 100 / cap) : 0;

        // Color based on how full it is
        QString barColor = pct < 50 ? "#52c785" : pct < 85 ? "#e09e3a" : "#e05555";

        QWidget* row = new QWidget(occupancyWidget);
        row->setStyleSheet("border:none;background:transparent;");
        QVBoxLayout* rowL = new QVBoxLayout(row);
        rowL->setContentsMargins(0,0,0,0);
        rowL->setSpacing(4);

        QHBoxLayout* labelRow = new QHBoxLayout();
        QLabel* nameL = new QLabel(QString::fromStdString(d->getName()), row);
        nameL->setStyleSheet("color:#b8bcc4;font-size:12px;font-family:'Segoe UI';"
                             "background:transparent;border:none;");
        QLabel* countL = new QLabel(
            QString("%1 / %2").arg(occ).arg(cap), row);
        countL->setStyleSheet(
            QString("color:%1;font-size:12px;font-weight:600;"
                    "font-family:'Segoe UI';background:transparent;border:none;")
                .arg(barColor));
        labelRow->addWidget(nameL);
        labelRow->addStretch(1);
        labelRow->addWidget(countL);

        // Progress bar
        QWidget* track = new QWidget(row);
        track->setFixedHeight(8);
        track->setStyleSheet("background-color:#1e2228;border-radius:4px;border:none;");
        QWidget* fill = new QWidget(track);
        fill->setStyleSheet(
            QString("background-color:%1;border-radius:4px;").arg(barColor));
        fill->setGeometry(0, 0, track->width() * pct / 100, 8);
        // connect resize to update bar
        QObject::connect(track, &QWidget::destroyed, fill, &QWidget::deleteLater);

        // use fixed width approximation for now
        int barW = qMax(1, pct * 2); // rough pixel estimate for 200px track
        fill->setFixedSize(qMin(barW, 200), 8);
        track->setFixedWidth(200);

        rowL->addLayout(labelRow);
        rowL->addWidget(track);
        occupancyLayout->addWidget(row);
    }
    occupancyLayout->addStretch(1);
}

// ════════════════════════════════════════════════════════
// STUDENTS PAGE
// ════════════════════════════════════════════════════════
QWidget* MainWindow::buildStudentsPage() {
    QWidget* page = new QWidget(this);
    page->setObjectName("contentArea");

    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->setSpacing(14);

    QLabel* title = new QLabel("Students", page);
    title->setProperty("class", "pageTitle");

    QLabel* subtitle = new QLabel("Enroll students into dormitory rooms", page);
    subtitle->setProperty("class", "pageSubtitle");

    idInput   = new QLineEdit(page); idInput->setPlaceholderText("e.g. S001");
    nameInput = new QLineEdit(page); nameInput->setPlaceholderText("e.g. Racha Brahimi");
    yearInput = new QLineEdit(page); yearInput->setPlaceholderText("1 – 5");
    dormSelectCombo = new QComboBox(page);
    roomSelectCombo = new QComboBox(page);

    QFormLayout* form = new QFormLayout();
    form->setSpacing(10);
    form->addRow("Student ID:",    idInput);
    form->addRow("Full Name:",     nameInput);
    form->addRow("Academic Year:", yearInput);
    form->addRow("Dormitory:",     dormSelectCombo);
    form->addRow("Room:",          roomSelectCombo);

    QPushButton* addBtn = new QPushButton("Enroll Student", page);
    addBtn->setProperty("class", "primaryButton");
    addBtn->setCursor(Qt::PointingHandCursor);

    removeStudentBtn = new QPushButton("Remove Selected Student", page);
    removeStudentBtn->setProperty("class", "primaryButton");
    removeStudentBtn->setCursor(Qt::PointingHandCursor);
    removeStudentBtn->setEnabled(false);   // disabled until a student is selected
    removeStudentBtn->setStyleSheet(
        "QPushButton { background-color: #6b2d2d; color: #ffffff; border: none;"
        "border-radius: 6px; padding: 10px 16px; font-weight: 600; }"
        "QPushButton:hover { background-color: #8b3a3a; }"
        "QPushButton:disabled { background-color: #2a2f38; color: #555; }"
        );

    studentStatusLabel = new QLabel("", page);
    studentListWidget  = new QListWidget(page);

    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addLayout(form);
    layout->addWidget(addBtn);
    layout->addWidget(removeStudentBtn);
    layout->addWidget(studentStatusLabel);
    layout->addWidget(new QLabel("Enrolled Students:", page));
    layout->addWidget(studentListWidget, 1);

    connect(addBtn, &QPushButton::clicked,
            this, &MainWindow::onAddStudentClicked);
    connect(removeStudentBtn, &QPushButton::clicked,
            this, &MainWindow::onRemoveStudentClicked);
    connect(dormSelectCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onDormitorySelectionChanged);
    connect(studentListWidget, &QListWidget::itemSelectionChanged, this, [this]() {
        removeStudentBtn->setEnabled(!studentListWidget->selectedItems().isEmpty());
    });
    return page;
}

void MainWindow::onDormitorySelectionChanged() {
    roomSelectCombo->clear();
    QString dormName = dormSelectCombo->currentText();
    if (dormName.isEmpty()) return;

    Dormitory* dorm = university.findDormitory(dormName.toStdString());
    if (!dorm) return;

    for (Room* r : dorm->getRooms()) {
        if (!r->isFull()) {
            roomSelectCombo->addItem(
                QString("Room %1  (%2/%3)")
                    .arg(r->getRoomNumber())
                    .arg(r->getOccupancy())
                    .arg(r->getCapacity()),
                r->getRoomNumber()
                );
        }
    }
}

void MainWindow::onAddStudentClicked() {
    QString id       = idInput->text().trimmed();
    QString name     = nameInput->text().trimmed();
    QString yearText = yearInput->text().trimmed();
    QString dormName = dormSelectCombo->currentText();
    int roomNumber   = roomSelectCombo->currentData().toInt();

    if (id.isEmpty() || name.isEmpty() || yearText.isEmpty() || dormName.isEmpty()) {
        QMessageBox::warning(this, "Missing Info", "Please fill in all fields.");
        return;
    }

    bool ok = false;
    int year = yearText.toInt(&ok);
    if (!ok || year < 1 || year > 5) {
        QMessageBox::warning(this, "Invalid Year", "Year must be between 1 and 5.");
        return;
    }

    if (university.findStudent(id.toStdString())) {
        QMessageBox::warning(this, "Duplicate ID", "A student with this ID already exists.");
        return;
    }

    bool enrolled = university.enrollStudent(
        id.toStdString(), name.toStdString(), year,
        dormName.toStdString(), roomNumber
        );

    if (enrolled) {
        studentStatusLabel->setText("Student enrolled successfully.");
        idInput->clear(); nameInput->clear(); yearInput->clear();
        refreshStudentList();
        refreshDashboard();
        refreshAllCombos();
    } else {
        studentStatusLabel->setText("Enrollment failed — room may be full.");
    }
}
void MainWindow::onRemoveStudentClicked() {
    QListWidgetItem* selected = studentListWidget->currentItem();
    if (!selected) return;

    QString text = selected->text();

    // Extract student ID from the list item text — format is "[S001]  Name..."
    // Find the ID between the first [ and ]
    int start = text.indexOf('[') + 1;
    int end   = text.indexOf(']');
    if (start <= 0 || end <= 0) return;

    QString studentId = text.mid(start, end - start).trimmed();

    // Ask for confirmation before deleting
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Remove Student",
        QString("Are you sure you want to remove student %1?").arg(studentId),
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply != QMessageBox::Yes) return;

    // Find which dormitory this student is in, then remove
    bool removed = false;
    for (Dormitory* d : university.getDormitories()) {
        if (d->findStudent(studentId.toStdString())) {
            removed = d->removeStudent(studentId.toStdString());
            break;
        }
    }

    if (removed) {
        studentStatusLabel->setText("Student removed successfully.");
        refreshStudentList();
        refreshDashboard();
        refreshAllCombos();
        removeStudentBtn->setEnabled(false);
        FileManager::saveData(university, "udrms_data.txt");
    } else {
        studentStatusLabel->setText("Could not remove student.");
    }
}
void MainWindow::refreshStudentList() {
    studentListWidget->clear();
    for (Dormitory* d : university.getDormitories()) {
        for (Student* s : d->getStudents()) {
            studentListWidget->addItem(
                QString("[%1]  %2  —  Year %3  —  %4 / Room %5")
                    .arg(QString::fromStdString(s->getId()))
                    .arg(QString::fromStdString(s->getFullName()))
                    .arg(s->getAcademicYear())
                    .arg(QString::fromStdString(s->getDormitoryName()))
                    .arg(s->getRoomNumber())
                );
        }
    }
}

// ════════════════════════════════════════════════════════
// DORMITORIES PAGE
// ════════════════════════════════════════════════════════
QWidget* MainWindow::buildDormitoriesPage() {
    QWidget* page = new QWidget(this);
    page->setObjectName("contentArea");

    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->setSpacing(14);

    QLabel* title = new QLabel("Dormitories", page);
    title->setProperty("class", "pageTitle");

    QLabel* subtitle = new QLabel("Add and manage dormitories and rooms", page);
    subtitle->setProperty("class", "pageSubtitle");

    dormNameInput     = new QLineEdit(page); dormNameInput->setPlaceholderText("e.g. Dormitory B");
    dormCapacityInput = new QLineEdit(page); dormCapacityInput->setPlaceholderText("e.g. 40");

    QFormLayout* dormForm = new QFormLayout();
    dormForm->setSpacing(10);
    dormForm->addRow("Dormitory Name:", dormNameInput);
    dormForm->addRow("Total Capacity:", dormCapacityInput);

    QPushButton* addDormBtn = new QPushButton("Add Dormitory", page);
    addDormBtn->setProperty("class", "primaryButton");
    addDormBtn->setCursor(Qt::PointingHandCursor);

    QLabel* roomSectionLabel = new QLabel("Add Room to Dormitory", page);
    roomSectionLabel->setProperty("class", "pageSubtitle");

    dormForRoomCombo  = new QComboBox(page);
    roomNumberInput   = new QLineEdit(page); roomNumberInput->setPlaceholderText("e.g. 201");
    roomCapacityInput = new QLineEdit(page); roomCapacityInput->setPlaceholderText("e.g. 2");

    QFormLayout* roomForm = new QFormLayout();
    roomForm->setSpacing(10);
    roomForm->addRow("Dormitory:",     dormForRoomCombo);
    roomForm->addRow("Room Number:",   roomNumberInput);
    roomForm->addRow("Room Capacity:", roomCapacityInput);

    QPushButton* addRoomBtn = new QPushButton("Add Room", page);
    addRoomBtn->setProperty("class", "primaryButton");
    addRoomBtn->setCursor(Qt::PointingHandCursor);

    dormStatusLabel = new QLabel("", page);
    dormListWidget  = new QListWidget(page);

    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addLayout(dormForm);
    layout->addWidget(addDormBtn);
    layout->addWidget(roomSectionLabel);
    layout->addLayout(roomForm);
    layout->addWidget(addRoomBtn);
    layout->addWidget(dormStatusLabel);
    layout->addWidget(new QLabel("Dormitories:", page));
    layout->addWidget(dormListWidget, 1);

    connect(addDormBtn, &QPushButton::clicked, this, &MainWindow::onAddDormitoryClicked);
    connect(addRoomBtn, &QPushButton::clicked, this, &MainWindow::onAddRoomClicked);

    return page;
}

void MainWindow::onAddDormitoryClicked() {
    QString name    = dormNameInput->text().trimmed();
    QString capText = dormCapacityInput->text().trimmed();

    if (name.isEmpty() || capText.isEmpty()) {
        QMessageBox::warning(this, "Missing Info", "Please fill in all fields.");
        return;
    }
    bool ok = false;
    int cap = capText.toInt(&ok);
    if (!ok || cap < 1) {
        QMessageBox::warning(this, "Invalid Capacity", "Capacity must be a positive number.");
        return;
    }
    if (university.findDormitory(name.toStdString())) {
        QMessageBox::warning(this, "Duplicate", "A dormitory with this name already exists.");
        return;
    }

    university.addDormitory(name.toStdString(), cap);
    dormStatusLabel->setText("Dormitory added successfully.");
    dormNameInput->clear(); dormCapacityInput->clear();
    refreshDormitoryList();
    refreshDashboard();
    refreshAllCombos();
}

void MainWindow::onAddRoomClicked() {
    QString dormName = dormForRoomCombo->currentText();
    QString numText  = roomNumberInput->text().trimmed();
    QString capText  = roomCapacityInput->text().trimmed();

    if (dormName.isEmpty() || numText.isEmpty() || capText.isEmpty()) {
        QMessageBox::warning(this, "Missing Info", "Please fill in all fields.");
        return;
    }
    bool ok1 = false, ok2 = false;
    int num = numText.toInt(&ok1);
    int cap = capText.toInt(&ok2);
    if (!ok1 || !ok2 || num < 1 || cap < 1) {
        QMessageBox::warning(this, "Invalid Input", "Room number and capacity must be positive numbers.");
        return;
    }

    Dormitory* dorm = university.findDormitory(dormName.toStdString());
    if (!dorm) return;

    dorm->addRoom(num, cap);
    dormStatusLabel->setText("Room added successfully.");
    roomNumberInput->clear(); roomCapacityInput->clear();
    refreshDormitoryList();
    refreshDashboard();
    refreshAllCombos();
}

void MainWindow::refreshDormitoryList() {
    dormListWidget->clear();
    for (Dormitory* d : university.getDormitories()) {
        dormListWidget->addItem(
            QString("%1  —  Capacity: %2  —  Occupancy: %3  —  Rooms: %4")
                .arg(QString::fromStdString(d->getName()))
                .arg(d->getCapacity())
                .arg(d->getTotalOccupancy())
                .arg(d->getRooms().size())
            );
    }
}

// ════════════════════════════════════════════════════════
// RESTAURANT PAGE
// ════════════════════════════════════════════════════════
QWidget* MainWindow::buildRestaurantPage() {
    QWidget* page = new QWidget(this);
    page->setObjectName("contentArea");

    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->setSpacing(14);

    QLabel* title = new QLabel("Restaurant", page);
    title->setProperty("class", "pageTitle");

    QLabel* subtitle = new QLabel("Set daily meals for each dormitory restaurant", page);
    subtitle->setProperty("class", "pageSubtitle");

    // ── Dorm selector row ──
    QHBoxLayout* selectorRow = new QHBoxLayout();
    QLabel* selectLabel = new QLabel("Dormitory:", page);
    restaurantDormCombo = new QComboBox(page);
    restaurantDormCombo->setFixedWidth(200);
    selectorRow->addWidget(selectLabel);
    selectorRow->addWidget(restaurantDormCombo);
    selectorRow->addStretch(1);

    restaurantInfoLabel = new QLabel("", page);

    // ── Helper to build one meal QGroupBox ──
    auto makeMealGroup = [&](const QString& mealName,
                             QLineEdit** main, QLineEdit** salad,
                             QLineEdit** fruit, QLineEdit** juice,
                             QLineEdit** soup) -> QGroupBox* {
        QGroupBox* box = new QGroupBox(mealName, page);
        QFormLayout* f = new QFormLayout(box);
        f->setSpacing(8);
        f->setContentsMargins(12, 16, 12, 12);

        *main  = new QLineEdit(box); (*main)->setPlaceholderText("e.g. Grilled chicken");
        *salad = new QLineEdit(box); (*salad)->setPlaceholderText("e.g. Green salad");
        *fruit = new QLineEdit(box); (*fruit)->setPlaceholderText("e.g. Orange");
        *juice = new QLineEdit(box); (*juice)->setPlaceholderText("e.g. Apple juice");
        *soup  = new QLineEdit(box); (*soup)->setPlaceholderText("optional");

        f->addRow("Main dish:", *main);
        f->addRow("Salad:",     *salad);
        f->addRow("Fruit:",     *fruit);
        f->addRow("Juice:",     *juice);
        f->addRow("Soup:",      *soup);
        return box;
    };

    QGroupBox* breakfastBox = makeMealGroup("Breakfast",
                                            &restBreakfastMain, &restBreakfastSalad,
                                            &restBreakfastFruit, &restBreakfastJuice, &restBreakfastSoup);

    QGroupBox* lunchBox = makeMealGroup("Lunch",
                                        &restLunchMain, &restLunchSalad,
                                        &restLunchFruit, &restLunchJuice, &restLunchSoup);

    QGroupBox* dinnerBox = makeMealGroup("Dinner",
                                         &restDinnerMain, &restDinnerSalad,
                                         &restDinnerFruit, &restDinnerJuice, &restDinnerSoup);

    QHBoxLayout* mealsRow = new QHBoxLayout();
    mealsRow->setSpacing(14);
    mealsRow->addWidget(breakfastBox);
    mealsRow->addWidget(lunchBox);
    mealsRow->addWidget(dinnerBox);

    QPushButton* saveBtn = new QPushButton("Save Menu", page);
    saveBtn->setProperty("class", "primaryButton");
    saveBtn->setCursor(Qt::PointingHandCursor);
    saveBtn->setFixedWidth(130);

    restaurantStatusLabel = new QLabel("", page);

    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addLayout(selectorRow);
    layout->addWidget(restaurantInfoLabel);
    layout->addLayout(mealsRow);
    layout->addWidget(saveBtn);
    layout->addWidget(restaurantStatusLabel);
    layout->addStretch(1);

    // ── Load existing menu when dorm selection changes ──
    connect(restaurantDormCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this]() {
                QString dormName = restaurantDormCombo->currentText();
                Dormitory* dorm = university.findDormitory(dormName.toStdString());
                if (!dorm) return;
                Restaurant& r = dorm->getRestaurant();
                restaurantInfoLabel->setText(
                    QString("Restaurant: %1").arg(QString::fromStdString(r.getName()))
                    );
                auto load = [](QLineEdit* w, const std::string& v) {
                    w->setText(QString::fromStdString(v));
                };
                Meal b = r.getBreakfast();
                load(restBreakfastMain,  b.mainDish);
                load(restBreakfastSalad, b.salad);
                load(restBreakfastFruit, b.fruit);
                load(restBreakfastJuice, b.juice);
                load(restBreakfastSoup,  b.soup);

                Meal l = r.getLunch();
                load(restLunchMain,  l.mainDish);
                load(restLunchSalad, l.salad);
                load(restLunchFruit, l.fruit);
                load(restLunchJuice, l.juice);
                load(restLunchSoup,  l.soup);

                Meal d = r.getDinner();
                load(restDinnerMain,  d.mainDish);
                load(restDinnerSalad, d.salad);
                load(restDinnerFruit, d.fruit);
                load(restDinnerJuice, d.juice);
                load(restDinnerSoup,  d.soup);
            });

    // ── Save button ──
    connect(saveBtn, &QPushButton::clicked, this, [this]() {
        QString dormName = restaurantDormCombo->currentText();
        Dormitory* dorm = university.findDormitory(dormName.toStdString());
        if (!dorm) return;
        Restaurant& r = dorm->getRestaurant();

        r.setBreakfast(
            restBreakfastMain->text().toStdString(),
            restBreakfastSalad->text().toStdString(),
            restBreakfastFruit->text().toStdString(),
            restBreakfastJuice->text().toStdString(),
            restBreakfastSoup->text().toStdString()
            );
        r.setLunch(
            restLunchMain->text().toStdString(),
            restLunchSalad->text().toStdString(),
            restLunchFruit->text().toStdString(),
            restLunchJuice->text().toStdString(),
            restLunchSoup->text().toStdString()
            );
        r.setDinner(
            restDinnerMain->text().toStdString(),
            restDinnerSalad->text().toStdString(),
            restDinnerFruit->text().toStdString(),
            restDinnerJuice->text().toStdString(),
            restDinnerSoup->text().toStdString()
            );
        restaurantStatusLabel->setText(
            QString("Menu saved for %1 Restaurant.").arg(dormName)
            );
    });

    return page;
}

// ════════════════════════════════════════════════════════
// SHARED REFRESH
// ════════════════════════════════════════════════════════
void MainWindow::refreshAllCombos() {
    QStringList dormNames;
    for (Dormitory* d : university.getDormitories())
        dormNames << QString::fromStdString(d->getName());

    auto repopulate = [&](QComboBox* combo) {
        QString current = combo->currentText();
        combo->blockSignals(true);
        combo->clear();
        combo->addItems(dormNames);
        int idx = combo->findText(current);
        combo->setCurrentIndex(idx >= 0 ? idx : 0);
        combo->blockSignals(false);
    };

    repopulate(dormSelectCombo);
    repopulate(dormForRoomCombo);
    repopulate(restaurantDormCombo);

    onDormitorySelectionChanged();
}
void MainWindow::showMessagesPage() {
    stack->setCurrentIndex(4);
    setActiveNavButton(navMessagesBtn);
    refreshMessages();
}

void MainWindow::onLogoutClicked() {
    FileManager::saveData(university, "udrms_data.txt");
    hide();
    emit logoutRequested();
}

void MainWindow::refreshMessages() {
    messagesListWidget->clear();
    QFile file("student_messages.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        messagesListWidget->addItem("No messages yet.");
        return;
    }
    QTextStream in(&file);
    QString all = in.readAll();
    file.close();

    QStringList blocks = all.split("─────────────────────────────\n",
                                   Qt::SkipEmptyParts);
    for (const QString& block : blocks) {
        if (block.trimmed().isEmpty()) continue;
        QListWidgetItem* item = new QListWidgetItem(block.trimmed());
        item->setForeground(QColor("#e8e8e8"));
        messagesListWidget->addItem(item);
    }

    if (messagesListWidget->count() == 0)
        messagesListWidget->addItem("No messages yet.");
}

void MainWindow::onReplyClicked() {
    QListWidgetItem* selected = messagesListWidget->currentItem();
    if (!selected) {
        messagesStatusLabel->setText("Please select a message to reply to.");
        return;
    }
    QString reply = replyInput->text().trimmed();
    if (reply.isEmpty()) {
        messagesStatusLabel->setText("Please type a reply first.");
        return;
    }

    // Extract student ID from selected message block
    QString text = selected->text();
    QString studentId;
    for (const QString& line : text.split('\n')) {
        if (line.startsWith("From")) {
            int s = line.indexOf('[') + 1;
            int e = line.indexOf(']');
            if (s > 0 && e > 0) studentId = line.mid(s, e - s).trimmed();
            break;
        }
    }

    // Save reply to a replies file
    QFile file("admin_replies.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << "─────────────────────────────\n";
        stream << "TO       : " << studentId << "\n";
        stream << "Date     : "
               << QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm") << "\n";
        stream << "Reply    : " << reply << "\n";
        file.close();

        replyInput->clear();
        messagesStatusLabel->setText("✅  Reply sent to student " + studentId + ".");
    }
}

void MainWindow::onResolveClicked() {
    QListWidgetItem* selected = messagesListWidget->currentItem();
    if (!selected) return;

    // Mark as resolved by changing the text colour to grey
    selected->setForeground(QColor("#555e6e"));
    QString text = selected->text();
    if (!text.startsWith("✅")) selected->setText("✅ [RESOLVED]\n" + text);
    messagesStatusLabel->setText("Message marked as resolved.");
}

QWidget* MainWindow::buildMessagesPage() {
    QWidget* page = new QWidget(this);
    page->setObjectName("contentArea");

    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->setSpacing(14);

    QLabel* title = new QLabel("Student Messages", page);
    title->setProperty("class", "pageTitle");

    QLabel* subtitle = new QLabel("Messages sent by students — select one to reply", page);
    subtitle->setProperty("class", "pageSubtitle");

    messagesListWidget = new QListWidget(page);
    messagesListWidget->setWordWrap(true);

    // Reply area
    QLabel* replyLabel = new QLabel("Reply to selected message:", page);
    replyLabel->setStyleSheet("color:#b8bcc4; font-size:12px; font-family:'Segoe UI';");

    replyInput = new QLineEdit(page);
    replyInput->setPlaceholderText("Type your reply here...");

    QHBoxLayout* btnRow = new QHBoxLayout();

    replyBtn = new QPushButton("Send Reply", page);
    replyBtn->setProperty("class", "primaryButton");
    replyBtn->setCursor(Qt::PointingHandCursor);

    resolveBtn = new QPushButton("Mark as Resolved ✓", page);
    resolveBtn->setCursor(Qt::PointingHandCursor);
    resolveBtn->setStyleSheet(
        "QPushButton { background-color:#1b4d5e; color:#ffffff; border:none;"
        "border-radius:6px; padding:10px 16px; font-weight:600;"
        "font-family:'Segoe UI'; }"
        "QPushButton:hover { background-color:#2a7a93; }");

    QPushButton* refreshBtn = new QPushButton("↻ Refresh", page);
    refreshBtn->setCursor(Qt::PointingHandCursor);
    refreshBtn->setStyleSheet(
        "QPushButton { background-color:#262b33; color:#b8bcc4; border:none;"
        "border-radius:6px; padding:10px 16px; font-family:'Segoe UI'; }"
        "QPushButton:hover { background-color:#2d333d; }");

    btnRow->addWidget(replyBtn);
    btnRow->addWidget(resolveBtn);
    btnRow->addWidget(refreshBtn);
    btnRow->addStretch(1);

    messagesStatusLabel = new QLabel("", page);
    messagesStatusLabel->setStyleSheet(
        "color:#52c785; font-size:12px; font-family:'Segoe UI';");

    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addWidget(messagesListWidget, 1);
    layout->addWidget(replyLabel);
    layout->addWidget(replyInput);
    layout->addLayout(btnRow);
    layout->addWidget(messagesStatusLabel);

    connect(replyBtn,   &QPushButton::clicked, this, &MainWindow::onReplyClicked);
    connect(resolveBtn, &QPushButton::clicked, this, &MainWindow::onResolveClicked);
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::refreshMessages);

    return page;
}