#include "StudentManager.h"
#include "../exceptions/StudentExceptions.h"
#include <cctype>
#include <numeric>

// ── CRUD ──────────────────────────────────────────────────────────────────────

void StudentManager::add(shared_ptr<Student> s) {
    if (students.count(s->getId()))
        throw DuplicateIDException(s->getId());
    students[s->getId()] = s;
}

void StudentManager::remove(int id) {
    auto it = students.find(id);
    if (it == students.end())
        throw StudentNotFoundException(id);
    students.erase(it);
}

void StudentManager::update(int id, shared_ptr<Student> updated) {
    if (!students.count(id))
        throw StudentNotFoundException(id);
    students[id] = updated;
}

// ── Recherche ─────────────────────────────────────────────────────────────────

shared_ptr<Student> StudentManager::findById(int id) const {
    auto it = students.find(id);
    if (it == students.end())
        throw StudentNotFoundException(id);
    return it->second;
}

vector<shared_ptr<Student>> StudentManager::findByName(const string& n) const {
    // Convertit le motif en minuscules pour une recherche insensible à la casse
    string lowerPattern = n;
    transform(lowerPattern.begin(), lowerPattern.end(),
              lowerPattern.begin(), ::tolower);

    vector<shared_ptr<Student>> results;
    for (const auto& [id, student] : students) {
        string lowerName = student->getName();
        transform(lowerName.begin(), lowerName.end(),
                  lowerName.begin(), ::tolower);
        if (lowerName.find(lowerPattern) != string::npos)
            results.push_back(student);
    }
    return results;
}

// ── Tri ───────────────────────────────────────────────────────────────────────

vector<shared_ptr<Student>> StudentManager::sortByGpa() const {
    vector<shared_ptr<Student>> vec;
    for (const auto& [id, s] : students) vec.push_back(s);

    sort(vec.begin(), vec.end(), [](const shared_ptr<Student>& a,
                                    const shared_ptr<Student>& b) {
        return a->getGpa() > b->getGpa();  // décroissant
    });
    return vec;
}

vector<shared_ptr<Student>> StudentManager::sortByName() const {
    vector<shared_ptr<Student>> vec;
    for (const auto& [id, s] : students) vec.push_back(s);

    sort(vec.begin(), vec.end(), [](const shared_ptr<Student>& a,
                                    const shared_ptr<Student>& b) {
        string na = a->getName(), nb = b->getName();
        transform(na.begin(), na.end(), na.begin(), ::tolower);
        transform(nb.begin(), nb.end(), nb.begin(), ::tolower);
        return na < nb;
    });
    return vec;
}

// ── Statistiques ──────────────────────────────────────────────────────────────

double StudentManager::averageGpa() const {
    if (students.empty()) return 0.0;

    double sum = 0.0;
    for (const auto& [id, s] : students)
        sum += s->getGpa();
    return sum / static_cast<double>(students.size());
}

int StudentManager::countByType(const string& type) const {
    return static_cast<int>(
        count_if(students.begin(), students.end(),
                 [&type](const pair<int, shared_ptr<Student>>& p) {
                     return p.second->getType() == type;
                 })
    );
}
