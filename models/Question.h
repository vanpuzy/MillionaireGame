#ifndef QUESTION_H
#define QUESTION_H

#include <QString>

struct Question {
    int id = 0;
    QString question;
    QString A;
    QString B;
    QString C;
    QString D;
    QString answer; // "A","B","C","D"
    QString level;  // "easy","medium","hard"
};

#endif // QUESTION_H
