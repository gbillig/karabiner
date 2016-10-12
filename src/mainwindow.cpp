#include "../inc/mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    userdata(UserData::GetInstance())
{
    ui->setupUi(this);
    createUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createUi() {
    // icon resources
    const QPixmap *plus_pixmap = new QPixmap(":/res/plus-48.png");
    QIcon *plus_icon = new QIcon(*plus_pixmap);
    const QPixmap *minus_pixmap = new QPixmap(":/res/minus-48.png");
    QIcon *minus_icon = new QIcon(*minus_pixmap);


    // create toolbar
    QToolBar *fileToolBar = addToolBar("File");

    // create actions
    const QPixmap *new_pixmap = new QPixmap(":/res/create_new-48.png");
    QIcon *new_icon = new QIcon(*new_pixmap);
    QAction *newDocument = new QAction(*new_icon, "New document", this);
    connect(newDocument, &QAction::triggered, this, &MainWindow::createNewUser);

    const QPixmap *open_pixmap = new QPixmap(":/res/opened_folder-48.png");
    QIcon *open_icon = new QIcon(*open_pixmap);
    QAction *openDocument = new QAction(*open_icon, "Open document", this);
    connect(openDocument, &QAction::triggered, this, &MainWindow::open);

    const QPixmap *save_pixmap = new QPixmap(":/res/save-48.png");
    QIcon *save_icon = new QIcon(*save_pixmap);
    QAction *saveDocument = new QAction(*save_icon, "Save document", this);

    const QPixmap *save_as_pixmap = new QPixmap(":/res/save_as-48.png");
    QIcon *save_as_icon = new QIcon(*save_as_pixmap);
    QAction *saveAsDocument = new QAction(*save_as_icon, "Save document as", this);
    connect(saveAsDocument, &QAction::triggered, this, &MainWindow::saveAs);

    QList<QAction*> *actions = new QList<QAction*>();
    actions->append(newDocument);
    actions->append(openDocument);
    actions->append(saveDocument);
    actions->append(saveAsDocument);

    fileToolBar->addActions(*actions);

    // create mainwindow layout
    mainLayout = new QGridLayout;

    // create column with list of users
    QPushButton *addUser = new QPushButton(*plus_icon, "Add user", this);
    connect(addUser, &QPushButton::clicked, this, &MainWindow::createNewUser);
    removeUser = new QPushButton(*minus_icon, "Remove user", this);
    connect(removeUser, &QPushButton::clicked, this, &MainWindow::removeSelectedUserEntry);
    removeUser->setEnabled(false);

    userColumn = new QListView;
    userColumnModel = new QStringListModel;
    QFont* columnFont = new QFont;
    columnFont->setPointSize(14);

    userColumn->setEditTriggers(QAbstractItemView::NoEditTriggers);
    userColumn->setModel(userColumnModel);
    userColumn->setFont(*columnFont);
    connect(userdata, &UserData::userAdded, this, &MainWindow::addUserEntry);
    connect(userColumn->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::userSelected);

    // create password entry column
    QPushButton *addPassword = new QPushButton(*plus_icon, "Add password entry", this);
    connect(addPassword, &QPushButton::clicked, this, &MainWindow::createNewPassword);
    removePassword = new QPushButton(*minus_icon, "Remove password entry", this);
    connect(removePassword, &QPushButton::clicked, this, &MainWindow::removeSelectedPasswordEntry);
    removePassword->setEnabled(false);

    passwordColumn = new QListView;
    passwordColumnModel= new QStringListModel;

    passwordColumn->setEditTriggers(QAbstractItemView::NoEditTriggers);
    passwordColumn->setModel(passwordColumnModel);
    passwordColumn->setFont(*columnFont);
    connect(userdata, &UserData::passwordEntryAdded, this, &MainWindow::addPasswordEntry);

    // create details panel
    serviceName = new QLabel;
    username = new QLabel;
    password = new QLabel;
    notes = new QLabel;
    connect(passwordColumn->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::passwordEntrySelected);

    QGridLayout* userColumnLayout = new QGridLayout;
    userColumnLayout->addWidget(addUser, 0, 0);
    userColumnLayout->addWidget(removeUser, 0, 1);
    userColumnLayout->addWidget(userColumn, 1, 0, 1, 0);

    QGridLayout* passwordColumnLayout = new QGridLayout;
    passwordColumnLayout->addWidget(addPassword, 0, 0);
    passwordColumnLayout->addWidget(removePassword, 0, 1);
    passwordColumnLayout->addWidget(passwordColumn, 1, 0, 1, 0);

    QFormLayout* detailsLayout = new QFormLayout;
    detailsLayout->addRow(tr("Service name:"), serviceName);
    detailsLayout->addRow(tr("Username:"), username);
    detailsLayout->addRow(tr("Password:"), password);
    detailsLayout->addRow(tr("Notes:"), notes);

    QWidget* userSection = new QWidget;
    userSection->setLayout(userColumnLayout);
    QWidget* passwordSection = new QWidget;
    passwordSection->setLayout(passwordColumnLayout);
    QWidget* detailsSection = new QWidget;
    detailsSection->setLayout(detailsLayout);
    int buttonHeight = addUser->height();
    detailsSection->setContentsMargins(0, buttonHeight + 4, 0, 0);

    QSplitter* mainSplitter = new QSplitter;
    mainSplitter->addWidget(userSection);
    mainSplitter->addWidget(passwordSection);
    mainSplitter->addWidget(detailsSection);

    mainLayout->addWidget(mainSplitter);
    ui->centralWidget->setLayout(mainLayout);
}

void MainWindow::open() {
    int rval;
    QString filePath = QFileDialog::getOpenFileName(this);
    if (!filePath.isEmpty()) {
        // remove all current user rows
        int numUserRows = userColumnModel->rowCount();
        userColumnModel->removeRows(0, numUserRows);

        rval = userdata->ParseUserFile(filePath);
        if (rval != 0) {
            QMessageBox *invalidFileMsgBox = new QMessageBox(this);
            invalidFileMsgBox->setText("Unable to open file \"" + filePath + "\".");
            invalidFileMsgBox->setIcon(QMessageBox::Critical);
            invalidFileMsgBox->exec();
        }
    }
}

void MainWindow::saveAs() {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File As"),
                                                    Q_NULLPTR,
                                                    tr("Karabiner Account File (*.kbr)"));

    if (!filePath.isEmpty()) {
        userdata->SaveUserFile(filePath);
    }
}

