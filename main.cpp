/**
 * @file main.cpp
 * @brief Point d'entrée principal — démonstration console de toute la logique.
 *
 * Ce fichier illustre :
 *  - La création des trois types d'étudiants
 *  - Les opérations CRUD via StudentManager
 *  - Le déclenchement et la capture de chaque exception
 *  - La persistance via PersistenceManager
 */

#include <iostream>
#include <memory>
#include "src/models/UndergraduateStudent.h"
#include "src/models/GraduateStudent.h"
#include "src/models/PhDStudent.h"
#include "src/services/StudentManager.h"
#include "src/persistence/PersistenceManager.h"
#include "src/exceptions/StudentExceptions.h"

using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// Helpers d'affichage
// ─────────────────────────────────────────────────────────────────────────────

static void printSeparator(const string& title) {
    cout << "\n" << string(60, '=') << "\n"
         << "  " << title << "\n"
         << string(60, '=') << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// main()
// ─────────────────────────────────────────────────────────────────────────────

int main() {
    StudentManager mgr;

    // ── Partie 1 : Création des étudiants ─────────────────────────────────
    printSeparator("PARTIE 1 — Création et affichage polymorphe");

    auto s1 = make_shared<UndergraduateStudent>("Benali Amira",  1001, 3.8f, "Informatique");
    auto s2 = make_shared<GraduateStudent>     ("Cherif Omar",   1002, 3.5f, "Intelligence Artificielle");
    auto s3 = make_shared<PhDStudent>          ("Meziane Riad",  1003, 3.9f, "Dr. Hadj", 2);
    auto s4 = make_shared<UndergraduateStudent>("Tlemcani Sara", 1004, 2.8f, "Mathematiques");
    auto s5 = make_shared<GraduateStudent>     ("Bensaid Karim", 1005, 3.1f, "Cybersecurite");

    s1->display();
    s2->display();
    s3->display();
    s4->display();
    s5->display();

    // ── Partie 2 : CRUD et tri ─────────────────────────────────────────────
    printSeparator("PARTIE 2 — CRUD + STL");

    mgr.add(s1); mgr.add(s2); mgr.add(s3); mgr.add(s4); mgr.add(s5);
    cout << "Nombre d'etudiants : " << mgr.count() << "\n";
    cout << "Moyenne GPA : " << mgr.averageGpa() << "\n\n";

    cout << "--- Tri par GPA (decroissant) ---\n";
    for (auto& s : mgr.sortByGpa())  s->display();

    cout << "\n--- Tri par Nom (alphabetique) ---\n";
    for (auto& s : mgr.sortByName()) s->display();

    cout << "\n--- Recherche partielle 'omar' ---\n";
    for (auto& s : mgr.findByName("omar")) s->display();

    // Statistiques par type
    cout << "\nLicence : " << mgr.countByType("Licence")  << "\n";
    cout << "Master  : " << mgr.countByType("Master")   << "\n";
    cout << "Doctorat: " << mgr.countByType("Doctorat") << "\n";

    // ── Partie 3 : Exceptions ─────────────────────────────────────────────
    printSeparator("PARTIE 3 — Gestion des exceptions");

    // ID invalide
    try {
        auto bad = make_shared<UndergraduateStudent>("Test", 42, 3.0f, "Info");
        mgr.add(bad);
    } catch (const InvalidIDException& e) {
        cerr << "[ERREUR ID]    " << e.what() << "\n";
    }

    // GPA invalide
    try {
        auto bad = make_shared<UndergraduateStudent>("Test", 1099, 5.5f, "Info");
        mgr.add(bad);
    } catch (const InvalidGradeException& e) {
        cerr << "[ERREUR GPA]   " << e.what() << "\n";
    }

    // ID dupliqué
    try {
        auto dup = make_shared<GraduateStudent>("Doublon", 1001, 3.0f, "Test");
        mgr.add(dup);
    } catch (const DuplicateIDException& e) {
        cerr << "[DOUBLON ID]   " << e.what() << "\n";
    }

    // Étudiant introuvable (suppression)
    try {
        mgr.remove(9999);
    } catch (const StudentNotFoundException& e) {
        cerr << "[INTROUVABLE]  " << e.what() << "\n";
    }

    // Étudiant introuvable (recherche)
    try {
        mgr.findById(8888);
    } catch (const StudentNotFoundException& e) {
        cerr << "[INTROUVABLE]  " << e.what() << "\n";
    }

    // ── Partie 4 : Persistance ─────────────────────────────────────────────
    printSeparator("PARTIE 4 — Persistance");

    const string path = "data/students.txt";
    PersistenceManager::save(mgr, path);
    cout << "Sauvegarde effectuee dans : " << path << "\n";

    StudentManager mgr2;
    PersistenceManager::load(mgr2, path);
    cout << "Rechargement reussi — " << mgr2.count() << " etudiant(s) charges.\n\n";
    for (auto& [id, s] : mgr2.getAll()) s->display();

    cout << "\n[OK] Toutes les parties console validees.\n";
    return 0;
}
