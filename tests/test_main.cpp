/**
 * @file test_main.cpp
 * @brief Tests unitaires couvrant tous les composants du système.
 *
 * Structure :
 *   - Chaque test est une fonction retournant bool (true = succès).
 *   - Un compteur global affiche le bilan à la fin.
 *
 * Cas couverts :
 *   - Création des 3 types et vérification getType() / computeScholarship()
 *   - CRUD complet sur StudentManager
 *   - Tri GPA décroissant et tri alphabétique
 *   - Déclenchement et capture de chaque exception
 *   - Persistance : save → clear → load → comparer
 */

#include <iostream>
#include <cassert>
#include <sstream>
#include <memory>
#include <cmath>
#include "src/models/UndergraduateStudent.h"
#include "src/models/GraduateStudent.h"
#include "src/models/PhDStudent.h"
#include "src/services/StudentManager.h"
#include "src/persistence/PersistenceManager.h"
#include "src/exceptions/StudentExceptions.h"
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// Infrastructure de test
// ─────────────────────────────────────────────────────────────────────────────

static int passed = 0, failed = 0;

#define RUN_TEST(fn) \
    do { \
        cout << "  [TEST] " #fn " ... "; \
        if (fn()) { cout << "PASS\n"; ++passed; } \
        else      { cout << "FAIL\n"; ++failed; } \
    } while(0)

static constexpr double EPS = 1e-6;

// ─────────────────────────────────────────────────────────────────────────────
// Partie 1 — Modèles
// ─────────────────────────────────────────────────────────────────────────────

bool test_UndergraduateType() {
    auto u = make_shared<UndergraduateStudent>("Benali Amira", 1001, 3.8f, "Informatique");
    return u->getType() == "Licence";
}

bool test_UndergraduateScholarship_high() {
    auto u = make_shared<UndergraduateStudent>("A", 1001, 3.5f, "Info");
    return fabs(u->computeScholarship() - 500.0) < EPS;
}

bool test_UndergraduateScholarship_low() {
    auto u = make_shared<UndergraduateStudent>("A", 1001, 2.9f, "Info");
    return fabs(u->computeScholarship() - 0.0) < EPS;
}

bool test_GraduateType() {
    auto g = make_shared<GraduateStudent>("Cherif Omar", 1002, 3.5f, "IA");
    return g->getType() == "Master";
}

bool test_GraduateScholarship() {
    auto g = make_shared<GraduateStudent>("A", 1002, 3.5f, "IA");
    return fabs(g->computeScholarship() - 3.5 * 300.0) < EPS;
}

bool test_PhDType() {
    auto p = make_shared<PhDStudent>("Meziane", 1003, 3.9f, "Dr. X", 2);
    return p->getType() == "Doctorat";
}

bool test_PhDScholarship() {
    auto p = make_shared<PhDStudent>("A", 1003, 3.0f, "Dr. X", 1);
    return fabs(p->computeScholarship() - 1200.0) < EPS;
}

// ─────────────────────────────────────────────────────────────────────────────
// Partie 2 — CRUD & STL
// ─────────────────────────────────────────────────────────────────────────────

bool test_AddAndCount() {
    StudentManager m;
    m.add(make_shared<UndergraduateStudent>("A", 1001, 3.0f, "Info"));
    m.add(make_shared<GraduateStudent>     ("B", 1002, 2.5f, "IA"));
    return m.count() == 2;
}

bool test_FindById() {
    StudentManager m;
    m.add(make_shared<UndergraduateStudent>("Benali", 1001, 3.0f, "Info"));
    auto s = m.findById(1001);
    return s && s->getName() == "Benali";
}

bool test_FindByName_partial() {
    StudentManager m;
    m.add(make_shared<UndergraduateStudent>("Benali Amira",  1001, 3.0f, "Info"));
    m.add(make_shared<UndergraduateStudent>("Bensaid Karim", 1002, 2.5f, "Math"));
    m.add(make_shared<GraduateStudent>     ("Cherif Omar",   1003, 3.5f, "IA"));
    auto res = m.findByName("ben");
    return res.size() == 2;
}

bool test_Remove() {
    StudentManager m;
    m.add(make_shared<UndergraduateStudent>("A", 1001, 3.0f, "Info"));
    m.remove(1001);
    return m.count() == 0;
}

bool test_Update() {
    StudentManager m;
    m.add(make_shared<UndergraduateStudent>("Old", 1001, 2.0f, "Info"));
    m.update(1001, make_shared<UndergraduateStudent>("New", 1001, 3.5f, "Math"));
    return m.findById(1001)->getName() == "New";
}

bool test_SortByGpa_Descending() {
    StudentManager m;
    m.add(make_shared<UndergraduateStudent>("A", 1001, 2.5f, "Info"));
    m.add(make_shared<GraduateStudent>     ("B", 1002, 3.8f, "IA"));
    m.add(make_shared<PhDStudent>          ("C", 1003, 3.0f, "Dr. X", 1));
    auto sorted = m.sortByGpa();
    return sorted[0]->getGpa() >= sorted[1]->getGpa()
        && sorted[1]->getGpa() >= sorted[2]->getGpa();
}

