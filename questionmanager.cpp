#include "questionmanager.h"


#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QRandomGenerator>

bool QuestionManager::loadFromFile(const QString &path) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return false;
    QByteArray data = f.readAll();
    f.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return false;
    QJsonArray arr = doc.array();
    m_questions.clear();
    for (int i=0;i<arr.size();++i) {
        QJsonObject o = arr[i].toObject();
        Question q;
        q.id = o.value("id").toInt(i+1);
        q.question = o.value("question").toString();
        q.A = o.value("A").toString();
        q.B = o.value("B").toString();
        q.C = o.value("C").toString();
        q.D = o.value("D").toString();
        q.answer = o.value("answer").toString();
        q.level = o.value("level").toString("medium");
        m_questions.append(q);
    }
    return true;
}

Question QuestionManager::randomByLevel(const QString &level) {
    QList<Question> list;
    for (auto &q : m_questions) {
        if (level.isEmpty() || q.level == level) list.append(q);
    }
    if (list.isEmpty()) {
        if (!m_questions.isEmpty()) return m_questions.at(QRandomGenerator::global()->bounded(m_questions.size()));
        return Question();
    }
    return list.at(QRandomGenerator::global()->bounded(list.size()));
}

void QuestionManager::addQuestion(const Question &q) {
    m_questions.append(q);
}

bool QuestionManager::saveToFile(const QString &path) const {
    QJsonArray arr;
    for (int i=0;i<m_questions.size();++i) {
        const Question &q = m_questions.at(i);
        QJsonObject o;
        o["id"] = q.id == 0 ? i+1 : q.id;
        o["question"] = q.question;
        o["A"] = q.A;
        o["B"] = q.B;
        o["C"] = q.C;
        o["D"] = q.D;
        o["answer"] = q.answer;
        o["level"] = q.level;
        arr.append(o);
    }
    QJsonDocument doc(arr);
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly)) return false;
    f.write(doc.toJson());
    f.close();
    return true;
}
