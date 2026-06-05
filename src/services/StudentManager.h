#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <string>
#include "../models/Student.h"
using namespace std;

/**
 * @class StudentManager
 * @brief Service de gestion des étudiants avec stockage STL.
 *
 * Utilise une std::map<int, shared_ptr<Student>> pour indexer
 * les étudiants par leur identifiant unique.
 */
class StudentManager {
private:
    map<int, shared_ptr<Student>> students;  ///< Stockage : ID → Étudiant

public:
    // ── CRUD ─────────────────────────────────────────────────────────────────
    /// @throws DuplicateIDException si l'ID existe déjà.
    void add(shared_ptr<Student> s);

    /// @throws StudentNotFoundException si l'ID est absent.
    void remove(int id);

    /// @throws StudentNotFoundException si l'ID est absent.
    void update(int id, shared_ptr<Student> updated);

    // ── Recherche ─────────────────────────────────────────────────────────────
    /// @throws StudentNotFoundException si l'ID est absent.
    shared_ptr<Student> findById(int id) const;

    /// Recherche partielle insensible à la casse.
    vector<shared_ptr<Student>> findByName(const string& n) const;

    // ── Tri ───────────────────────────────────────────────────────────────────
    vector<shared_ptr<Student>> sortByGpa()  const;  ///< Ordre décroissant
    vector<shared_ptr<Student>> sortByName() const;  ///< Ordre alphabétique

    // ── Statistiques ──────────────────────────────────────────────────────────
    double averageGpa()   const;
    int    countByType(const string& type) const;
    int    count()        const { return static_cast<int>(students.size()); }

    const map<int, shared_ptr<Student>>& getAll() const { return students; }

    /// Vide complètement la liste (utile pour le rechargement).
    void clear() { students.clear(); }
};

#endif // STUDENTMANAGER_H
