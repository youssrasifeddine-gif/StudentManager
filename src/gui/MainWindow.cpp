#include "MainWindow.h"
#include "StudentDialog.h"
#include "../persistence/PersistenceManager.h"
#include "../exceptions/StudentExceptions.h"
#include "../models/UndergraduateStudent.h"
#include "../models/GraduateStudent.h"
#include "../models/PhDStudent.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>
#include <iomanip>
#include <sstream>

// ─────────────────────────────────────────────────────────────────────────────
// Construction
// ─────────────────────────────────────────────────────────────────────────────

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      manager(make_shared<StudentManager>()),
      dataFilePath("data/students.txt")
{
    setWindowTitle("Système de Gestion des Étudiants");
    resize(900, 600);
    setupUi();

    // Chargement automatique au démarrage
    PersistenceManager::load(*manager, dataFilePath.toStdString());
    refreshTable();
}

// ─────────────────────────────────────────────────────────────────────────────
// setupUi()
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::setupUi() {
    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* mainLayout = new QVBoxLayout(central);

    // ── Barre de recherche ─────────────────────────────────────────────────
    auto* searchLayout = new QHBoxLayout;
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Rechercher par nom...");
    searchLayout->addWidget(new QLabel("Recherche :"));
    searchLayout->addWidget(searchBar);
    mainLayout->addLayout(searchLayout);

    // ── Tableau ────────────────────────────────────────────────────────────
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(6);
    tableWidget->setHorizontalHeaderLabels(
        {"ID", "Nom", "Type", "GPA", "Bourse (EUR)", "Détail"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setAlternatingRowColors(true);
    mainLayout->addWidget(tableWidget);

    // ── Boutons CRUD ───────────────────────────────────────────────────────
    auto* btnLayout = new QHBoxLayout;
    btnAdd     = new QPushButton("+ Ajouter",    this);
    btnEdit    = new QPushButton("✎ Modifier",   this);
    btnDelete  = new QPushButton("✕ Supprimer",  this);
    btnRefresh = new QPushButton("↺ Rafraîchir", this);
    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnEdit);
    btnLayout->addWidget(btnDelete);
    btnLayout->addStretch();
    btnLayout->addWidget(btnRefresh);
    mainLayout->addLayout(btnLayout);

    // ── Barre de statut ────────────────────────────────────────────────────
    statusBar()->showMessage("Prêt");

    // ── Connexions ─────────────────────────────────────────────────────────
    connect(btnAdd,     &QPushButton::clicked,  this, &MainWindow::onAddStudent);
    connect(btnEdit,    &QPushButton::clicked,  this, &MainWindow::onEditStudent);
    connect(btnDelete,  &QPushButton::clicked,  this, &MainWindow::onDeleteStudent);
    connect(btnRefresh, &QPushButton::clicked,  this, &MainWindow::onRefresh);
    connect(searchBar,  &QLineEdit::textChanged,
            this, &MainWindow::onSearchTextChanged);

    // Style de base
    setStyleSheet(R"(
        QMainWindow { background: #f4f6f9; }
        QTableWidget { background: white; font-size: 13px; }
        QTableWidget::item:selected { background: #3498db; color: white; }
        QPushButton {
            padding: 6px 16px; border-radius: 4px; font-weight: bold;
            background: #2c3e50; color: white; border: none;
        }
        QPushButton:hover  { background: #3498db; }
        QPushButton:pressed{ background: #1abc9c; }
        QLineEdit {
            padding: 5px 8px; border: 1px solid #bdc3c7;
            border-radius: 4px; font-size: 13px;
        }
        QStatusBar { background: #ecf0f1; color: #2c3e50; font-style: italic; }
    )");
}

// ─────────────────────────────────────────────────────────────────────────────
// Peuplement du tableau
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::populateTable(const vector<shared_ptr<Student>>& students) {
    tableWidget->setRowCount(0);

    for (const auto& s : students) {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(s->getId())));
        tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(s->getName())));
        tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(s->getType())));
        tableWidget->setItem(row, 3, new QTableWidgetItem(
            QString::number(static_cast<double>(s->getGpa()), 'f', 2)));
        tableWidget->setItem(row, 4, new QTableWidgetItem(
            QString::number(s->computeScholarship(), 'f', 0)));

        // Colonne Détail : infos supplémentaires selon le type
        QString detail;
        if (s->getType() == "Licence") {
            auto u = dynamic_pointer_cast<UndergraduateStudent>(s);
            detail = QString::fromStdString(u->getMajor());
        } else if (s->getType() == "Master") {
            auto g = dynamic_pointer_cast<GraduateStudent>(s);
            detail = QString::fromStdString(g->getResearchTopic());
        } else if (s->getType() == "Doctorat") {
            auto p = dynamic_pointer_cast<PhDStudent>(s);
            detail = QString("Dir: %1 | An: %2")
                         .arg(QString::fromStdString(p->getSupervisor()))
                         .arg(p->getYearOfStudy());
        }
        tableWidget->setItem(row, 5, new QTableWidgetItem(detail));
    }

    updateStatusBar();
}

