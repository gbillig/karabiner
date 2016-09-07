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

void MainWindow::createUi()
{
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
    mainLayout = new QGridLayout();

    // create column with list of users
    QPushButton *addUser = new QPushButton(*plus_icon, "Add user", this);
    connect(addUser, &QPushButton::clicked, this, &MainWindow::createNewUser);

    QPushButton *removeUser = new QPushButton(*minus_icon, "Remove user", this);

    userColumn = new QListView(this);
    userColumnModel = new QStringListModel(this);

    userColumn->setEditTriggers(QAbstractItemView::NoEditTriggers);
    userColumn->setModel(userColumnModel);
    updateUserColumn();
    connect(userdata, &UserData::userDataChanged, this, &MainWindow::updateUserColumn);
    connect(userColumn->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::updatePasswordColumn);

    // create password entry column
    QPushButton *addPassword = new QPushButton(*plus_icon, "Add password entry", this);
    connect(addPassword, &QPushButton::clicked, this, &MainWindow::createNewPassword);
    QPushButton *removePassword = new QPushButton(*minus_icon, "Remove password entry", this);

    passwordColumn = new QListView(this);
    passwordColumnModel= new QStringListModel(this);

    passwordColumn->setEditTriggers(QAbstractItemView::NoEditTriggers);
    passwordColumn->setModel(passwordColumnModel);
    connect(userdata, &UserData::PwEntryChanged, this, &MainWindow::updatePasswordColumn);

    QGridLayout* userColumnLayout = new QGridLayout();
    userColumnLayout->addWidget(addUser, 0, 0);
    userColumnLayout->addWidget(removeUser, 0, 1);
    userColumnLayout->addWidget(userColumn, 1, 0, 1, 0);

    QGridLayout* passwordColumnLayout = new QGridLayout();
    passwordColumnLayout->addWidget(addPassword, 0, 0);
    passwordColumnLayout->addWidget(removePassword, 0, 1);
    passwordColumnLayout->addWidget(passwordColumn, 1, 0, 1, 0);

    mainLayout->addLayout(userColumnLayout, 0, 0);
    mainLayout->addLayout(passwordColumnLayout, 0, 1);

    ui->centralWidget->setLayout(mainLayout);
}

void MainWindow::open()
{
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

void MainWindow::saveAs()
{
    QFileDialog saveAsDialog(this);
    saveAsDialog.setFileMode(QFileDialog::AnyFile);
    QString filePath = saveAsDialog.getSaveFileName(this);
    if (!filePath.isEmpty()) {
        userdata->SaveUserFile(filePath);
    }
}

void MainWindow::createNewUser()
{
    NewUserDialog *dialog = new NewUserDialog(this);
    dialog->exec();
}

void MainWindow::createNewPassword()
{
    NewPasswordDialog *dialog = new NewPasswordDialog(this);
    dialog->exec();
}

void MainWindow::updateUserColumn()
{
    int i;

    // find the selected row
    int selectedRow = -1;
    QList<QModelIndex> selectedRowIndexes = userColumn->selectionModel()->selectedRows();
    if (selectedRowIndexes.size() > 0) {
        QModelIndex selectedRowIndex = selectedRowIndexes[0];
        selectedRow = selectedRowIndex.row();
    }

    QStringList userStringList = QStringList();
    QVector<User>* users = userdata->GetUsers();

    for (i = 0; i < users->size(); i++ ) {
        userStringList << users->value(i).username;
    }
    userColumnModel->setStringList(userStringList);

    if (selectedRow < 0) {
        return;
    }

    QModelIndex selectedIndex = userColumnModel->index(selectedRow, 0);
    userColumn->selectionModel()->select(selectedIndex, QItemSelectionModel::Select);
}

void MainWindow::updatePasswordColumn()
{
    // find the selected row
    QList<QModelIndex> selectedRowIndexes = userColumn->selectionModel()->selectedRows();

    if (selectedRowIndexes.size() == 0) {
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

    int i;
    for (i = 0; i < selectedUser->password_entries.length(); i++) {
        passwordStringList.append(selectedUser->password_entries[i].service_name);
    }

    passwordColumnModel->setStringList(passwordStringList);
}
