#ifndef STUDENTLOGINWINDOW_H
#define STUDENTLOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "university.h"
#include "student.h"

class StudentLoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit StudentLoginWindow(University& university, QWidget* parent = nullptr);

signals:
    void loginSuccess(Student* student);
    void backRequested();

private slots:
    void onLoginClicked();

private:
    University& university;
    QLineEdit* nameInput;
    QLineEdit* idInput;
    QLabel*    errorLabel;
    void setupUI();
};

#endif // STUDENTLOGINWINDOW_H