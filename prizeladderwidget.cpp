#include "prizeladderwidget.h"
#include "ui_prizeladderwidget.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QListWidgetItem>
#include <QLabel>
#include <QDebug>
#include <algorithm>

PrizeLadderWidget::PrizeLadderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrizeLadderWidget)
{
    ui->setupUi(this);

    // Tắt scrollbar
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setSpacing(5);

    // Load prizes
    loadPrizesFromJson("prizes.json");
    centerResultFrame();
}

PrizeLadderWidget::~PrizeLadderWidget()
{
    delete ui;
}

void PrizeLadderWidget::setCurrentLevel(int level)
{
    if(prizesArray.isEmpty()) return;

    int rowIndex = prizesArray.size() - 1 - level; // top = cao nhất
    highlightCurrentRow(rowIndex);

//    // Chỉ set current row khi rowIndex hợp lệ
//    if(rowIndex >= 0 && rowIndex < ui->listWidget->count())
//        ui->listWidget->setCurrentRow(rowIndex);
}

void PrizeLadderWidget::highlightCurrentRow(int row)
{
    for(int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        if(i == row)
            item->setBackgroundColor(QColor(255,255,0,120));
        else
            item->setBackgroundColor(QColor(0,0,0,50));
    }
}

void PrizeLadderWidget::centerResultFrame()
{
    QWidget *parent = ui->frame->parentWidget();
       int pw = parent->width();
       int ph = parent->height();

       int fw = ui->frame->width();
       int fh = ui->frame->height();

       ui->frame->move((pw - fw) / 2, (ph - fh) / 2);
}

void PrizeLadderWidget::loadPrizesFromJson(const QString &jsonPath)
{
    QFile file(jsonPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open JSON file:" << jsonPath;
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    prizesArray = obj["prizes"].toArray();

    ui->listWidget->clear();
    int total = prizesArray.size();

    for(int i = 0; i < total; ++i) {
        QString prizeText = prizesArray[i].toObject()["text"].toString();
        int level = total - i;

        // Bôi đậm mốc 5,10,15
        QString htmlText;
        if(level == 5 || level == 10 || level == 15) {
            htmlText = QString("<b><span style='color:yellow;font-family:Arial; font-size:17pt;'>%1.</span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    "
                               "<span style='margin-left:15px;color:white;font-family:Arial; font-size:20pt;'>%2</span></b>")
                               .arg(level)
                               .arg(prizeText);
        } else {
            htmlText = QString("<span style='color:yellow;font-family:Arial; font-size:13pt;'>%1.</span> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;   "
                               "<span style='margin-left:15px;color:white;font-family:Arial; font-size:16pt;'>%2</span>")
                               .arg(level)
                               .arg(prizeText);
        }

        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        QLabel *label = new QLabel;
        label->setText(htmlText);
        label->setTextFormat(Qt::RichText);
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        label->setStyleSheet(
            "background-color: rgba(0,0,0,50);"
            "border: 2px solid white;"
            "border-radius: 5px;"
            "padding: 5px;"
        );

        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, label);
    }
}

void PrizeLadderWidget::adjustRowHeight()
{
    int rowCount = ui->listWidget->count();
    if(rowCount == 0) return;

    int spacing = ui->listWidget->spacing() + 5;
    int horizontalMargin = 50;

    // Tính rowHeight, bảo đảm ≥ 1
    int rowHeight = std::max(1, (ui->listWidget->height() - spacing*(rowCount-1)) / rowCount);
    int rowWidth  = ui->listWidget->width() - 2 * horizontalMargin;

    for(int i = 0; i < rowCount; ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        item->setSizeHint(QSize(rowWidth, rowHeight));

        QWidget *w = ui->listWidget->itemWidget(item);
        if(w) {
            w->setMinimumHeight(rowHeight);
            w->setMinimumWidth(rowWidth);
            w->setContentsMargins(horizontalMargin, 0, horizontalMargin, 0);
        }
    }
}

void PrizeLadderWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    adjustRowHeight();
    centerResultFrame();
}

void PrizeLadderWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    // Chỉ sau khi show mới tính height chuẩn
    adjustRowHeight();

    // Đặt level 0, không nhảy scroll
    setCurrentLevel(0);
}


void PrizeLadderWidget::on_playButton_clicked()
{
    emit playButtonClicked();
}
