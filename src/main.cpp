#include "include/ui/mainwindow.h"
#include "include/sender/filetransferserviceprovider.h"

#include <memory>
#include <QApplication>
#include <QGuiApplication>
#include <QLoggingCategory>


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

    QLoggingCategory::setFilterRules("qt.timer.debug=true");


    return mainApplication.exec();
}
