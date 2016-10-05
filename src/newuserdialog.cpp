#include "../inc/newuserdialog.h"


NewUserDialog::NewUserDialog(QWidget *parent)
      : QDialog(parent),
        userdata(UserData::GetInstance())
{
    usernameEdit = new QLineEdit;
    passwordEdit = new QLineEdit;
    passwordConfirmEdit = new QLineEdit;


    QFormLayout *accountFormLayout = new QFormLayout;
    accountFormLayout->addRow(tr("&Username:"), usernameEdit);
    accountFormLayout->addRow(tr("&Password:"), passwordEdit);
    accountFormLayout->addRow(tr("&Confirm Password:"), passwordConfirmEdit);

    cryptoBox = new QGroupBox(tr("Encryption algorithm"));
    Aes128RadioButton = new QRadioButton(tr("AES-128"));
    Aes192RadioButton = new QRadioButton(tr("AES-192"));
    Aes256RadioButton = new QRadioButton(tr("AES-256"));

    // temporarily disable other crypto options
    Aes128RadioButton->setEnabled(false);
    Aes192RadioButton->setEnabled(false);

    hashBox = new QGroupBox(tr("Hashing algorithm"));
    Sha256RadioButton = new QRadioButton(tr("SHA-256"));
    Sha384RadioButton = new QRadioButton(tr("SHA-384"));
    Sha512RadioButton = new QRadioButton(tr("SHA-512"));

    // temporarily disable other password hashing options
    Sha384RadioButton->setEnabled(false);
    Sha512RadioButton->setEnabled(false);

    Aes256RadioButton->setChecked(true);
    Sha256RadioButton->setChecked(true);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                   | QDialogButtonBox::Cancel,
                                     Qt::Horizontal);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &NewUserDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *cryptoBoxLayout = new QVBoxLayout;
    cryptoBoxLayout->addWidget(Aes128RadioButton);
    cryptoBoxLayout->addWidget(Aes192RadioButton);
    cryptoBoxLayout->addWidget(Aes256RadioButton);
    cryptoBox->setLayout(cryptoBoxLayout);

    QVBoxLayout *hashBoxLayout = new QVBoxLayout;
    hashBoxLayout->addWidget(Sha256RadioButton);
    hashBoxLayout->addWidget(Sha384RadioButton);
    hashBoxLayout->addWidget(Sha512RadioButton);
    hashBox->setLayout(hashBoxLayout);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(accountFormLayout, 0, 0, 1, -1);
    mainLayout->addWidget(cryptoBox, 1, 0);
    mainLayout->addWidget(hashBox, 1, 1);
    mainLayout->addWidget(buttonBox, 2, 0, 1, -1);
    setLayout(mainLayout);
}

void NewUserDialog::accept() {

    if (passwordEdit->text() != passwordConfirmEdit->text()) {
        // passwords do not match
        return;
    }

    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    User *newUser = new User(username, password);
    userdata->AddNewUser(*newUser);
    done(QDialog::Accepted);
}
