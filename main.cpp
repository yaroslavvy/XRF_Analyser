#include <QSplashScreen>
#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QSplashScreen splashScreen(QPixmap("resources/pictures/splashScreen/splashScreen.png"));
    splashScreen.show();

    MainWindow mainWindow;
    splashScreen.finish(&mainWindow);
    mainWindow.showMaximized();

    return app.exec();
}
