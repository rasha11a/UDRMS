#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QProgressBar>
#include <QPropertyAnimation>

class WelcomeWindow : public QWidget {
    Q_OBJECT

public:
    WelcomeWindow(QWidget* parent = nullptr);

signals:
    void finished();   // emitted when auto-advance or button click fires

private slots:
    void onTick();         // kept to avoid moc errors, body is empty
    void onGetStarted();

private:
    QPushButton* startButton;

    void setupUI();
    QWidget* buildLeftPanel();
    QWidget* buildRightPanel();
};

// ── Illustration widget ──────────────────────────────────────────────────────
// A custom QWidget that paints the dormitory scene in paintEvent()
#include <QWidget>
#include <QPainter>

class IllustrationWidget : public QWidget {
    Q_OBJECT
public:
    IllustrationWidget(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    void drawScene(QPainter& p);
    void drawBuilding(QPainter& p);
    void drawWing(QPainter& p, int x, int y, int w, int h,
                  const QString& label, const QColor& signColor);
    void drawTree(QPainter& p, int cx, int y, int r1, int r2);
    void drawStudent(QPainter& p, int x, int y, int variant);
    void drawLamp(QPainter& p, int x, int y);
    void drawStars(QPainter& p);
};

#endif // WELCOMEWINDOW_H