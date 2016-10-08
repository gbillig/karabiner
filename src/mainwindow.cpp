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
    QPushButton *removeUser = new QPushButton(*minus_icon, "Remove user", this);
    connect(removeUser, &QPushButton::clicked, this, &MainWindow::deleteUser);

    userColumn = new QListView;
    userColumnModel = new QStringListModel;
    QFont* columnFont = new QFont;
    columnFont->setPointSize(14);

    userColumn->setEditTriggers(QAbstractItemView::NoEditTriggers);
    userColumn->setModel(userColumnModel);
    userColumn->setFont(*columnFont);
    connect(userdata, &UserData::userDataChanged, this, &MainWindow::updateUserColumn);
    connect(userColumn->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::updatePasswordColumnTotal);

    // create password entry column
    QPushButton *addPassword = new QPushButton(*plus_icon, "Add password entry", this);
    connect(addPassword, &QPushButton::clicked, this, &MainWindow::createNewPassword);
    QPushButton *removePassword = new QPushButton(*minus_icon, "Remove password entry", this);

    passwordColumn = new QListView;
    passwordColumnModel= new QStringListModel;

    passwordColumn->setEditTriggers(QAbstractItemView::NoEditTriggers);
    passwordColumn->setModel(passwordColumnModel);
    passwordColumn->setFont(*columnFont);
    connect(userdata, &UserData::PwEntryChanged, this, &MainWindow::updatePasswordColumn);

    // create details panel
    serviceName = new QLabel;
    username = new QLabel;
    password = new QLabel;
    notes = new QLabel;
    connect(passwordColumn->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::updateDetailsPane);

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

void MainWindow::deleteUser() {
    // find the selected user
    QList<QModelIndex> selectedRowIndexes = userColumn->selectionModel()->selectedRows();

    if (selectedRowIndexes.size() == 0) {
        return;
    }

    int selectedRow = selectedRowIndexes[0].row();
    QString selectedUsername = userColumnModel->stringList()[selectedRow];
    userdata->DeleteUser(selectedUsername);
}

void MainWindow::createNewPassword() {
    NewPasswordDialog *dialog = new NewPasswordDialog(this);
    dialog->exec();
}

void MainWindow::updateUserColumn(QString newUsername) {
    int i;
    int newUsernameRow = -1;

    QStringList userStringList = QStringList();
    QVector<User>* users = userdata->GetUsers();

    // create new string list for user column model
    QString curUsername;
    for (i = 0; i < users->size(); i++ ) {
        curUsername = users->value(i).username;
        userStringList << curUsername;

        // record row of new username
        if (newUsername != "" && newUsername == curUsername) {
            newUsernameRow = i;
        }
    }
    userColumnModel->setStringList(userStringList);

    if (newUsernameRow == -1) {
        return;
    }

    QModelIndex newUsernameIndex = userColumnModel->index(newUsernameRow, 0);
    userColumn->selectionModel()->select(newUsernameIndex, QItemSelectionModel::Select);
}

void MainWindow::updatePasswordColumnTotal() {
    updatePasswordColumn("");
}

void MainWindow::updatePasswordColumn(QString newServiceName) {
    // find the selected row
    QList<QModelIndex> selectedRowIndexes = userColumn->selectionModel()->selectedRows();

    if (selectedRowIndexes.size() == 0) {
        MainWindow::clearPasswordColumn();
        return;
    }

    int selectedRow = selectedRowIndexes[0].row();
    QString selectedUsername = userColumnModel->stringList()[selectedRow];

    User* selectedUser = userdata->GetUser(selectedUsername);

    if (!selectedUser->isDecrypted) {
        bool authenticated = 0;

        while (!authenticated) {
            bool accepted;
            QString password = QInputDialog::getText(this, "Authentication", "Password:",
                                                     QLineEdit::Password, "", &accepted);

            if (!accepted) {
                passwordColumnModel->setStringList(QStringList());
                return;
            }

            authenticated = selectedUser->Authenticate(password, User::Decrypt);
        }
    }

    QStringList passwordStringList = QStringList();

    int i, newServiceNameRow = -1;
    QString curServiceName;
    for (i = 0; i < selectedUser->password_entries.length(); i++) {
        curServiceName = selectedUser->password_entries[i].service_name;
        passwordStringList.append(curServiceName);
        if (curServiceName == newServiceName) {
            newServiceNameRow = i;
        }

    }

    passwordColumnModel->setStringList(passwordStringList);

    if (newServiceNameRow == -1) {
        return;
    }

    QModelIndex newServiceNameIndex = passwordColumnModel->index(newServiceNameRow, 0);
    passwordColumn->selectionModel()->select(newServiceNameIndex, QItemSelectionModel::Select);
}

void MainWindow::updateDetailsPane() {
    // find the selected user
    QList<QModelIndex> selectedRowIndexes = userColumn->selectionModel()->selectedRows();

    if (selectedRowIndexes.size() == 0) {
        return;
    }

    int selectedRow = selectedRowIndexes[0].row();
    QString selectedUsername = userColumnModel->stringList()[selectedRow];

    User* selectedUser = userdata->GetUser(selectedUsername);

    // find the selected password entry
    selectedRowIndexes = passwordColumn->selectionModel()->selectedRows();

    if (selectedRowIndexes.size() == 0) {
        return;
    }

    selectedRow = selectedRowIndexes[0].row();
    PwEntry selectedPwEntry = selectedUser->password_entries[selectedRow];

    serviceName->setText(selectedPwEntry.service_name);
    username->setText(selectedPwEntry.username);
    password->setText(selectedPwEntry.password);
    notes->setText(selectedPwEntry.notes);
}

void MainWindow::clearPasswordColumn() {
    QStringList emptyList = QStringList();
    passwordColumnModel->setStringList(emptyList);
}
