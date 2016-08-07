#include "newdialog.h"
#include <QtWidgets>

NewDialog::NewDialog(QWidget *parent)
      : QDialog(parent)
{
    nameLabel = new QLabel(tr("Document &name:"));
    nameEdit = new QLineEdit;
    nameLabel->setBuddy(nameEdit);

    cryptoBox = new QGroupBox(tr("Encryption algorithm"));
    Aes128RadioButton = new QRadioButton(tr("AES-128"));
    Aes192RadioButton = new QRadioButton(tr("AES-192"));
    Aes256RadioButton = new QRadioButton(tr("AES-256"));

    hashBox = new QGroupBox(tr("Hashing algorithm"));
    Sha256RadioButton = new QRadioButton(tr("SHA-256"));
    Sha384RadioButton = new QRadioButton(tr("SHA-384"));
    Sha512RadioButton = new QRadioButton(tr("SHA-512"));

    Aes256RadioButton->setChecked(true);
    Sha256RadioButton->setChecked(true);

    buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(QDialogButtonBox::Ok);
    buttonBox->addButton(QDialogButtonBox::Cancel);


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

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(nameLabel);
    topLayout->addWidget(nameEdit);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(topLayout, 0, 0, 1, -1);
    mainLayout->addWidget(cryptoBox, 1, 0);
    mainLayout->addWidget(hashBox, 1, 1);
    mainLayout->addWidget(buttonBox, 2, 0, 1, -1);
    setLayout(mainLayout);
}
