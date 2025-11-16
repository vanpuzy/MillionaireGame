#ifndef PRIZELADDERWIDGET_H
#define PRIZELADDERWIDGET_H

#include <QWidget>
#include <QWidget>
#include <QListWidget>
#include <QJsonArray>

namespace Ui {
class PrizeLadderWidget;
}

class PrizeLadderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PrizeLadderWidget(QWidget *parent = nullptr);
    ~PrizeLadderWidget();

    void loadPrizesFromJson(const QString &jsonPath);

    void adjustRowHeight();
    void highlightCurrentRow(int row);
    void centerResultFrame();
public slots:
     void setCurrentLevel(int level);
public :

signals:
    void playButtonClicked();
protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
private slots:
    void on_playButton_clicked();
private:
    Ui::PrizeLadderWidget *ui;
    QJsonArray prizesArray;
};

#endif // PRIZELADDERWIDGET_H
