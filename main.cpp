#include "mainwindow.h"
#include "veniceservice.h"

#include <memory>
#include <QApplication>
#include <QGuiApplication>


/**
 * @brief Create and show the main application window
 * @param argc Argument count
 * @param argv Argument vector
 * @return 0 if application ends correctly
 */
int main(int argc, char *argv[])
{
    QApplication mainApplication(argc, argv);
    MainWindow w;
    w.setMainApplication(&mainApplication);
    w.show();

    return mainApplication.exec();
}
