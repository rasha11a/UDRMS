#include "studentwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

// ════════════════════════════════════════════════════════
// CONSTRUCTOR
// ════════════════════════════════════════════════════════
StudentWindow::StudentWindow(University& university, Student* student, QWidget* parent)
    : QMainWindow(parent), university(university), student(student) {
    setupUI();
}

void StudentWindow::setupUI() {
    setWindowTitle("UDRMS — Student Portal");
    resize(960, 600);

    QWidget* central = new QWidget(this);
    QHBoxLayout* root = new QHBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    stack = new QStackedWidget(this);
    stack->addWidget(buildMyRoomPage());         // 0
    stack->addWidget(buildMenuPage());           // 1
    stack->addWidget(buildAvailableRoomsPage()); // 2
    stack->addWidget(buildMessagesPage());       // 3
    stack->addWidget(buildContactsPage());       // 4

    root->addWidget(buildSidebar());
    root->addWidget(stack, 1);
    setCentralWidget(central);

    setActiveNavButton(navRoomBtn);
    stack->setCurrentIndex(0);
}

// ════════════════════════════════════════════════════════
// SIDEBAR
// ════════════════════════════════════════════════════════
QWidget* StudentWindow::buildSidebar() {
    QWidget* sidebar = new QWidget(this);
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(200);

    QVBoxLayout* layout = new QVBoxLayout(sidebar);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    // Student name at top
    QLabel* title = new QLabel("UDRMS", sidebar);
    title->setObjectName("sidebarTitle");
    title->setProperty("class", "sidebarTitle");

    QString shortName = QString::fromStdString(student->getFullName()).split(" ").first();
    QLabel* greeting = new QLabel("👋  " + shortName, sidebar);
    greeting->setStyleSheet("color: #52c785; font-size: 13px; font-weight: 600;"
                            "font-family: 'Segoe UI'; padding: 4px 20px 16px 20px;");

    navRoomBtn     = new QPushButton("  My Room",        sidebar);
    navMenuBtn     = new QPushButton("  Today's Menu",   sidebar);
    navAvailBtn    = new QPushButton("  Available Rooms",sidebar);
    navMsgBtn      = new QPushButton("  Send Message",   sidebar);
    navContactsBtn = new QPushButton("  Contacts",       sidebar);

    for (QPushButton* btn : {navRoomBtn, navMenuBtn, navAvailBtn,
                             navMsgBtn, navContactsBtn}) {
        btn->setProperty("class", "navButton");
        btn->setCheckable(true);
        btn->setCursor(Qt::PointingHandCursor);
    }

    // Logout button at the bottom
    QPushButton* logoutBtn = new QPushButton("  Logout", sidebar);
    logoutBtn->setProperty("class", "navButton");
    logoutBtn->setCursor(Qt::PointingHandCursor);
    logoutBtn->setStyleSheet(
        "QPushButton { background-color: transparent; color: #e05555;"
        "border: none; text-align: left; padding: 12px 20px; font-size: 14px; }"
        "QPushButton:hover { background-color: #2d1a1a; color: #ff6b6b; }"
        );

    layout->addWidget(title);
    layout->addWidget(greeting);
    layout->addWidget(navRoomBtn);
    layout->addWidget(navMenuBtn);
    layout->addWidget(navAvailBtn);
    layout->addWidget(navMsgBtn);
    layout->addWidget(navContactsBtn);
    layout->addStretch(1);
    layout->addWidget(logoutBtn);

    connect(navRoomBtn,     &QPushButton::clicked, this, &StudentWindow::showMyRoomPage);
    connect(navMenuBtn,     &QPushButton::clicked, this, &StudentWindow::showMenuPage);
    connect(navAvailBtn,    &QPushButton::clicked, this, &StudentWindow::showAvailableRoomsPage);
    connect(navMsgBtn,      &QPushButton::clicked, this, &StudentWindow::showMessagesPage);
    connect(navContactsBtn, &QPushButton::clicked, this, &StudentWindow::showContactsPage);
    connect(logoutBtn,      &QPushButton::clicked, this, &StudentWindow::logoutRequested);

    return sidebar;
}

void StudentWindow::setActiveNavButton(QPushButton* active) {
    for (QPushButton* btn : {navRoomBtn, navMenuBtn, navAvailBtn,
                             navMsgBtn, navContactsBtn})
        btn->setChecked(btn == active);
}

