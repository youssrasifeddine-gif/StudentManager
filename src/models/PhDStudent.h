#ifndef PHDSTUDENT_H
#define PHDSTUDENT_H

#include "Student.h"

/**
 * @class PhDStudent
 * @brief Doctorant.
 *
 * Bourse fixe : 1 200 €/mois.
 */
class PhDStudent : public Student {
private:
    string supervisor;   ///< Nom du directeur de thèse
    int    yearOfStudy;  ///< Année en cours (1, 2, 3, ...)

public:
    PhDStudent(const string& name, int id, float gpa,
               const string& supervisor, int yearOfStudy);

    void   display()            const override;
    string getType()            const override { return "Doctorat"; }
    double computeScholarship() const override { return 1200.0; }

    string getSupervisor()  const { return supervisor; }
    int    getYearOfStudy() const { return yearOfStudy; }
    void   setSupervisor(const string& s) { supervisor = s; }
    void   setYearOfStudy(int y)          { yearOfStudy = y; }
};

#endif // PHDSTUDENT_H
