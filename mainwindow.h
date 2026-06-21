#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QVBoxLayout>
#include <QDate>
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QLabel>
#include <QComboBox>
#include "university.h"
#include "fileManager.h"
class MainWindow : public QMainWindow {
    Q_OBJECT
signals:
    void logoutRequested();

public:
    MainWindow(University& university, QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onAddStudentClicked();
    void onAddDormitoryClicked();
    void onAddRoomClicked();
    void showDashboardPage();
    void showStudentsPage();
    void showDormitoriesPage();
    void showRestaurantPage();
    void onDormitorySelectionChanged();   // updates room dropdown
    void onRemoveStudentClicked();
    void showMessagesPage();
    void onReplyClicked();
    void onLogoutClicked();

private:
    // ── Core layout ─────────────────────────────────────
    QStackedWidget* stack;
    QPushButton* navDashboardBtn;
    QPushButton* navStudentsBtn;
    QPushButton* navDormitoriesBtn;
    QPushButton* navRestaurantBtn;
    QPushButton* navMessagesBtn;
    // ── Restaurant meal inputs ────────────────────────────
    QLabel*    restaurantStatusLabel;
    QLineEdit* restBreakfastMain;
    QLineEdit* restBreakfastSalad;
    QLineEdit* restBreakfastFruit;
    QLineEdit* restBreakfastJuice;
    QLineEdit* restBreakfastSoup;
    QLineEdit* restLunchMain;
    QLineEdit* restLunchSalad;
    QLineEdit* restLunchFruit;
    QLineEdit* restLunchJuice;
    QLineEdit* restLunchSoup;
    QLineEdit* restDinnerMain;
    QLineEdit* restDinnerSalad;
    QLineEdit* restDinnerFruit;
    QLineEdit* restDinnerJuice;
    QLineEdit* restDinnerSoup;
    // ── Dashboard widgets ────────────────────────────────
    QLabel* totalStudentsValueLabel;
    QLabel* totalDormitoriesValueLabel;
    QLabel* totalRoomsValueLabel;
    QLabel* availableRoomsValueLabel;
    QLabel* occupancyRateValueLabel;
    QWidget* occupancyWidget;
    QVBoxLayout* occupancyLayout;
    // ── Students page widgets ────────────────────────────
    QLineEdit* idInput;
    QLineEdit* nameInput;
    QLineEdit* yearInput;
    QComboBox* dormSelectCombo;     // pick which dorm to enroll into
    QComboBox* roomSelectCombo;     // pick which room
    QListWidget* studentListWidget;
    QLabel* studentStatusLabel;
    QPushButton* removeStudentBtn;

    // ── Dormitories page widgets ─────────────────────────
    QLineEdit* dormNameInput;
    QLineEdit* dormCapacityInput;
    QLineEdit* roomNumberInput;
    QLineEdit* roomCapacityInput;
    QComboBox* dormForRoomCombo;    // pick dorm to add a room to
    QListWidget* dormListWidget;
    QLabel* dormStatusLabel;
    // ── Messages page widgets ────────────────────────────
    QListWidget* messagesListWidget;
    QLineEdit*   replyInput;
    QPushButton* replyBtn;
    QPushButton* resolveBtn;
    QLabel*      messagesStatusLabel;

    // ── Restaurant page widgets ──────────────────────────
    QComboBox* restaurantDormCombo;
    QLabel* restaurantInfoLabel;

    // ── Data ────────────────────────────────────────────
    // ── Data ────────────────────────────────────────────
    University& university;   // reference — shared with the whole app
    // ── Helpers ─────────────────────────────────────────
    void setupUI();
    QWidget* buildSidebar();
    QWidget* buildDashboardPage();
    QWidget* buildStudentsPage();
    QWidget* buildDormitoriesPage();
    QWidget* buildRestaurantPage();
    QWidget* buildStatCard(const QString& label, QLabel** valueLabelOut);

    void refreshDashboard();
    void refreshStudentList();
    void refreshDormitoryList();
    void refreshAllCombos();    // keeps all QComboBoxes in sync
    void refreshMessages();
    QWidget* buildMessagesPage();
    void setActiveNavButton(QPushButton* active);
    void onResolveClicked();

protected:
    void closeEvent(QCloseEvent* event) override;
};


#endif // MAINWINDOW_H