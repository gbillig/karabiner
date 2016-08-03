#include "newdialog.h"
#include <QtWidgets>

NewDialog::NewDialog(QWidget *parent)
      : QDialog(parent)
{
    nameLabel = new QLabel(tr("Document &name:"));
    nameEdit = new QLineEdit;
    nameLabel->setBuddy(nameEdit);

    buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(QDialogButtonBox::Ok);
    buttonBox->addButton(QDialogButtonBox::Cancel);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(nameLabel);
    topLayout->addWidget(nameEdit);


    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(topLayout, 0, 0);
    mainLayout->addWidget(buttonBox, 1, 0);
    setLayout(mainLayout);
}
