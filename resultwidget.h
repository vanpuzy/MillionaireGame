#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include "models/GameState.h"

namespace Ui {
class ResultWidget;
}



class ResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResultWidget(QWidget *parent = nullptr);
    ~ResultWidget();
    void centerResultFrame();
    void showResult(QString prize);
    void setWon(GameState state);
signals:
    void playButtonClicked();
protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_replayBtn_clicked();

private:
    Ui::ResultWidget *ui;
};

#endif // RESULTWIDGET_H