void StudentWindow::showMyRoomPage()          { stack->setCurrentIndex(0); setActiveNavButton(navRoomBtn); }
void StudentWindow::showMenuPage()            { stack->setCurrentIndex(1); setActiveNavButton(navMenuBtn); }
void StudentWindow::showAvailableRoomsPage()  { stack->setCurrentIndex(2); setActiveNavButton(navAvailBtn); }
void StudentWindow::showMessagesPage()        { stack->setCurrentIndex(3); setActiveNavButton(navMsgBtn); }
void StudentWindow::showContactsPage()        { stack->setCurrentIndex(4); setActiveNavButton(navContactsBtn); }

// ════════════════════════════════════════════════════════
// HELPERS
// ════════════════════════════════════════════════════════
QWidget* StudentWindow::buildInfoRow(const QString& label, const QString& value) {
    QWidget* row = new QWidget(this);
    row->setStyleSheet("background-color: #262b33; border-radius: 6px;");
    QHBoxLayout* layout = new QHBoxLayout(row);
    layout->setContentsMargins(14, 10, 14, 10);

    QLabel* lbl = new QLabel(label, row);
    lbl->setStyleSheet("color: #8b909a; font-size: 12px; font-family: 'Segoe UI';");
    lbl->setFixedWidth(140);

    QLabel* val = new QLabel(value, row);
    val->setStyleSheet("color: #ffffff; font-size: 13px; font-weight: 600;"
                       "font-family: 'Segoe UI';");

    layout->addWidget(lbl);
    layout->addWidget(val, 1);
    return row;
}

QWidget* StudentWindow::buildMealCard(const QString& mealName,
                                      const QString& icon,
                                      const Meal& meal) {
    QWidget* card = new QWidget(this);
    card->setStyleSheet("background-color: #262b33; border-radius: 10px;"
                        "border: 1px solid #313742;");

    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setContentsMargins(16, 14, 16, 14);
    layout->setSpacing(8);

    // header
    QLabel* header = new QLabel(icon + "  " + mealName, card);
    header->setStyleSheet("color: #52c785; font-size: 14px; font-weight: 700;"
                          "font-family: 'Segoe UI';");

    QFrame* div = new QFrame(card);
    div->setFixedHeight(1);
    div->setStyleSheet("background-color: #313742;");

    layout->addWidget(header);
    layout->addWidget(div);

    if (!meal.isSet()) {
        QLabel* na = new QLabel("Not assigned yet.\nPlease contact the administrator.", card);
        na->setStyleSheet("color: #8b909a; font-size: 12px; font-family: 'Segoe UI';");
        na->setWordWrap(true);
        layout->addWidget(na);
    } else {
        auto addField = [&](const QString& lbl, const std::string& val) {
            if (val.empty() || val == "Not set") return;
            QLabel* row = new QLabel(
                "<span style='color:#8b909a;'>" + lbl + ":</span>  " +
                    QString::fromStdString(val), card);
            row->setStyleSheet("color: #e8e8e8; font-size: 12px; font-family: 'Segoe UI';");
            layout->addWidget(row);
        };
        addField("Main dish", meal.mainDish);
        addField("Salad",     meal.salad);
        addField("Fruit",     meal.fruit);
        addField("Juice",     meal.juice);
        if (!meal.soup.empty())
            addField("Soup", meal.soup);
    }
    layout->addStretch(1);
    return card;
}

