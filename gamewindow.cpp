#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "addquestiondialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QProgressBar>
#include <QDialog>

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChartView>


using namespace QtCharts;

GameWindow::GameWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    connect(&m_controller, &GameController::questionChanged, this, &GameWindow::onQuestionChanged);
    connect(&m_controller, &GameController::correct, this, &GameWindow::onCorrect);
    connect(&m_controller, &GameController::wrong, this, &GameWindow::onWrong);
    connect(&m_controller, &GameController::lifeline5050Used, this, &GameWindow::on5050Options);
    connect(&m_controller, &GameController::audienceResult, this, &GameWindow::onAudienceResult);

    m_controller.start();

    intStyles();
    resetStyle();
    initSounds();
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::intStyles()
{
    ui->fiftyfiftyBtn->setStyleSheet(
                "QToolButton {"
                "   background-color: #55007F;"
                "   color: white;"
                "   border-radius: 10px;"
                "   padding: 5px 12px;"
                "}"
                "QToolButton:disabled {"
                "   background-color: #A080B5;"
                "   color: #EEE;"
                "}"
                "QToolButton:hover {"
                "   background-color: #7A1AB3;"
                "}"
                "QToolButton:pressed {"
                "   background-color: #4B0055;"
                "}"
                );

    ui->addAdvisoryTeamBtn->setStyleSheet(
                "QToolButton {"
                "   background-color: rgb(222, 66, 17);"
                "   color: white;"
                "   border-radius: 10px;"
                "   padding: 5px 12px;"
                "   border: none;"
                "}"
                "QToolButton:disabled {"
                "   background-color: rgb(240, 150, 130);"
                "   color: #EEE;"
                "   border: none;"
                "}"
                "QToolButton:hover:!disabled {"
                "   background-color: rgb(255, 90, 40);"
                "}"
                "QToolButton:pressed:!disabled {"
                "   background-color: rgb(180, 50, 10);"
                "}"
                );
}

void GameWindow::initSounds()
{
    correctSound = new QSoundEffect(this);
    correctSound->setSource(QUrl::fromLocalFile(":/sounds/correct.wav"));
    correctSound->setVolume(0.5);

    wrongSound = new QSoundEffect(this);
    wrongSound->setSource(QUrl::fromLocalFile(":/sounds/wrong.wav"));
    wrongSound->setVolume(0.5);


    step5Sound = new QSoundEffect(this);
    step5Sound->setSource(QUrl::fromLocalFile(":/sounds/step5.wav"));
    step5Sound->setVolume(0.5);
}

void GameWindow::restartGame()
{
    m_controller.start();
    ui->fiftyfiftyBtn->setEnabled(true);
    ui->addAdvisoryTeamBtn->setEnabled(true);
    ui->stopBtn->setEnabled(true);
}

void GameWindow::highlightCorrect(const QString &opt)
{
    QToolButton *b = buttonForOption(opt);
    if (b)
        b->setStyleSheet( "QToolButton {"
                          "    background-color: #8BC34A;"
                          "    border: 2px solid rgb(243, 230, 196);"
                          "    color: rgb(132, 17, 12);"
                          "    border-radius: 10px;"
                          "    padding: 8px 15px;"
                          "}"
                          "QToolButton:hover {"
                          "    background-color: rgb(250, 240, 225);"
                          "}"
                          "QToolButton:pressed {"
                          "    background-color: rgb(230, 210, 185);"
                          "}");

}

QToolButton *GameWindow::buttonForOption(const QString &opt)
{
    if (opt == "A") return ui->optionABtn;
    if (opt == "B") return ui->optionBBtn;
    if (opt == "C") return ui->optionCBtn;
    if (opt == "D") return ui->optionDBtn;
    return nullptr;
}

void GameWindow::resetStyle()
{

    m_origOptionStyle =  "QToolButton {"
                         "        background-color: rgb(242, 229, 210);"
                         "    border: 2px solid rgb(243, 230, 196);"
                         "    color: rgb(132, 17, 12);"
                         "    border-radius: 10px;"
                         "    padding: 8px 15px;"
                         "}"
                         "QToolButton:hover {"
                         "    background-color: rgb(250, 240, 225);"
                         "}"
                         "QToolButton:pressed {"
                         "    background-color: rgb(230, 210, 185);"
                         "}";

    ui->optionABtn->setStyleSheet(m_origOptionStyle);
    ui->optionBBtn->setStyleSheet(m_origOptionStyle);
    ui->optionCBtn->setStyleSheet(m_origOptionStyle);
    ui->optionDBtn->setStyleSheet(m_origOptionStyle);



}

