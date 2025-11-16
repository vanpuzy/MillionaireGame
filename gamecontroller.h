#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QPair>
#include "QuestionManager.h"
#include "LifelineManager.h"

class GameController : public QObject
{
    Q_OBJECT
public:
    explicit GameController(QObject *parent = nullptr);

    void start(const QString &level = "");
    Question currentQuestion() const;
    void nextQuestion();
    int currentIndex() const { return m_index; }
    QString currentPrize() const;
    QString safePrize();
    bool gameOver() const;
    void setGameOver(bool gameOver);

    bool loadPrizesFromFile(const QString &fileName);
    int totalQuestions() const {
        return m_questions.size();
    }
public slots:
    void answer(const QString &option);
    void use5050();
    void useAskAudience();

signals:
    void questionChanged(const Question &q);
    void correct();
    void wrong(const QString &correctAnswer);
    void lifeline5050Used(const QList<QString> &remainingOptions);
    void audienceResult(const QList<QPair<QString,int>> &distribution);
    void prizeChanged(const QString &prize);

private:
    QuestionManager m_qm;
    LifelineManager m_lm;

    QList<Question> m_questions; // danh sách câu hỏi tuần tự
    QList<QString> m_prizes;
    int m_index = 0;           // chỉ số prize
    int m_questionIndex = 0;   // chỉ số câu hỏi hiện tại
    QString m_levelFilter;
    Question m_current;
    bool m_gameOver = false;
};

#endif // GAMECONTROLLER_H