void MainWindow::createNewUser() {
    NewUserDialog *dialog = new NewUserDialog(this);
    dialog->exec();
}

void MainWindow::createNewPassword() {
    NewPasswordDialog *dialog = new NewPasswordDialog(this);
    dialog->exec();
}

void MainWindow::addUserEntry(QString usernameToAdd) {
    int lastRow = userColumnModel->rowCount();
    userColumnModel->insertRow(lastRow);
    QModelIndex lastRowModelIndex = userColumnModel->index(lastRow);
    userColumnModel->setData(lastRowModelIndex, usernameToAdd, Qt::DisplayRole);
    userColumn->selectionModel()->select(lastRowModelIndex, QItemSelectionModel::ClearAndSelect);
}

void MainWindow::removeSelectedUserEntry() {
    QList<QModelIndex> selectedRowIndexes = userColumn->selectionModel()->selectedRows();
    QModelIndex selectedRowModelIndex = selectedRowIndexes.first();

    QString usernameString = selectedRowModelIndex.data().toString();
    userdata->DeleteUser(usernameString);

    int selectedRow = selectedRowModelIndex.row();
    userColumnModel->removeRow(selectedRow);
    userColumn->selectionModel()->clear();
}

void MainWindow::userSelected(const QItemSelection &selectedUserItem, const QItemSelection &deselectedUserItem) {
    QList<QModelIndex> selectedUserIndexes = selectedUserItem.indexes();
    QList<QModelIndex> deselectedUserIndexes = deselectedUserItem.indexes();

    if (selectedUserIndexes.size() == 0) {
        removeUser->setEnabled(false);
        refreshPasswordEntries();
        return;
    }

    removeUser->setEnabled(true);

    int selectedRow = selectedUserIndexes[0].row();
    QString selectedUsername = userColumnModel->stringList()[selectedRow];
    User* selectedUser = userdata->GetUser(selectedUsername);

    // authenticate user if encrypted
    if (!selectedUser->isDecrypted()) {
        bool authenticated = 0;

        while (!authenticated) {
            bool accepted;
            QString password = QInputDialog::getText(this, "Authentication", "Password:",
                                                     QLineEdit::Password, "", &accepted);

            // if user presses cancel on the dialog
            if (!accepted) {
                if (deselectedUserIndexes.size() == 0) {
                    // clear selection if nothing was previously selected
                    userColumn->selectionModel()->clearSelection();
                } else {
                    // select previously selected row
                    userColumn->selectionModel()->select(deselectedUserIndexes[0], QItemSelectionModel::ClearAndSelect);
                }

                return;
            }

            authenticated = selectedUser->Authenticate(password, User::Decrypt);
        }
    }

    refreshPasswordEntries();
}

