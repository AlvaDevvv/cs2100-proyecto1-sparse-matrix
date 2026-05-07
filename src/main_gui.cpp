#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("SparseSpreadsheet");
    app.setApplicationVersion("1.0");
    app.setStyle("Fusion");

    MainWindow w;
    w.show();
    return app.exec();
}
