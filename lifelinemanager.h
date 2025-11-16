#ifndef LIFELINEMANAGER_H
#define LIFELINEMANAGER_H

#include "models/Question.h"
#include <QPair>
#include <QList>

struct AudienceResult { // pair(option, percent)
    QList<QPair<QString,int>> distribution;
};

class LifelineManager {
public:
    LifelineManager() = default;
    QList<QString> use5050(const Question &q); // returns remaining options e.g. ["B","C"]
    AudienceResult askAudience(const Question &q);
};

#endif // LIFELINEMANAGER_H
