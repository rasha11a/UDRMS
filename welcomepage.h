#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class WelcomePage : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomePage(QWidget *parent = nullptr);
    ~WelcomePage();

signals:
    void enterRequested(); // emitted when user clicks Enter or timer fires

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onEnterClicked();
    void onAutoAdvance();
    void updateCountdown();

private:
    void setupUI();
    void setupLeftPanel();
    void setupRightPanel();
    void startAnimations();

    // Left panel
    QWidget     *m_leftPanel;
    QLabel      *m_illustrationLabel;
    QLabel      *m_logoLabel;
    QLabel      *m_taglineLabel;

    // Right panel
    QWidget     *m_rightPanel;
    QLabel      *m_greetingLabel;
    QLabel      *m_systemNameLabel;
    QLabel      *m_descriptionLabel;
    QLabel      *m_featuresLabel;
    QPushButton *m_enterBtn;
    QLabel      *m_countdownLabel;
    QLabel      *m_versionLabel;

    // Timers
    QTimer *m_autoAdvanceTimer;
    QTimer *m_countdownTimer;
    int     m_countdown;

    // Animations
    QGraphicsOpacityEffect *m_rightPanelEffect;
    QPropertyAnimation     *m_fadeInAnim;
};

#endif // WELCOMEPAGE_H
