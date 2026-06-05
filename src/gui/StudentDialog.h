#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLabel>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <memory>
#include "../models/Student.h"
using namespace std;

/**
 * @class StudentDialog
 * @brief Formulaire d'ajout / modification d'un étudiant.
 *
 * Les champs supplémentaires s'affichent dynamiquement
 * selon le type sélectionné dans la QComboBox.
 */
class StudentDialog : public QDialog {
    Q_OBJECT

public:
    /// Constructeur pour ajout (formulaire vide).
    explicit StudentDialog(QWidget* parent = nullptr);

    /// Constructeur pour modification (pré-remplissage).
    StudentDialog(QWidget* parent, const shared_ptr<Student>& existing);

    /// Retourne le shared_ptr<Student> construit à partir du formulaire.
    shared_ptr<Student> getStudent() const;

private slots:
    void onTypeChanged(const QString& type);
    void onValidate();

private:
    // ── Widgets communs ───────────────────────────────────────────────────────
    QComboBox*       cboType;
    QLineEdit*       edtName;
    QLineEdit*       edtId;
    QDoubleSpinBox*  spnGpa;

    // ── Widgets Licence ────────────────────────────────────────────────────────
    QGroupBox*  grpLicence;
    QLineEdit*  edtMajor;

    // ── Widgets Master ─────────────────────────────────────────────────────────
    QGroupBox*  grpMaster;
    QLineEdit*  edtResearchTopic;

    // ── Widgets Doctorat ───────────────────────────────────────────────────────
    QGroupBox*  grpPhD;
    QLineEdit*  edtSupervisor;
    QSpinBox*   spnYear;

    QDialogButtonBox* buttonBox;

    bool editMode;  ///< true si modification, false si ajout

    void setupUi();
    void populateFrom(const shared_ptr<Student>& s);
};

#endif // STUDENTDIALOG_H
