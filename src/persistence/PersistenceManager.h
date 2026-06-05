#ifndef PERSISTENCEMANAGER_H
#define PERSISTENCEMANAGER_H

#include <string>
#include "../services/StudentManager.h"
using namespace std;

/**
 * @class PersistenceManager
 * @brief Gère la sauvegarde et le chargement des étudiants dans un fichier.
 *
 * Format d'une ligne :
 *   Licence|1001|Benali Amira|3.8|Informatique
 *   Master|1002|Cherif Omar|3.5|Intelligence Artificielle
 *   Doctorat|1003|Meziane Riad|3.9|Dr. Hadj|2
 */
class PersistenceManager {
public:
    /// Sauvegarde tous les étudiants dans le fichier spécifié.
    static void save(const StudentManager& mgr, const string& path);

    /// Charge les étudiants depuis le fichier dans mgr.
    /// Ignore silencieusement les lignes corrompues.
    static void load(StudentManager& mgr, const string& path);
};

#endif // PERSISTENCEMANAGER_H
