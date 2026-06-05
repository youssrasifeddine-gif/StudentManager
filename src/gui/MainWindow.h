#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QStatusBar>
#include <memory>
#include "../services/StudentManager.h"
using namespace std;

/**
 * @class MainWindow
 * @brief Fenêtre principale de l'application Qt.
 *
 * Affiche la liste des étudiants dans un QTableWidget avec
 * recherche en temps réel et boutons CRUD.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void onAddStudent();
    void onEditStudent();
    void onDeleteStudent();
    void onRefresh();
    void onSearchTextChanged(const QString& text);

private:
    // ── Widgets ───────────────────────────────────────────────────────────────
    QTableWidget* tableWidget;
    QLineEdit*    searchBar;
    QPushButton*  btnAdd;
    QPushButton*  btnEdit;
    QPushButton*  btnDelete;
    QPushButton*  btnRefresh;

    // ── Données ───────────────────────────────────────────────────────────────
    shared_ptr<StudentManager> manager;
    QString                    dataFilePath;

    // ── Helpers ───────────────────────────────────────────────────────────────
    void setupUi();
    void populateTable(const vector<shared_ptr<Student>>& students);
    void refreshTable();
    void updateStatusBar();
    int  getSelectedId() const;  ///< Retourne -1 si aucune ligne sélectionnée
    void saveData();
};

#endif // MAINWINDOW_H