void GameWindow::on_fiftyfiftyBtn_clicked()
{
    qDebug() << " on_fiftyfiftyBtn_clicked ";

    m_controller.use5050();
    ui->fiftyfiftyBtn->setEnabled(false);
    // Set style sheet mới

}

void GameWindow::on_addAdvisoryTeamBtn_clicked()
{
    m_controller.useAskAudience();
    ui->addAdvisoryTeamBtn->setEnabled(false);
}

void GameWindow::onClickAnswer()
{
    QPushButton *b = qobject_cast<QPushButton*>(sender());
    if (!b) return;
    QString text = b->text(); // e.g. "A: Hà Nội"
    QString opt = text.section(':', 0, 0).trimmed();
    // disable buttons to avoid double-click
    ui->optionABtn->setEnabled(false);  ui->optionBBtn->setEnabled(false);  ui->optionCBtn->setEnabled(false);  ui->optionDBtn->setEnabled(false);
    // check answer
    m_controller.answer(opt);
}

void GameWindow::onQuestionChanged(const Question &q)
{
    ui->questionLabel->setText(QString("Câu hỏi số %1: %2")
                               .arg(m_controller.currentIndex() + 1)  // +1 vì m_index là 0-based
                               .arg(q.question));
    ui->optionABtn->setText(QString("     %1").arg(q.A));
    ui->optionBBtn->setText(QString("     %1").arg(q.B));
    ui->optionCBtn->setText(QString("     %1").arg(q.C));
    ui->optionDBtn->setText(QString("     %1").arg(q.D));

    // reset styles
    ui->optionABtn->setEnabled(true);
    ui->optionBBtn->setEnabled(true);
    ui->optionCBtn->setEnabled(true);
    ui->optionDBtn->setEnabled(true);
    //    ui->optionABtn->setStyleSheet("");   ui->optionBBtn->setStyleSheet("");   ui->optionCBtn->setStyleSheet("");   ui->optionDBtn->setStyleSheet("");
    this->resetStyle();
}

void GameWindow::onCorrect()
{
    int timeSleep = 3000;
    if((m_controller.currentIndex()+1) %5 ==0)
    {
        step5Sound->play();
        timeSleep = 2000;
    } else {
        correctSound->play();
    }

    highlightCorrect(m_controller.currentQuestion().answer);

    // Delay 1 giây để highlight
    QTimer::singleShot(1000, [this, timeSleep]() {

        emit showPrizePage();
        emit changeCurrentLevel(m_controller.currentIndex());

        // Delay thêm để người chơi xem PrizeLadderWidget
        QTimer::singleShot(timeSleep, [this]() {
            step5Sound->stop();

            if (m_controller.gameOver()) {
                emit goResultPage(m_controller.currentPrize(), GameState::Won);
            } else {
                m_controller.nextQuestion();
                emit hidePrizePage();
            }
        });
    });
}


void GameWindow::onWrong(const QString &correct)
{
    wrongSound->play();
    highlightCorrect(correct);
    //     QMessageBox::information(this, "Wrong", QString("Sai! Đáp án đúng là %1").arg(correct));
    //    // Restart game after wrong
    //    m_controller.start();
    ui->optionABtn->setEnabled(false);
    ui->optionBBtn->setEnabled(false);
    ui->optionCBtn->setEnabled(false);
    ui->optionDBtn->setEnabled(false);

    QTimer::singleShot(3000, [this]() {
        emit goResultPage(m_controller.safePrize(), GameState::Lose); // Hàm bạn sẽ tạo để chuyển trang kết quả
    });

    ui->stopBtn->setEnabled(false);
}

void GameWindow::on_optionABtn_clicked()
{
    // disable buttons to avoid double-click
    ui->optionABtn->setEnabled(false);
    ui->optionBBtn->setEnabled(false);
    ui->optionCBtn->setEnabled(false);
    ui->optionDBtn->setEnabled(false);
    // check answer
    m_controller.answer("A");
}

void GameWindow::on_optionBBtn_clicked()
{
    // disable buttons to avoid double-click
    ui->optionABtn->setEnabled(false);
    ui->optionBBtn->setEnabled(false);
    ui->optionCBtn->setEnabled(false);
    ui->optionDBtn->setEnabled(false);
    // check answer
    m_controller.answer("B");
}

