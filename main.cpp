#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"
#include "splashscreen.h"
#include "welcomewindow.h"
#include "roleselectionwindow.h"
#include "studentloginwindow.h"
#include "studentwindow.h"
#include "FileManager.h"
#include "adminloginwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&styleFile);
        app.setStyleSheet(stream.readAll());
        styleFile.close();
    }

    University* university = new University("ENSIA");
    FileManager::loadData(*university, "udrms_data.txt");
    if (university->getDormitories().empty()) {
        university->addDormitory("Dormitory A", 50);
        university->findDormitory("Dormitory A")->addRoom(101, 2);
        university->findDormitory("Dormitory A")->addRoom(102, 2);
    }

    SplashScreen*        splash   = new SplashScreen();
    RoleSelectionWindow* roles    = nullptr;
    MainWindow*          adminWin = nullptr;
    StudentLoginWindow*  stuLogin = nullptr;
    StudentWindow*       stuWin   = nullptr;

    auto showRoles = [&]() {
        if (!roles) {
            roles = new RoleSelectionWindow(*university);

            // Admin path — show login first
            QObject::connect(roles, &RoleSelectionWindow::adminSelected, [&]() {
                roles->hide();
                AdminLoginWindow* adminLogin = new AdminLoginWindow();

                QObject::connect(adminLogin, &AdminLoginWindow::backRequested, [&, adminLogin]() {
                    adminLogin->hide();
                    adminLogin->deleteLater();
                    roles->show();
                });

                QObject::connect(adminLogin, &AdminLoginWindow::loginSuccess, [&, adminLogin]() {
                    adminLogin->hide();
                    adminLogin->deleteLater();
                    if (!adminWin) {
                        adminWin = new MainWindow(*university);
                        QObject::connect(adminWin, &MainWindow::logoutRequested, [&]() {
                            adminWin->hide();
                            FileManager::saveData(*university, "udrms_data.txt");
                            roles->show();
                        });
                    }
                    adminWin->show();
                    adminWin->raise();
                });

                adminLogin->show();
            });
            QObject::connect(roles, &RoleSelectionWindow::studentSelected, [&]() {
                roles->hide();
                if (!stuLogin) {
                    stuLogin = new StudentLoginWindow(*university);

                    QObject::connect(stuLogin, &StudentLoginWindow::backRequested, [&]() {
                        stuLogin->hide();
                        roles->show();
                    });

                    QObject::connect(stuLogin, &StudentLoginWindow::loginSuccess,
                                     [&](Student* student) {
                                         stuLogin->hide();
                                         stuWin = new StudentWindow(*university, student);
                                         QObject::connect(stuWin, &StudentWindow::logoutRequested, [&]() {
                                             stuWin->hide();
                                             stuWin->deleteLater();
                                             stuWin   = nullptr;
                                             stuLogin->deleteLater();
                                             stuLogin = nullptr;
                                             roles->show();
                                         });
                                         stuWin->show();
                                     });
                }
                stuLogin->show();
            });
        }
        roles->show();
    };

    QObject::connect(splash, &SplashScreen::finished, [&]() {
        splash->deleteLater();
        WelcomeWindow* welcome = new WelcomeWindow();
        QObject::connect(welcome, &WelcomeWindow::finished, [&, welcome]() {
            welcome->deleteLater();
            showRoles();
        });
        welcome->show();
    });

    splash->show();
    return app.exec();
}