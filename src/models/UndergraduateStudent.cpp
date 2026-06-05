#include "UndergraduateStudent.h"
#include <iostream>
#include <iomanip>

UndergraduateStudent::UndergraduateStudent(const string& name, int id, float gpa,
                                           const string& major)
    : Student(name, id, gpa), major(major)
{}

double UndergraduateStudent::computeScholarship() const {
    return (gpa >= 3.5f) ? 500.0 : 0.0;
}

void UndergraduateStudent::display() const {
    cout << fixed << setprecision(1);
    cout << "[Licence]  ID: " << id
         << " | Nom: "     << name
         << " | GPA: "     << gpa
         << " | Filiere: " << major
         << " | Bourse: "  << computeScholarship() << " EUR"
         << endl;
}
