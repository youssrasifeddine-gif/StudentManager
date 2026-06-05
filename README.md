# StudentManager — Système de Gestion des Étudiants en C++/Qt

> Mini-Projet IMS — Année Universitaire 2025/2026

## Membres du groupe
| Prénom Nom | Rôle principal |
|---|---|
| ABiA Abderahman | Modélisation OOP + Exceptions |
| SIFEDDINE Youssra | GUI Qt + Persistance |

---

## Description

Système complet de gestion d'étudiants développé en **C++17 avec Qt**. Il implémente :

- Une **hiérarchie de classes** polymorphes (Licence, Master, Doctorat)
- Un **service de gestion** STL (`std::map`, `std::sort`, `std::find_if`)
- Des **exceptions personnalisées** héritant de `std::exception`
- Une **persistance fichier** (`|`-separated) avec chargement automatique
- Une **interface graphique Qt** complète avec recherche en temps réel

---

## Structure du projet

```
StudentManager/
├── src/
│   ├── models/          # Student, UndergraduateStudent, GraduateStudent, PhDStudent
│   ├── services/        # StudentManager (logique métier)
│   ├── exceptions/      # StudentExceptions.h
│   ├── persistence/     # PersistenceManager
│   └── gui/             # MainWindow, StudentDialog (Qt)
├── data/
│   └── students.txt     # Données persistantes
├── tests/
│   └── test_main.cpp    # Tests unitaires (sans Qt)
├── main.cpp             # Démo console
├── main_gui.cpp         # Entrée Qt
└── StudentManager.pro   # Fichier projet Qt
```

---

## Compilation

### Application Qt (GUI)

```bash
qmake StudentManager.pro
make
./StudentManager
```

### Démo console (sans Qt)

```bash
g++ -std=c++17 -o demo main.cpp \
    src/models/Student.cpp \
    src/models/UndergraduateStudent.cpp \
    src/models/GraduateStudent.cpp \
    src/models/PhDStudent.cpp \
    src/services/StudentManager.cpp \
    src/persistence/PersistenceManager.cpp \
    -Isrc
./demo
```

### Tests unitaires

```bash
g++ -std=c++17 -o test_runner tests/test_main.cpp \
    src/models/Student.cpp \
    src/models/UndergraduateStudent.cpp \
    src/models/GraduateStudent.cpp \
    src/models/PhDStudent.cpp \
    src/services/StudentManager.cpp \
    src/persistence/PersistenceManager.cpp
./test_runner
```

---

## Fonctionnalités

| Fonctionnalité | Statut |
|---|---|
| POO & Polymorphisme | ✅ |
| STL (map, sort, find_if) | ✅ |
| Exceptions personnalisées | ✅ |
| Persistance fichier | ✅ |
| Interface graphique Qt | ✅ |
| Tests unitaires | ✅ |
| Smart pointers exclusifs | ✅ |

---

## Liens
- **GitHub** : https://github.com/youssrasifeddine-gif/StudentManager
- **Vidéo démo** : https://youtu.be/[lien-video]
- **Rapport** : https://drive.google.com/file/d/1ik7ko9J_depy7uDhK5mHpCRDXWN-XScO/view?usp=drivesdk
---


