#include "../inc/newpassworddialog.h"

NewPasswordDialog::NewPasswordDialog(QWidget *parent)
      : QDialog(parent),
        userdata(UserData::GetInstance())
{
    serviceNameEdit = new QLineEdit(this);
    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);

    QFormLayout *pwEntryFormLayout = new QFormLayout;
    pwEntryFormLayout->addRow(tr("&Service name:"), serviceNameEdit);
    pwEntryFormLayout->addRow(tr("&Username:"), usernameEdit);
    pwEntryFormLayout->addRow(tr("&Password:"), passwordEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                   | QDialogButtonBox::Cancel,
                                     Qt::Horizontal);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &NewPasswordDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addLayout(pwEntryFormLayout, 0, 0, 1, -1);
    mainLayout->addWidget(buttonBox, 1, 0, 1, -1);
    setLayout(mainLayout);
}

void NewPasswordDialog::accept()
{
    QString service_name = serviceNameEdit->text();
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    QString notes = "notes";

    PwEntry *newPwEntry = new PwEntry(service_name, username, password, notes);
    userdata->AddNewPwEntry(*newPwEntry);
    done(QDialog::Accepted);
}
