#ifndef UNDERGRADUATESTUDENT_H
#define UNDERGRADUATESTUDENT_H

#include "Student.h"

/**
 * @class UndergraduateStudent
 * @brief Étudiant en Licence.
 *
 * Bourse : 500 € si GPA >= 3.5, sinon 0 €.
 */
class UndergraduateStudent : public Student {
private:
    string major;  ///< Filière (ex: "Informatique")

public:
    UndergraduateStudent(const string& name, int id, float gpa, const string& major);

    void   display()            const override;
    string getType()            const override { return "Licence"; }
    double computeScholarship() const override;

    string getMajor() const { return major; }
    void   setMajor(const string& m) { major = m; }
};

#endif // UNDERGRADUATESTUDENT_H
