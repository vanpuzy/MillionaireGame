#include "lifelinemanager.h"

#include <QRandomGenerator>
#include <algorithm>
#include <QDebug>
QList<QString> LifelineManager::use5050(const Question &q) {
    // Keep correct answer and one random incorrect
    QList<QString> opts = {"A","B","C","D"};
    opts.removeOne(q.answer);
    qDebug() << "answer :" <<q.answer;
    int idx = QRandomGenerator::global()->bounded(opts.size());
    QString keptWrong = opts.at(idx);
    QList<QString> remaining;
    remaining.append(q.answer);
    remaining.append(keptWrong);
     qDebug() << "remaining answer :" <<keptWrong;
    // shuffle
    std::shuffle(remaining.begin(), remaining.end(), *QRandomGenerator::global());
    return remaining;
}

AudienceResult LifelineManager::askAudience(const Question &q) {
    AudienceResult res;
    // baseline: correct answer gets high percent depending on difficulty
    int baseCorrect = 60; // easy 80, medium 60, hard 40
    if (q.level == "easy") baseCorrect = 80;
    else if (q.level == "hard") baseCorrect = 40;
    else baseCorrect = 60;

    int correctPercent = baseCorrect + QRandomGenerator::global()->bounded(11) - 5; // +/-5 noise
    if (correctPercent < 10) correctPercent = 10;
    if (correctPercent > 95) correctPercent = 95;

    int rem = 100 - correctPercent;
    int p1 = QRandomGenerator::global()->bounded(rem+1);
    int p2 = QRandomGenerator::global()->bounded(rem - p1 + 1);
    int p3 = rem - p1 - p2;
    QList<QPair<QString,int>> dist;
    // assign to options
    QStringList opts = {"A","B","C","D"};
    // place correctPercent to q.answer
    for (const QString &o: opts) {
        if (o == q.answer) dist.append(qMakePair(o, correctPercent));
        else {
            int v = 0;
            if (p1>0) { v = p1; p1 = 0; }
            else if (p2>0) { v = p2; p2 = 0; }
            else if (p3>0) { v = p3; p3 = 0; }
            else v = 0;
            dist.append(qMakePair(o, v));
        }
    }
    // shuffle distribution a bit to avoid deterministic order
    std::shuffle(dist.begin(), dist.end(), *QRandomGenerator::global());
    res.distribution = dist;
    return res;
}
