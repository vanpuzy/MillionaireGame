#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QSoundEffect>
#include "models/GameState.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum class PageId:int{
    IdlPage = 0,
    PrizePage = 1,
    GamePage,
    ResultPage
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void change_stack_page(PageId pageId);
    void initSounds();
protected:
    void keyPressEvent(QKeyEvent *event);
public slots:
    void on_playbutton_clicked();
    void on_go_result_page(QString prize, GameState);
private:
    Ui::MainWindow *ui;
    QSoundEffect* letPlaySound;

};
#endif // MAINWINDOW_H
