#ifndef STUDENT_H
#define STUDENT_H

#include <string>
using namespace std;

/**
 * @class Student
 * @brief Classe de base abstraite représentant un étudiant.
 *
 * Définit l'interface commune pour tous les types d'étudiants.
 * Utilise le polymorphisme via des méthodes virtuelles pures.
 */
class Student {
protected:
    string name;  ///< Nom complet de l'étudiant
    int    id;    ///< Identifiant unique (>= 1000)
    float  gpa;   ///< Note globale entre 0.0 et 4.0

public:
    /**
     * @brief Constructeur avec validation.
     * @throws InvalidIDException si id < 1000
     * @throws InvalidGradeException si gpa hors [0.0, 4.0]
     */
    Student(const string& name, int id, float gpa);

    // ── Méthodes pures : chaque classe dérivée DOIT les implémenter ──────────
    virtual void   display()             const = 0;
    virtual string getType()             const = 0;
    virtual double computeScholarship()  const = 0;

    // ── Accesseurs ────────────────────────────────────────────────────────────
    string getName() const { return name; }
    int    getId()   const { return id;   }
    float  getGpa()  const { return gpa;  }

    // ── Mutateurs avec validation ─────────────────────────────────────────────
    void setName(const string& n);
    /// @throws InvalidGradeException si g hors [0.0, 4.0]
    void setGpa(float g);

    virtual ~Student() {}
};

#endif // STUDENT_H
