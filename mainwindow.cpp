#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "prizeladderwidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->prizePage, SIGNAL(playButtonClicked()), this, SLOT(on_playbutton_clicked()));
    connect(ui->gamePage, SIGNAL(goResultPage(QString, GameState)), this, SLOT(on_go_result_page(QString, GameState)));
    connect(ui->resultPage, SIGNAL(playButtonClicked()), this, SLOT(on_playbutton_clicked()));
    connect(ui->gamePage, &GameWindow::showPrizePage, this, [this](){
        ui->stackedWidget->setCurrentIndex((int)PageId::PrizePage);
    });

    connect(ui->gamePage, &GameWindow::hidePrizePage, this, [this](){
        ui->stackedWidget->setCurrentIndex((int)PageId::GamePage);
    });

    connect(ui->gamePage, SIGNAL(changeCurrentLevel(int)), ui->prizePage, SLOT(setCurrentLevel(int)));
    ui->stackedWidget->setCurrentIndex((int)PageId::IdlPage);

    initSounds();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::change_stack_page(PageId pageId)
{

}

void MainWindow::initSounds()
{
    letPlaySound = new QSoundEffect(this);
    letPlaySound->setSource(QUrl::fromLocalFile(":/sounds/letplay.wav"));
    letPlaySound->setVolume(0.5);

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        if(ui->stackedWidget->currentIndex() == (int)PageId::IdlPage)
        {
            // Chuyển sang Prize Page
            ui->stackedWidget->setCurrentIndex((int)PageId::PrizePage);
            letPlaySound->play();
        }
        // nếu PrizePage có widget con, có thể gọi init/restart ở đây
    }
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Q) {
        qApp->quit();
    }
    else {
        // Gọi base class để không mất các phím khác
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::on_playbutton_clicked()
{
    ui->stackedWidget->setCurrentIndex((int)PageId::GamePage);
    ui->gamePage->restartGame();
    letPlaySound->stop();
}

void MainWindow::on_go_result_page(QString prize, GameState state)
{
    ui->stackedWidget->setCurrentIndex((int)PageId::ResultPage);
    ui->resultPage->setWon(state);
    ui->resultPage->showResult(prize);
}

