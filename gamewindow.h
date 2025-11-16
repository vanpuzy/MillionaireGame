#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include "models/Question.h"
#include "gamecontroller.h"
#include "models/GameState.h"
#include <QSoundEffect>
#include <QToolButton>
namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    void  intStyles();
    void initSounds();
    void restartGame();
    void highlightCorrect(const QString &opt);
    QToolButton* buttonForOption(const QString &opt);
    void resetStyle();
public :
signals:
    void goResultPage(QString prize, GameState state );
    void showPrizePage();
    void hidePrizePage();
    void changeCurrentLevel(int level);
private slots:
    void on_fiftyfiftyBtn_clicked();

    void on_addAdvisoryTeamBtn_clicked();

    void onClickAnswer();
    void onQuestionChanged(const Question &q);
    void onCorrect();
    void onWrong(const QString & correct);

    void on_optionABtn_clicked();

    void on_optionBBtn_clicked();

    void on_optionCBtn_clicked();

    void on_optionDBtn_clicked();

    void onAddQuestion();

    void onAudienceResult(const QList<QPair<QString,int>> &distribution);

    void on5050Options(const QList<QString> &remaining);

    void on_stopBtn_clicked();

private:
    Ui::GameWindow *ui;
    QSoundEffect* correctSound;
    QSoundEffect* wrongSound;
    QSoundEffect* step5Sound;

    GameController m_controller;
    QString m_origOptionStyle;
};

#endif // GAMEWINDOW_H
