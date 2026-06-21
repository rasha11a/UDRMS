#ifndef ADMINLOGINWINDOW_H
#define ADMINLOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class AdminLoginWindow : public QWidget {
    Q_OBJECT
public:
    explicit AdminLoginWindow(QWidget* parent = nullptr);

signals:
    void loginSuccess();
    void backRequested();

private slots:
    void onLoginClicked();

private:
    QLineEdit* passwordInput;
    QLabel*    errorLabel;
    void setupUI();
};

#endif
