#ifndef QUESTIONMANAGER_H
#define QUESTIONMANAGER_H

#include "models/Question.h"
#include <QList>

class QuestionManager
{
public:
    QuestionManager() = default;
    bool loadFromFile(const QString &path = "questions.json");
    QList<Question> getAll() const { return m_questions; }
    Question randomByLevel(const QString &level);
    void addQuestion(const Question &q);
    bool saveToFile(const QString &path = "questions.json") const;
private:
    QList<Question> m_questions;
};

#endif // QUESTIONMANAGER_H
