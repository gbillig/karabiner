#include "../inc/newpassworddialog.h"

NewPasswordDialog::NewPasswordDialog(QWidget *parent)
      : QDialog(parent),
        userdata(UserData::GetInstance())
{
    QLabel *serviceNameLabel = new QLabel(tr("&Service name:"));
    serviceNameEdit = new QLineEdit(this);
    serviceNameLabel->setBuddy(serviceNameEdit);

    QLabel *usernameLabel = new QLabel(tr("&Username:"));
    usernameEdit = new QLineEdit(this);
    usernameLabel->setBuddy(usernameEdit);

    QLabel *passwordLabel = new QLabel(tr("&Password:"));
    passwordEdit = new QLineEdit(this);
    passwordLabel->setBuddy(passwordEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                   | QDialogButtonBox::Cancel,
                                     Qt::Horizontal);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &NewPasswordDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QHBoxLayout *serviceNameLayout = new QHBoxLayout();
    serviceNameLayout->addWidget(serviceNameLabel);
    serviceNameLayout->addWidget(serviceNameEdit);

    QHBoxLayout *usernameLayout = new QHBoxLayout();
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(usernameEdit);

    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordEdit);

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addLayout(serviceNameLayout, 0, 0, 1, -1);
    mainLayout->addLayout(usernameLayout, 1, 0, 1, -1);
    mainLayout->addLayout(passwordLayout, 2, 0, 1, -1);
    mainLayout->addWidget(buttonBox, 3, 0, 1, -1);
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
