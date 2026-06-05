/**
 * @file main_gui.cpp
 * @brief Point d'entrée de l'application Qt.
 */

#include <QApplication>
#include <QDir>
#include "src/gui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Gestion des Étudiants");
    app.setApplicationVersion("1.0");

    // S'assurer que le dossier data/ existe
    QDir().mkpath("data");

    MainWindow w;
    w.show();
    return app.exec();
}
