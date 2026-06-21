#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include "university.h"
#include "student.h"

class StudentWindow : public QMainWindow {
    Q_OBJECT

public:
    StudentWindow(University& university, Student* student, QWidget* parent = nullptr);

signals:
    void logoutRequested();

private slots:
    void showMyRoomPage();
    void showMenuPage();
    void showAvailableRoomsPage();
    void showMessagesPage();
    void showContactsPage();
    void onSendMessageClicked();

private:
    University& university;
    Student*    student;       // the logged-in student

    // ── Layout ──────────────────────────────────────────
    QStackedWidget* stack;
    QPushButton* navRoomBtn;
    QPushButton* navMenuBtn;
    QPushButton* navAvailBtn;
    QPushButton* navMsgBtn;
    QPushButton* navContactsBtn;

    // ── Messages page ────────────────────────────────────
    QComboBox*  msgTypeCombo;
    QTextEdit*  msgInput;
    QLabel*     msgStatusLabel;

    // ── Setup ───────────────────────────────────────────
    void setupUI();
    QWidget* buildSidebar();
    QWidget* buildMyRoomPage();
    QWidget* buildMenuPage();
    QWidget* buildAvailableRoomsPage();
    QWidget* buildMessagesPage();
    QWidget* buildContactsPage();

    // helpers
    QWidget* buildMealCard(const QString& mealName,
                           const QString& icon,
                           const Meal& meal);
    QWidget* buildInfoRow(const QString& label, const QString& value);
    void setActiveNavButton(QPushButton* active);
};

#endif // STUDENTWINDOW_H