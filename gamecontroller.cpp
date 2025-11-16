#include "GameController.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

GameController::GameController(QObject *parent) : QObject(parent)
{
    bool ok = m_qm.loadFromFile("questions.json");
    Q_UNUSED(ok)

    bool prizesOk = loadPrizesFromFile("prizes.json");
    std::reverse(m_prizes.begin(), m_prizes.end());
    if (!prizesOk) {
        qWarning() << "Cannot load prizes, using default values";
        m_prizes = {"15.000.000 VNĐ", "10.000.000 VNĐ", "5.000.000 VNĐ", "3.000.000 VNĐ",
                    "1.000.000 VNĐ", "500.000 VNĐ", "300.000 VNĐ", "200.000 VNĐ",
                    "150.000 VNĐ", "100.000 VNĐ", "75.000 VNĐ", "50.000 VNĐ",
                    "30.000 VNĐ", "20.000 VNĐ", "10.000 VNĐ"};
        std::reverse(m_prizes.begin(), m_prizes.end());
    }
}

bool GameController::loadPrizesFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return false;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) return false;

    QJsonObject obj = doc.object();
    QJsonArray arr = obj.value("prizes").toArray();
    if (arr.isEmpty()) return false;

    m_prizes.clear();
    for (auto v : arr) {
        QJsonObject prizeObj = v.toObject();
        m_prizes.append(prizeObj.value("text").toString());
    }

    return !m_prizes.isEmpty();
}

void GameController::start(const QString &level)
{
    m_levelFilter = level;
    m_index = 0;
    m_questionIndex = 0;
    m_gameOver = false;

    m_questions.clear();
    for (auto &q : m_qm.getAll()) {
        if (level.isEmpty() || q.level == level)
            m_questions.append(q);
    }

    if (!m_questions.isEmpty())
        m_current = m_questions.at(m_questionIndex);

    emit questionChanged(m_current);
    emit prizeChanged(m_prizes.value(m_index, "0 VNĐ"));
}

Question GameController::currentQuestion() const
{
    return m_current;
}

QString GameController::currentPrize() const
{
    return m_prizes.value(m_index, "0 VNĐ");
}

void GameController::nextQuestion()
{
    if (m_gameOver) return;

    m_questionIndex++;
    if (m_questionIndex < m_questions.size()) {
        m_current = m_questions.at(m_questionIndex);
        emit questionChanged(m_current);
    } else {
        m_gameOver = true;
        emit questionChanged(m_current);
    }
}

void GameController::answer(const QString &option)
{
    if (m_gameOver) return;

    if (option == m_current.answer) {
        emit correct();

        m_index = qMin(m_index + 1, m_prizes.size() - 1);
        emit prizeChanged(m_prizes.value(m_index, "0 VNĐ"));


        if (m_questionIndex >= m_questions.size() - 1) {
            m_gameOver = true;  // đánh dấu thắng
        }
        // Không gọi nextQuestion() luôn
        // GameWindow sẽ delay nhạc xong rồi gọi nextQuestion()
    } else {
        m_gameOver = true;
        emit wrong(m_current.answer);
    }
}

void GameController::use5050()
{
    if (m_gameOver) return;
    QList<QString> rem = m_lm.use5050(m_current);
    emit lifeline5050Used(rem);
}

void GameController::useAskAudience()
{
    if (m_gameOver) return;
    AudienceResult r = m_lm.askAudience(m_current);
    emit audienceResult(r.distribution);
}

bool GameController::gameOver() const { return m_gameOver; }
void GameController::setGameOver(bool gameOver) { m_gameOver = gameOver; }

QString GameController::safePrize()
{
    QList<int> safeIndexes = {4, 9, 14};
    int safeIdx = 0;
    for (int idx : safeIndexes) {
        if (m_index > idx) safeIdx = idx;
        else break;
    }
    return m_prizes.value(safeIdx, "0 VNĐ");
}
