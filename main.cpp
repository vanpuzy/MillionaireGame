#include "GameWindow.h"
#include "mainwindow.h"
#include <QApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString serverName = "MyUniqueAiTrieuPhuApp";

    // Kiểm tra xem đã có instance khác chạy chưa
    QLocalSocket socket;
    socket.connectToServer(serverName);
    if (socket.waitForConnected(500)) {
        QMessageBox::information(nullptr, "Info", "Ứng dụng đã chạy!");
        return 0; // Nếu đã chạy, thoát
    }

    // Nếu chưa có instance khác, tạo server để "khóa"
    QLocalServer server;
    server.listen(serverName);

    MainWindow w;
    w.showFullScreen();

    return a.exec();
}