void MainWindow::refreshTable() {
    vector<shared_ptr<Student>> all;
    for (const auto& [id, s] : manager->getAll())
        all.push_back(s);
    populateTable(all);
}

void MainWindow::updateStatusBar() {
    QString msg = QString("Total : %1 étudiant(s)  |  Moyenne GPA : %2")
                      .arg(manager->count())
                      .arg(QString::number(manager->averageGpa(), 'f', 2));
    statusBar()->showMessage(msg);
}

// ─────────────────────────────────────────────────────────────────────────────
// Slots
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::onAddStudent() {
    StudentDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        try {
            manager->add(dlg.getStudent());
            saveData();
            refreshTable();
            statusBar()->showMessage("Étudiant ajouté avec succès.", 3000);
        } catch (const DuplicateIDException& e) {
            QMessageBox::critical(this, "Erreur ID", QString::fromStdString(e.what()));
        } catch (const InvalidGradeException& e) {
            QMessageBox::warning(this, "Erreur GPA", QString::fromStdString(e.what()));
        } catch (const InvalidIDException& e) {
            QMessageBox::warning(this, "Erreur ID", QString::fromStdString(e.what()));
        } catch (const exception& e) {
            QMessageBox::critical(this, "Erreur", QString::fromStdString(e.what()));
        }
    }
}

void MainWindow::onEditStudent() {
    int id = getSelectedId();
    if (id == -1) {
        QMessageBox::information(this, "Sélection", "Veuillez sélectionner un étudiant.");
        return;
    }
    try {
        auto s = manager->findById(id);
        StudentDialog dlg(this, s);
        if (dlg.exec() == QDialog::Accepted) {
            try {
                manager->update(id, dlg.getStudent());
                saveData();
                refreshTable();
                statusBar()->showMessage("Étudiant modifié avec succès.", 3000);
            } catch (const exception& e) {
                QMessageBox::critical(this, "Erreur", QString::fromStdString(e.what()));
            }
        }
    } catch (const StudentNotFoundException& e) {
        QMessageBox::warning(this, "Introuvable", QString::fromStdString(e.what()));
    }
}

void MainWindow::onDeleteStudent() {
    int id = getSelectedId();
    if (id == -1) {
        QMessageBox::information(this, "Sélection", "Veuillez sélectionner un étudiant.");
        return;
    }
    auto reply = QMessageBox::question(this, "Confirmer la suppression",
                                       QString("Supprimer l'étudiant ID %1 ?").arg(id),
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        try {
            manager->remove(id);
            saveData();
            refreshTable();
            statusBar()->showMessage("Étudiant supprimé.", 3000);
        } catch (const StudentNotFoundException& e) {
            QMessageBox::warning(this, "Introuvable", QString::fromStdString(e.what()));
        }
    }
}

void MainWindow::onRefresh() {
    refreshTable();
    statusBar()->showMessage("Liste actualisée.", 2000);
}

void MainWindow::onSearchTextChanged(const QString& text) {
    if (text.isEmpty()) {
        refreshTable();
        return;
    }
    auto results = manager->findByName(text.toStdString());
    populateTable(results);
}

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

int MainWindow::getSelectedId() const {
    auto selected = tableWidget->selectedItems();
    if (selected.isEmpty()) return -1;
    int row = tableWidget->currentRow();
    auto* item = tableWidget->item(row, 0);
    return item ? item->text().toInt() : -1;
}

void MainWindow::saveData() {
    PersistenceManager::save(*manager, dataFilePath.toStdString());
}