// ════════════════════════════════════════════════════════
// PAGE 1 — MY ROOM
// ════════════════════════════════════════════════════════
QWidget* StudentWindow::buildMyRoomPage() {
    QWidget* page = new QWidget(this);
    page->setObjectName("contentArea");

    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->setSpacing(12);

    QLabel* title = new QLabel("My Room", page);
    title->setProperty("class", "pageTitle");

    QLabel* subtitle = new QLabel("Your accommodation details", page);
    subtitle->setProperty("class", "pageSubtitle");

    layout->addWidget(title);
    layout->addWidget(subtitle);

    if (!student->isAssigned()) {
        QLabel* msg = new QLabel("You are not currently assigned to any room.\n"
                                 "Please contact the administrator.", page);
        msg->setStyleSheet("color: #8b909a; font-size: 13px; font-family: 'Segoe UI';");
        msg->setWordWrap(true);
        layout->addWidget(msg);
        layout->addStretch(1);
        return page;
    }

    // Info rows
    layout->addWidget(buildInfoRow("Student ID",     QString::fromStdString(student->getId())));
    layout->addWidget(buildInfoRow("Full Name",      QString::fromStdString(student->getFullName())));
    layout->addWidget(buildInfoRow("Academic Year",  QString("Year %1").arg(student->getAcademicYear())));
    layout->addWidget(buildInfoRow("Dormitory",      QString::fromStdString(student->getDormitoryName())));
    layout->addWidget(buildInfoRow("Room Number",    QString::number(student->getRoomNumber())));

    // Roommates
    Dormitory* dorm = university.findDormitory(student->getDormitoryName());
    if (dorm) {
        Room* room = dorm->findRoom(student->getRoomNumber());
        if (room) {
            layout->addWidget(buildInfoRow("Room Capacity",
                                           QString("%1 / %2 occupied")
                                               .arg(room->getOccupancy())
                                               .arg(room->getCapacity())));

            // List roommates
            QLabel* rmLabel = new QLabel("Roommates:", page);
            rmLabel->setStyleSheet("color: #8b909a; font-size: 12px;"
                                   "font-family: 'Segoe UI'; margin-top: 8px;");
            layout->addWidget(rmLabel);

            bool hasRoommates = false;
            for (Student* s : dorm->getStudents()) {
                if (s->getRoomNumber() == student->getRoomNumber() &&
                    s->getId() != student->getId()) {
                    layout->addWidget(buildInfoRow("",
                                                   QString::fromStdString(s->getFullName()) +
                                                       "  (Year " + QString::number(s->getAcademicYear()) + ")"));
                    hasRoommates = true;
                }
            }
            if (!hasRoommates) {
                QLabel* alone = new QLabel("  No roommates yet.", page);
                alone->setStyleSheet("color: #8b909a; font-size: 12px;"
                                     "font-family: 'Segoe UI';");
                layout->addWidget(alone);
            }
        }
    }

    layout->addStretch(1);
    return page;
}

// ════════════════════════════════════════════════════════
// PAGE 2 — TODAY'S MENU
// ════════════════════════════════════════════════════════
QWidget* StudentWindow::buildMenuPage() {
    QWidget* page = new QWidget(this);
    page->setObjectName("contentArea");

    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->setSpacing(14);

    QLabel* title = new QLabel("Today's Menu", page);
    title->setProperty("class", "pageTitle");

    QString dateStr = QDateTime::currentDateTime().toString("dddd, MMMM d yyyy");
    QLabel* subtitle = new QLabel(dateStr, page);
    subtitle->setProperty("class", "pageSubtitle");

    layout->addWidget(title);
    layout->addWidget(subtitle);

    // Get meals from the student's dormitory restaurant
    Dormitory* dorm = university.findDormitory(student->getDormitoryName());
    Meal breakfast, lunch, dinner;
    if (dorm) {
        Restaurant& r = dorm->getRestaurant();
        breakfast = r.getBreakfast();
        lunch     = r.getLunch();
        dinner    = r.getDinner();
    }

    QHBoxLayout* mealsRow = new QHBoxLayout();
    mealsRow->setSpacing(14);
    mealsRow->addWidget(buildMealCard("Breakfast", "🌅", breakfast));
    mealsRow->addWidget(buildMealCard("Lunch",     "☀️",  lunch));
    mealsRow->addWidget(buildMealCard("Dinner",    "🌙", dinner));

    layout->addLayout(mealsRow);
    layout->addStretch(1);
    return page;
}

