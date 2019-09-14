#include <QSplashScreen>
#include <QApplication>
#include "main_window.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QSplashScreen splashScreen(QPixmap("resources/pictures/splashScreen/splashScreen.png"));
    splashScreen.show();

    ui::MainWindow::getInstance();//the first: creating object and initialization of static ui::MainWindow instance
    ui::MainWindow::getInstance()->initMainWindow();//the second: using this function instead of constructor, because recursive call of ui::MainWindow::getInstance() can be possible, while static ui::MainWindow instance is not creating in constructor
    splashScreen.finish(ui::MainWindow::getInstance());
    ui::MainWindow::getInstance()->showMaximized();

    return app.exec();
}
