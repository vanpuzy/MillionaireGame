#include "resultwidget.h"
#include "ui_resultwidget.h"

ResultWidget::ResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWidget)
{
    ui->setupUi(this);
    centerResultFrame();
}

ResultWidget::~ResultWidget()
{
    delete ui;
}

void ResultWidget::centerResultFrame()
{
    QWidget *parent = ui->resultFrame->parentWidget();
    int pw = parent->width();
    int ph = parent->height();

    int fw = ui->resultFrame->width();
    int fh = ui->resultFrame->height();

    ui->resultFrame->move((pw - fw) / 2, (ph - fh) / 2);
}

void ResultWidget::showResult(QString prize)
{
    ui->totalMoneyLabel->setText("Số tiền thưởng:" + prize);
}

void ResultWidget::setWon(GameState state)
{
    if(state == GameState::Won)
    {
        ui->congurationLabel->setText("Chúc mừng bạn đã chiến thắng");
        ui->resultLabel->setText("Người chiến thắng!");
        ui->resultImage->setPixmap(QPixmap(":/images/cup.png"));
    }
    else if(state == GameState::Pause)
    {
        ui->congurationLabel->setText("Bạn đã dừng cuộc chơi!");
        ui->resultLabel->setText("Người chiến thằng!");
        ui->resultImage->setPixmap(QPixmap(":/images/pause.png"));
    }
    else if(state == GameState::Lose)
    {
        ui->congurationLabel->setText("Câu trả lời sai");
        ui->resultLabel->setText("Người thua cuộc!");
        ui->resultImage->setPixmap(QPixmap(":/images/loser.png"));
    }
}

void ResultWidget::resizeEvent(QResizeEvent *event)
{

    centerResultFrame();
}

void ResultWidget::on_replayBtn_clicked()
{
    emit playButtonClicked();
}