// ════════════════════════════════════════════════════════
// PAGE 3 — AVAILABLE ROOMS
// ════════════════════════════════════════════════════════
QWidget* StudentWindow::buildAvailableRoomsPage() {
    QWidget* page = new QWidget(this);
    page->setObjectName("contentArea");

    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->setSpacing(14);

    QLabel* title = new QLabel("Available Rooms", page);
    title->setProperty("class", "pageTitle");

    QLabel* subtitle = new QLabel("Rooms with free spaces across all dormitories", page);
    subtitle->setProperty("class", "pageSubtitle");

    layout->addWidget(title);
    layout->addWidget(subtitle);

    QListWidget* list = new QListWidget(page);

    bool found = false;
    for (Dormitory* d : university.getDormitories()) {
        for (Room* r : d->getRooms()) {
            if (!r->isFull()) {
                int free = r->getCapacity() - r->getOccupancy();
                list->addItem(
                    QString("🏠  %1  —  Room %2  —  %3 free space%4")
                        .arg(QString::fromStdString(d->getName()))
                        .arg(r->getRoomNumber())
                        .arg(free)
                        .arg(free == 1 ? "" : "s")
                    );
                found = true;
            }
        }
    }

    if (!found) {
        list->addItem("No available rooms at the moment.");
    }

    QLabel* note = new QLabel("💡  To request a room change, use the Send Message page.", page);
    note->setStyleSheet("color: #8b909a; font-size: 12px; font-family: 'Segoe UI';"
                        "padding: 8px; background-color: #262b33; border-radius: 6px;");
    note->setWordWrap(true);

    layout->addWidget(list, 1);
    layout->addWidget(note);
    return page;
}

// ════════════════════════════════════════════════════════
// PAGE 4 — SEND MESSAGE
// ════════════════════════════════════════════════════════
QWidget* StudentWindow::buildMessagesPage() {
    QWidget* page = new QWidget(this);
    page->setObjectName("contentArea");

    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->setSpacing(14);

    QLabel* title = new QLabel("Send Message", page);
    title->setProperty("class", "pageTitle");

    QLabel* subtitle = new QLabel("Contact the dormitory administration", page);
    subtitle->setProperty("class", "pageSubtitle");

    layout->addWidget(title);
    layout->addWidget(subtitle);

    // message type selector
    msgTypeCombo = new QComboBox(page);
    msgTypeCombo->addItems({
        "🔧  Maintenance Request",
        "🚪  Room Change Request",
        "🍽  Restaurant Complaint",
        "❓  General Question",
        "🚨  Urgent Issue"
    });

    // message text
    msgInput = new QTextEdit(page);
    msgInput->setPlaceholderText("Describe your request or problem here...");
    msgInput->setFixedHeight(140);
    msgInput->setStyleSheet(
        "QTextEdit { background-color: #262b33; border: 1px solid #3a4150;"
        "border-radius: 6px; padding: 8px; color: #ffffff; font-family: 'Segoe UI'; }"
        "QTextEdit:focus { border: 1px solid #52c785; }"
        );

    QPushButton* sendBtn = new QPushButton("Send Message", page);
    sendBtn->setProperty("class", "primaryButton");
    sendBtn->setCursor(Qt::PointingHandCursor);

    msgStatusLabel = new QLabel("", page);
    msgStatusLabel->setStyleSheet("color: #52c785; font-size: 12px; font-family: 'Segoe UI';");

    QLabel* typeLabel = new QLabel("Request type:", page);
    typeLabel->setStyleSheet("color: #b8bcc4; font-size: 12px; font-family: 'Segoe UI';");

    QLabel* msgLabel = new QLabel("Your message:", page);
    msgLabel->setStyleSheet("color: #b8bcc4; font-size: 12px; font-family: 'Segoe UI';");

    layout->addWidget(typeLabel);
    layout->addWidget(msgTypeCombo);
    layout->addWidget(msgLabel);
    layout->addWidget(msgInput);
    layout->addWidget(sendBtn);
    layout->addWidget(msgStatusLabel);
    layout->addStretch(1);

    connect(sendBtn, &QPushButton::clicked, this, &StudentWindow::onSendMessageClicked);
    // Show admin replies for this student
    QLabel* repliesTitle = new QLabel("Admin Replies:", page);
    repliesTitle->setStyleSheet(
        "color:#b8bcc4; font-size:12px; font-family:'Segoe UI'; margin-top:8px;");

    QListWidget* repliesList = new QListWidget(page);
    repliesList->setFixedHeight(120);

    // Load replies addressed to this student
    QFile rFile("admin_replies.txt");
    if (rFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&rFile);
        QString all = in.readAll();
        rFile.close();
        QStringList blocks = all.split("─────────────────────────────\n",
                                       Qt::SkipEmptyParts);
        QString myId = QString::fromStdString(student->getId());
        for (const QString& block : blocks) {
            if (block.contains("TO       : " + myId)) {
                repliesList->addItem(block.trimmed());
            }
        }
    }
    if (repliesList->count() == 0)
        repliesList->addItem("No replies yet from the administration.");

    layout->addWidget(repliesTitle);
    layout->addWidget(repliesList);

    return page;

}

