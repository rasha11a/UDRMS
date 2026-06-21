#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>

class SplashScreen : public QWidget {
    Q_OBJECT

public:
    SplashScreen(QWidget* parent = nullptr);

signals:
    // Emitted automatically once the splash sequence is finished,
    // so MainWindow knows when to appear.
    void finished();

private:
    QLabel* titleLabel;
    QLabel* subtitleLabel;
    QPropertyAnimation* fadeInAnim;
    QPropertyAnimation* fadeOutAnim;

    void startSequence();
};

#endif // SPLASHSCREEN_H