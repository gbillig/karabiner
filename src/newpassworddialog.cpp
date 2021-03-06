#include "../inc/newpassworddialog.h"

NewPasswordDialog::NewPasswordDialog(QWidget *parent)
      : QDialog(parent),
        userdata(UserData::GetInstance())
{
    serviceNameEdit = new QLineEdit;
    usernameEdit = new QLineEdit;
    passwordEdit = new QLineEdit;

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
    setWindowTitle(tr("Add password entry"));
}

void NewPasswordDialog::accept()
{
    QString service_name = serviceNameEdit->text();
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    QString notes = "notes";

    if (service_name == "") {
        // service_name field is emtpy
        return;
    }

    if (username == "") {
        // username field is empty
        return;
    }

    if (password == "") {
        // password field is empty
        return;
    }

    PwEntry *newPwEntry = new PwEntry(service_name, username, password, notes);

    MainWindow* mainWindow = (MainWindow*) this->parent();
    if (!mainWindow) {
        return;
    }

    // find which category is selected
    QList<QModelIndex> selectedCategoryRowIndexes = mainWindow->categoryColumn->selectionModel()->selectedRows();
    if (selectedCategoryRowIndexes.size() == 0) {
        return;
    }

    int selectedCategoryRow = selectedCategoryRowIndexes[0].row();
    QStringList categoryList = mainWindow->categoryColumnModel->stringList();
    QString selectedCategoryString = categoryList[selectedCategoryRow];
    Category* selectedCategory = userdata->GetCategory(selectedCategoryString);

    userdata->AddNewPwEntry(selectedCategory, *newPwEntry);
    done(QDialog::Accepted);
}
