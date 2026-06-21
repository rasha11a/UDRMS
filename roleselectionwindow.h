#ifndef ROLESELECTIONWINDOW_H
#define ROLESELECTIONWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "university.h"

class RoleSelectionWindow : public QWidget {
    Q_OBJECT

public:
    // university is passed by reference so Student and Admin windows
    // share the exact same data — no copying
    explicit RoleSelectionWindow(University& university, QWidget* parent = nullptr);

signals:
    void adminSelected();
    void studentSelected();

private:
    University& university;
    void setupUI();
};

#endif // ROLESELECTIONWINDOW_H