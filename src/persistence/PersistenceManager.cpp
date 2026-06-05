#include "PersistenceManager.h"
#include "../models/UndergraduateStudent.h"
#include "../models/GraduateStudent.h"
#include "../models/PhDStudent.h"
#include "../exceptions/StudentExceptions.h"
#include <fstream>
#include <sstream>
#include <iostream>

// ─────────────────────────────────────────────────────────────────────────────
// Helpers internes
// ─────────────────────────────────────────────────────────────────────────────

/** Découpe une chaîne selon un délimiteur et retourne un vecteur de tokens. */
static vector<string> split(const string& line, char delim) {
    vector<string> tokens;
    stringstream   ss(line);
    string         token;
    while (getline(ss, token, delim))
        tokens.push_back(token);
    return tokens;
}

// ─────────────────────────────────────────────────────────────────────────────
// save()
// ─────────────────────────────────────────────────────────────────────────────

void PersistenceManager::save(const StudentManager& mgr, const string& path) {
    ofstream ofs(path);
    if (!ofs.is_open()) {
        cerr << "[PersistenceManager] Impossible d'ouvrir le fichier en ecriture : "
             << path << endl;
        return;
    }

    for (const auto& [id, s] : mgr.getAll()) {
        const string& type = s->getType();
        ofs << type << "|" << s->getId() << "|" << s->getName()
            << "|" << s->getGpa();

        if (type == "Licence") {
            auto u = dynamic_pointer_cast<UndergraduateStudent>(s);
            ofs << "|" << u->getMajor();
        } else if (type == "Master") {
            auto g = dynamic_pointer_cast<GraduateStudent>(s);
            ofs << "|" << g->getResearchTopic();
        } else if (type == "Doctorat") {
            auto p = dynamic_pointer_cast<PhDStudent>(s);
            ofs << "|" << p->getSupervisor() << "|" << p->getYearOfStudy();
        }

        ofs << "\n";
    }
    ofs.close();
}

// ─────────────────────────────────────────────────────────────────────────────
// load()
// ─────────────────────────────────────────────────────────────────────────────

void PersistenceManager::load(StudentManager& mgr, const string& path) {
    ifstream ifs(path);
    if (!ifs.is_open()) {
        // Fichier absent → démarrage avec liste vide, sans plantage
        return;
    }

    string line;
    int    lineNumber = 0;
    while (getline(ifs, line)) {
        ++lineNumber;
        if (line.empty()) continue;

        try {
            auto tokens = split(line, '|');

            if (tokens.size() < 4)
                throw runtime_error("Ligne incomplete");

            const string& type = tokens[0];
            int   id  = stoi(tokens[1]);
            string nm = tokens[2];
            float  gp = stof(tokens[3]);

            if (type == "Licence") {
                if (tokens.size() < 5) throw runtime_error("Champ manquant");
                mgr.add(make_shared<UndergraduateStudent>(nm, id, gp, tokens[4]));

            } else if (type == "Master") {
                if (tokens.size() < 5) throw runtime_error("Champ manquant");
                mgr.add(make_shared<GraduateStudent>(nm, id, gp, tokens[4]));

            } else if (type == "Doctorat") {
                if (tokens.size() < 6) throw runtime_error("Champ manquant");
                int year = stoi(tokens[5]);
                mgr.add(make_shared<PhDStudent>(nm, id, gp, tokens[4], year));

            } else {
                throw runtime_error("Type inconnu : " + type);
            }

        } catch (const exception& e) {
            cerr << "[PersistenceManager] Ligne " << lineNumber
                 << " ignoree (" << e.what() << "): " << line << endl;
        }
    }
    ifs.close();
}
