#include "StudentDialog.h"
#include "../models/UndergraduateStudent.h"
#include "../models/GraduateStudent.h"
#include "../models/PhDStudent.h"
#include "../exceptions/StudentExceptions.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

// ─────────────────────────────────────────────────────────────────────────────
// Constructeurs
// ─────────────────────────────────────────────────────────────────────────────

StudentDialog::StudentDialog(QWidget* parent)
    : QDialog(parent), editMode(false)
{
    setWindowTitle("Ajouter un étudiant");
    setupUi();
    onTypeChanged("Licence");  // valeur par défaut
}

StudentDialog::StudentDialog(QWidget* parent, const shared_ptr<Student>& existing)
    : QDialog(parent), editMode(true)
{
    setWindowTitle("Modifier un étudiant");
    setupUi();
    populateFrom(existing);
    edtId->setReadOnly(true);  // L'ID ne peut pas changer en mode édition
}

// ─────────────────────────────────────────────────────────────────────────────
// setupUi()
// ─────────────────────────────────────────────────────────────────────────────

void StudentDialog::setupUi() {
    auto* mainLayout = new QVBoxLayout(this);
    auto* form       = new QFormLayout;

    // Champs communs
    cboType = new QComboBox(this);
    cboType->addItems({"Licence", "Master", "Doctorat"});
    form->addRow("Type :", cboType);

    edtName = new QLineEdit(this);
    form->addRow("Nom complet :", edtName);

    edtId = new QLineEdit(this);
    edtId->setPlaceholderText("≥ 1000");
    form->addRow("ID :", edtId);

    spnGpa = new QDoubleSpinBox(this);
    spnGpa->setRange(0.0, 4.0);
    spnGpa->setSingleStep(0.1);
    spnGpa->setDecimals(1);
    form->addRow("GPA :", spnGpa);

    mainLayout->addLayout(form);

    // ── Groupe Licence ─────────────────────────────────────────────────────
    grpLicence = new QGroupBox("Informations Licence", this);
    auto* lForm = new QFormLayout(grpLicence);
    edtMajor = new QLineEdit(grpLicence);
    lForm->addRow("Filière :", edtMajor);
    mainLayout->addWidget(grpLicence);

    // ── Groupe Master ──────────────────────────────────────────────────────
    grpMaster = new QGroupBox("Informations Master", this);
    auto* mForm = new QFormLayout(grpMaster);
    edtResearchTopic = new QLineEdit(grpMaster);
    mForm->addRow("Sujet de recherche :", edtResearchTopic);
    mainLayout->addWidget(grpMaster);

    // ── Groupe Doctorat ────────────────────────────────────────────────────
    grpPhD = new QGroupBox("Informations Doctorat", this);
    auto* pForm = new QFormLayout(grpPhD);
    edtSupervisor = new QLineEdit(grpPhD);
    spnYear       = new QSpinBox(grpPhD);
    spnYear->setRange(1, 10);
    pForm->addRow("Directeur de thèse :", edtSupervisor);
    pForm->addRow("Année d'étude :",      spnYear);
    mainLayout->addWidget(grpPhD);

    // Boutons
    buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(buttonBox);

    // Connexions
    connect(cboType,    &QComboBox::currentTextChanged,
            this, &StudentDialog::onTypeChanged);
    connect(buttonBox,  &QDialogButtonBox::accepted,
            this, &StudentDialog::onValidate);
    connect(buttonBox,  &QDialogButtonBox::rejected,
            this, &QDialog::reject);

    setMinimumWidth(400);

    setStyleSheet(R"(
        QDialog    { background: #f9fafb; }
        QGroupBox  { font-weight: bold; border: 1px solid #bdc3c7;
                     border-radius: 4px; margin-top: 6px; padding: 8px; }
        QGroupBox::title { subcontrol-origin: margin; left: 10px; }
        QLineEdit, QDoubleSpinBox, QSpinBox, QComboBox {
            padding: 4px 8px; border: 1px solid #bdc3c7;
            border-radius: 3px; font-size: 13px; }
        QPushButton { padding: 6px 20px; font-weight: bold;
                      background: #2c3e50; color: white;
                      border: none; border-radius: 4px; }
        QPushButton:hover { background: #3498db; }
    )");
}

// ─────────────────────────────────────────────────────────────────────────────
// Pré-remplissage depuis un étudiant existant
// ─────────────────────────────────────────────────────────────────────────────

void StudentDialog::populateFrom(const shared_ptr<Student>& s) {
    cboType->setCurrentText(QString::fromStdString(s->getType()));
    edtName->setText(QString::fromStdString(s->getName()));
    edtId->setText(QString::number(s->getId()));
    spnGpa->setValue(static_cast<double>(s->getGpa()));

    if (s->getType() == "Licence") {
        auto u = dynamic_pointer_cast<UndergraduateStudent>(s);
        edtMajor->setText(QString::fromStdString(u->getMajor()));
    } else if (s->getType() == "Master") {
        auto g = dynamic_pointer_cast<GraduateStudent>(s);
        edtResearchTopic->setText(QString::fromStdString(g->getResearchTopic()));
    } else if (s->getType() == "Doctorat") {
        auto p = dynamic_pointer_cast<PhDStudent>(s);
        edtSupervisor->setText(QString::fromStdString(p->getSupervisor()));
        spnYear->setValue(p->getYearOfStudy());
    }

    onTypeChanged(cboType->currentText());
}

// ─────────────────────────────────────────────────────────────────────────────
// Slots
// ─────────────────────────────────────────────────────────────────────────────

void StudentDialog::onTypeChanged(const QString& type) {
    grpLicence->setVisible(type == "Licence");
    grpMaster->setVisible(type  == "Master");
    grpPhD->setVisible(type     == "Doctorat");
    adjustSize();
}

void StudentDialog::onValidate() {
    // Validation minimale côté GUI avant de créer l'objet
    if (edtName->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Champ requis", "Le nom ne peut pas être vide.");
        return;
    }
    bool ok;
    int id = edtId->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "ID invalide", "L'ID doit être un entier.");
        return;
    }
    if (id < 1000) {
        QMessageBox::warning(this, "ID invalide", "L'ID doit être >= 1000.");
        return;
    }
    accept();
}

// ─────────────────────────────────────────────────────────────────────────────
// getStudent()
// ─────────────────────────────────────────────────────────────────────────────

shared_ptr<Student> StudentDialog::getStudent() const {
    string type = cboType->currentText().toStdString();
    string name = edtName->text().trimmed().toStdString();
    int    id   = edtId->text().toInt();
    float  gpa  = static_cast<float>(spnGpa->value());

    if (type == "Licence") {
        string major = edtMajor->text().trimmed().toStdString();
        return make_shared<UndergraduateStudent>(name, id, gpa, major);
    } else if (type == "Master") {
        string topic = edtResearchTopic->text().trimmed().toStdString();
        return make_shared<GraduateStudent>(name, id, gpa, topic);
    } else {
        string sup  = edtSupervisor->text().trimmed().toStdString();
        int    year = spnYear->value();
        return make_shared<PhDStudent>(name, id, gpa, sup, year);
    }
}
