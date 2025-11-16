#ifndef PRIZEITEMWIDGET_H
#define PRIZEITEMWIDGET_H

#include <QIcon>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class PrizeItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PrizeItemWidget(int level, const QString &text, QWidget *parent = nullptr)
            : QWidget(parent)
        {
            QHBoxLayout *layout = new QHBoxLayout(this);
            layout->setContentsMargins(5,2,5,2);  // padding xung quanh item
            layout->setSpacing(10);               // khoảng cách số và text

            // Label số (thay icon)
            QLabel *numberLabel = new QLabel(QString::number(level), this);
            numberLabel->setStyleSheet("color: yellow; font-weight: bold; font-size: 16px;");
            layout->addWidget(numberLabel);

            // Label text
            QLabel *textLabel = new QLabel(text, this);
            textLabel->setStyleSheet("color: white; font-weight: bold;");
            layout->addWidget(textLabel);

            layout->addStretch(); // đẩy text về trái

            // border + radius
            setStyleSheet("background-color: rgba(0,0,0,50); border: 2px solid white; border-radius: 5px;");
        }
};


#endif // PRIZEITEMWIDGET_H