void StudentWindow::onSendMessageClicked() {
    QString text = msgInput->toPlainText().trimmed();
    if (text.isEmpty()) {
        msgStatusLabel->setText("Please write a message before sending.");
        msgStatusLabel->setStyleSheet("color: #e05555; font-size: 12px; font-family: 'Segoe UI';");
        return;
    }

    // Save message to a text file that admin can read
    QFile file("student_messages.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << "─────────────────────────────\n";
        stream << "From     : " << QString::fromStdString(student->getFullName())
               << "  [" << QString::fromStdString(student->getId()) << "]\n";
        stream << "Room     : " << student->getRoomNumber()
               << "  —  " << QString::fromStdString(student->getDormitoryName()) << "\n";
        stream << "Type     : " << msgTypeCombo->currentText() << "\n";
        stream << "Date     : " << QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm") << "\n";
        stream << "Message  : " << text << "\n";
        file.close();

        msgInput->clear();
        msgStatusLabel->setText("✅  Message sent successfully. "
                                "The administration will respond shortly.");
        msgStatusLabel->setStyleSheet("color: #52c785; font-size: 12px; font-family: 'Segoe UI';");
    } else {
        msgStatusLabel->setText("Failed to send message. Please try again.");
        msgStatusLabel->setStyleSheet("color: #e05555; font-size: 12px; font-family: 'Segoe UI';");
    }
}

// ════════════════════════════════════════════════════════
// PAGE 5 — CONTACTS
// ════════════════════════════════════════════════════════
QWidget* StudentWindow::buildContactsPage() {
    QWidget* page = new QWidget(this);
    page->setObjectName("contentArea");

    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->setSpacing(12);

    QLabel* title = new QLabel("Important Contacts", page);
    title->setProperty("class", "pageTitle");

    QLabel* subtitle = new QLabel("Key numbers and contacts at the dormitory", page);
    subtitle->setProperty("class", "pageSubtitle");

    layout->addWidget(title);
    layout->addWidget(subtitle);

    // Contact cards
    struct Contact { QString icon; QString role; QString name; QString phone; };
    QList<Contact> contacts = {
                                {"🏠", "Chef de Pavillon",      "M. Rachid Bensalem",   "023 45 67 89"},
                                {"🔧", "Maintenance (Siyana)",  "M. Karim Hamidi",      "023 45 67 90"},
                                {"🍽", "Restaurant Manager",    "Mme. Fatima Zahra",    "023 45 67 91"},
                                {"🚑", "Medical Infirmary",     "Dr. Nadia Boucherit",  "023 45 67 92"},
                                {"🔒", "Security Office",       "M. Yacine Taleb",      "023 45 67 93"},
                                {"📋", "Administration Office", "Mme. Asma Khaldi",     "023 45 67 94"},
                                };

    for (const Contact& c : contacts) {
        QWidget* card = new QWidget(page);
        card->setStyleSheet("background-color: #262b33; border-radius: 8px;");

        QHBoxLayout* row = new QHBoxLayout(card);
        row->setContentsMargins(16, 12, 16, 12);

        QLabel* iconLbl = new QLabel(c.icon, card);
        iconLbl->setStyleSheet("font-size: 22px;");
        iconLbl->setFixedWidth(36);

        QLabel* infoLbl = new QLabel(
            "<b style='color:#ffffff;'>" + c.role + "</b><br>"
                                                    "<span style='color:#b8bcc4;'>" + c.name + "</span>", card);
        infoLbl->setStyleSheet("font-family: 'Segoe UI'; font-size: 12px;");

        QLabel* phoneLbl = new QLabel("📞  " + c.phone, card);
        phoneLbl->setStyleSheet("color: #52c785; font-size: 12px;"
                                "font-family: 'Segoe UI'; font-weight: 600;");

        row->addWidget(iconLbl);
        row->addWidget(infoLbl, 1);
        row->addWidget(phoneLbl);

        layout->addWidget(card);
    }

    layout->addStretch(1);
    return page;
}