#include "PhDStudent.h"
#include <iostream>
#include <iomanip>

PhDStudent::PhDStudent(const string& name, int id, float gpa,
                       const string& supervisor, int yearOfStudy)
    : Student(name, id, gpa), supervisor(supervisor), yearOfStudy(yearOfStudy)
{}

void PhDStudent::display() const {
    cout << fixed << setprecision(1);
    cout << "[Doctorat] ID: " << id
         << " | Nom: "        << name
         << " | GPA: "        << gpa
         << " | Directeur: "  << supervisor
         << " | Annee: "      << yearOfStudy
         << " | Bourse: "     << computeScholarship() << " EUR"
         << endl;
}