void GameWindow::on_optionCBtn_clicked()
{
    ui->optionABtn->setEnabled(false);
    ui->optionBBtn->setEnabled(false);
    ui->optionCBtn->setEnabled(false);
    ui->optionDBtn->setEnabled(false);
    // check answer
    m_controller.answer("C");
}

void GameWindow::on_optionDBtn_clicked()
{
    ui->optionABtn->setEnabled(false);
    ui->optionBBtn->setEnabled(false);
    ui->optionCBtn->setEnabled(false);
    ui->optionDBtn->setEnabled(false);
    // check answer
    m_controller.answer("D");
}

void GameWindow::onAddQuestion()
{
    AddQuestionDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        Question q = dlg.question();
        // Append to JSON file and reload manager (simplified)
        QFile f("questions.json");
        if (!f.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Error", "Cannot open questions.json for read.");
            return;
        }
        QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
        f.close();
        QJsonArray arr = doc.isArray() ? doc.array() : QJsonArray();
        QJsonObject o;
        o["question"] = q.question;
        o["A"] = q.A;
        o["B"] = q.B;
        o["C"] = q.C;
        o["D"] = q.D;
        o["answer"] = q.answer;
        o["level"] = q.level;
        arr.append(o);
        doc.setArray(arr);
        if (!f.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, "Error", "Cannot open questions.json for write.");
            return;
        }
        f.write(doc.toJson());
        f.close();
        QMessageBox::information(this, "Saved", "Question saved. Restarting game to load.");
        m_controller.start(); // reloads inside controller
    }
}

void GameWindow::onAudienceResult(const QList<QPair<QString, int> > &distribution)
{
    // Tạo 1 set duy nhất chứa dữ liệu
    QBarSet *set = new QBarSet("Audience");
    for (auto &p : distribution) {
        *set << p.second; // thêm phần trăm vào set
    }
    set->setColor(QColor("#FFD700")); // màu vàng kiểu Ai là triệu phú

    QBarSeries *series = new QBarSeries();
    series->append(set);

    // Chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Ý kiến khán giả");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->hide(); // ẩn legend

    // Trục X: A, B, C, D
    QStringList categories;
    for (auto &p : distribution) {
        categories << p.first;
    }
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Trục Y: 0-100%
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setLabelFormat("%d%%");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Hiển thị chart trong QDialog
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QDialog dlg(this);
    dlg.setWindowTitle("Ý kiến khán giả");
    dlg.setStyleSheet("background-color: #2b2b2b;");
    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    layout->addWidget(chartView);

    dlg.resize(400, 300);
    dlg.exec();

    //    QString msg = "Audience poll:\n";
    //    for (auto &p : distribution) {
    //        msg += QString("%1 : %2%\n").arg(p.first).arg(p.second);

    //    }
    //    QMessageBox::information(this, "Audience", msg);
    //    qDebug() << " Audien msg " << msg;
}

void GameWindow::on5050Options(const QList<QString> &remaining)
{
    // disable the other two options
    //    QStringList keep;
    //    for (auto &s : remaining)
    //    {
    //        keep << s;
    //        qDebug() << "remaining " << s;
    //    }

    //    QList<QToolButton*> all = {ui->optionABtn, ui->optionBBtn, ui->optionCBtn, ui->optionDBtn};
    //    for (QToolButton *b : all) {
    //        QString opt = b->text().section(':',0,0).trimmed();
    //        if (!keep.contains(opt)) {
    //            b->setEnabled(false);
    //            b->setStyleSheet("background-color: #cccccc;");
    //        }
    //    }

    for (const QString &opt : {"A", "B", "C", "D"}) {
        QToolButton *btn = buttonForOption(opt);
        if (!remaining.contains(opt)) {
            if (btn) {
                btn->setEnabled(false);
                btn->setStyleSheet("QToolButton {"
                                   "        background-color: #cccccc;"
                                   "    border: 2px solid rgb(243, 230, 196);"
                                   "    color: rgb(132, 17, 12);"
                                   "    border-radius: 10px;"
                                   "    padding: 8px 15px;"
                                   "}"
                                   "QToolButton:hover {"
                                   "    background-color: rgb(250, 240, 225);"
                                   "}"
                                   "QToolButton:pressed {"
                                   "    background-color: rgb(230, 210, 185);"
                                   "}");
            }
        }
    }
}

void GameWindow::on_stopBtn_clicked()
{
    emit goResultPage(m_controller.currentPrize(), GameState::Pause);
}