bool test_SortByName_Alphabetical() {
    StudentManager m;
    m.add(make_shared<UndergraduateStudent>("Zara",  1001, 2.5f, "Info"));
    m.add(make_shared<GraduateStudent>     ("Alice", 1002, 3.0f, "IA"));
    m.add(make_shared<PhDStudent>          ("Marc",  1003, 3.5f, "Dr. X", 1));
    auto sorted = m.sortByName();
    return sorted[0]->getName() == "Alice"
        && sorted[1]->getName() == "Marc"
        && sorted[2]->getName() == "Zara";
}

bool test_AverageGpa() {
    StudentManager m;
    m.add(make_shared<UndergraduateStudent>("A", 1001, 4.0f, "Info"));
    m.add(make_shared<UndergraduateStudent>("B", 1002, 2.0f, "Info"));
    return fabs(m.averageGpa() - 3.0) < EPS;
}

// ─────────────────────────────────────────────────────────────────────────────
// Partie 3 — Exceptions
// ─────────────────────────────────────────────────────────────────────────────

bool test_InvalidIDException() {
    try {
        make_shared<UndergraduateStudent>("X", 42, 3.0f, "Info");
        return false;
    } catch (const InvalidIDException&) { return true; }
}

bool test_InvalidGradeException() {
    try {
        make_shared<UndergraduateStudent>("X", 1001, 5.5f, "Info");
        return false;
    } catch (const InvalidGradeException&) { return true; }
}

bool test_InvalidGradeException_negative() {
    try {
        make_shared<GraduateStudent>("X", 1001, -0.1f, "IA");
        return false;
    } catch (const InvalidGradeException&) { return true; }
}

bool test_DuplicateIDException() {
    StudentManager m;
    m.add(make_shared<UndergraduateStudent>("A", 1001, 3.0f, "Info"));
    try {
        m.add(make_shared<UndergraduateStudent>("B", 1001, 2.0f, "Math"));
        return false;
    } catch (const DuplicateIDException&) { return true; }
}

bool test_StudentNotFoundException_remove() {
    StudentManager m;
    try {
        m.remove(9999);
        return false;
    } catch (const StudentNotFoundException&) { return true; }
}

bool test_StudentNotFoundException_find() {
    StudentManager m;
    try {
        m.findById(8888);
        return false;
    } catch (const StudentNotFoundException&) { return true; }
}

bool test_SetGpaInvalid() {
    auto s = make_shared<UndergraduateStudent>("A", 1001, 3.0f, "Info");
    try {
        s->setGpa(4.5f);
        return false;
    } catch (const InvalidGradeException&) { return true; }
}

// ─────────────────────────────────────────────────────────────────────────────
// Partie 4 — Persistance
// ─────────────────────────────────────────────────────────────────────────────

bool test_PersistenceSaveLoad() {
    StudentManager m;
    m.add(make_shared<UndergraduateStudent>("Benali Amira",  1001, 3.8f, "Informatique"));
    m.add(make_shared<GraduateStudent>     ("Cherif Omar",   1002, 3.5f, "IA"));
    m.add(make_shared<PhDStudent>          ("Meziane Riad",  1003, 3.9f, "Dr. Hadj", 2));

    const string path = "data/test_students.txt";
    PersistenceManager::save(m, path);

    StudentManager m2;
    PersistenceManager::load(m2, path);

    if (m2.count() != 3) return false;
    auto s = m2.findById(1001);
    return s && s->getName() == "Benali Amira"
             && s->getType() == "Licence"
             && fabs(s->getGpa() - 3.8f) < 0.01f;
}

bool test_PersistenceFileAbsent() {
    // Ne doit pas planter si le fichier n'existe pas
    StudentManager m;
    PersistenceManager::load(m, "data/nonexistent_xyz.txt");
    return m.count() == 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// main()
// ─────────────────────────────────────────────────────────────────────────────

int main() {
    cout << "\n========================================\n";
    cout << "  TESTS UNITAIRES — Gestion Etudiants  \n";
    cout << "========================================\n";

    cout << "\n-- Modeles --\n";
    RUN_TEST(test_UndergraduateType);
    RUN_TEST(test_UndergraduateScholarship_high);
    RUN_TEST(test_UndergraduateScholarship_low);
    RUN_TEST(test_GraduateType);
    RUN_TEST(test_GraduateScholarship);
    RUN_TEST(test_PhDType);
    RUN_TEST(test_PhDScholarship);

    cout << "\n-- CRUD & STL --\n";
    RUN_TEST(test_AddAndCount);
    RUN_TEST(test_FindById);
    RUN_TEST(test_FindByName_partial);
    RUN_TEST(test_Remove);
    RUN_TEST(test_Update);
    RUN_TEST(test_SortByGpa_Descending);
    RUN_TEST(test_SortByName_Alphabetical);
    RUN_TEST(test_AverageGpa);

    cout << "\n-- Exceptions --\n";
    RUN_TEST(test_InvalidIDException);
    RUN_TEST(test_InvalidGradeException);
    RUN_TEST(test_InvalidGradeException_negative);
    RUN_TEST(test_DuplicateIDException);
    RUN_TEST(test_StudentNotFoundException_remove);
    RUN_TEST(test_StudentNotFoundException_find);
    RUN_TEST(test_SetGpaInvalid);

    cout << "\n-- Persistance --\n";
    RUN_TEST(test_PersistenceSaveLoad);
    RUN_TEST(test_PersistenceFileAbsent);

    cout << "\n========================================\n";
    cout << "  BILAN : " << passed << " PASS / " << failed << " FAIL\n";
    cout << "========================================\n\n";
    return (failed == 0) ? 0 : 1;
}
