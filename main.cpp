#include <QSplashScreen>
#include <QApplication>
#include "main_window.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QSplashScreen splashScreen(QPixmap("resources/pictures/splashScreen/splashScreen.png"));
    splashScreen.show();

    ui::MainWindow mainWindow;
    splashScreen.finish(&mainWindow);
    mainWindow.showMaximized();

    return app.exec();
}