void MainWindow::addPasswordEntry(QString newServiceName) {
    int lastRow = passwordColumnModel->rowCount();
    passwordColumnModel->insertRow(lastRow);
    QModelIndex lastRowModelIndex = passwordColumnModel->index(lastRow);
    passwordColumnModel->setData(lastRowModelIndex, newServiceName, Qt::DisplayRole);
    passwordColumn->selectionModel()->select(lastRowModelIndex, QItemSelectionModel::ClearAndSelect);
}

void MainWindow::removeSelectedPasswordEntry() {
    QList<QModelIndex> selectedRowIndexes = passwordColumn->selectionModel()->selectedRows();
    QModelIndex selectedRowModelIndex = selectedRowIndexes.first();

    QString serviceName = selectedRowModelIndex.data().toString();
    userdata->DeleteUser(serviceName);

    int selectedRow = selectedRowModelIndex.row();
    passwordColumnModel->removeRow(selectedRow);
    passwordColumn->selectionModel()->clear();
}

void MainWindow::passwordEntrySelected(const QItemSelection &selectedPasswordItem, const QItemSelection &deselectedPasswordItem) {
    QList<QModelIndex> selectedPasswordIndexes = selectedPasswordItem.indexes();
    QList<QModelIndex> deselectedPasswordIndexes = deselectedPasswordItem.indexes();

    if (selectedPasswordIndexes.length() != 0) {
        removePassword->setEnabled(true);
    } else {
        removePassword->setEnabled(false);
    }

    refreshDetailsPane();
}

void MainWindow::refreshPasswordEntries() {
    int numRows = passwordColumnModel->rowCount();
    passwordColumnModel->removeRows(0, numRows);

    // find the selected row
    QList<QModelIndex> selectedRowIndexes = userColumn->selectionModel()->selectedRows();
    if (selectedRowIndexes.size() == 0) {
        return;
    }

    int selectedRow = selectedRowIndexes.first().row();
    QString selectedUsername = userColumnModel->stringList()[selectedRow];
    User* selectedUser = userdata->GetUser(selectedUsername);

    int numPasswordEntries = selectedUser->password_entries.length();
    passwordColumnModel->insertRows(0, numPasswordEntries);

    int i;
    QModelIndex curRowModelIndex;
    for (i = 0; i < numPasswordEntries; i++) {
        curRowModelIndex = passwordColumnModel->index(i);
        passwordColumnModel->setData(curRowModelIndex, selectedUser->password_entries[i].service_name, Qt::DisplayRole);
    }
}

void MainWindow::refreshDetailsPane() {
    // find the selected user
    QList<QModelIndex> selectedRowIndexes = userColumn->selectionModel()->selectedRows();

    if (selectedRowIndexes.size() == 0) {
        clearDetailsPane();
        return;
    }

    int selectedRow = selectedRowIndexes[0].row();
    QString selectedUsername = userColumnModel->stringList()[selectedRow];

    User* selectedUser = userdata->GetUser(selectedUsername);

    // find the selected password entry
    selectedRowIndexes = passwordColumn->selectionModel()->selectedRows();

    if (selectedRowIndexes.size() == 0) {
        clearDetailsPane();
        return;
    }

    selectedRow = selectedRowIndexes[0].row();
    PwEntry selectedPwEntry = selectedUser->password_entries[selectedRow];

    serviceName->setText(selectedPwEntry.service_name);
    username->setText(selectedPwEntry.username);
    password->setText(selectedPwEntry.password);
    notes->setText(selectedPwEntry.notes);
}

void MainWindow::clearDetailsPane() {
    serviceName->setText("");
    username->setText("");
    password->setText("");
    notes->setText("");
}
