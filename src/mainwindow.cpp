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
    connect(newDocument, &QAction::triggered, this, &MainWindow::createNewCategory);

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

    // create column with list of categories
    addCategory = new QPushButton(*plus_icon, "Add category", this);
    connect(addCategory, &QPushButton::clicked, this, &MainWindow::createNewCategory);
    removeCategory = new QPushButton(*minus_icon, "Remove category", this);
    removeCategory->setEnabled(false);
    connect(removeCategory, &QPushButton::clicked, this, &MainWindow::removeSelectedCategoryEntry);

    categoryColumn = new QListView;
    categoryColumnModel = new QStringListModel;
    QFont* columnFont = new QFont;
    columnFont->setPointSize(14);

    categoryColumn->setEditTriggers(QAbstractItemView::NoEditTriggers);
    categoryColumn->setModel(categoryColumnModel);
    categoryColumn->setFont(*columnFont);
    connect(userdata, &UserData::categoryAdded, this, &MainWindow::addCategoryEntry);
    connect(categoryColumn->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::categorySelected);

    // create password entry column
    addPassword = new QPushButton(*plus_icon, "Add password entry", this);
    addPassword->setEnabled(false);
    connect(addPassword, &QPushButton::clicked, this, &MainWindow::createNewPassword);
    removePassword = new QPushButton(*minus_icon, "Remove password entry", this);
    removePassword->setEnabled(false);
    connect(removePassword, &QPushButton::clicked, this, &MainWindow::removeSelectedPasswordEntry);

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

    QGridLayout* categoryColumnLayout = new QGridLayout;
    categoryColumnLayout->addWidget(addCategory, 0, 0);
    categoryColumnLayout->addWidget(removeCategory, 0, 1);
    categoryColumnLayout->addWidget(categoryColumn, 1, 0, 1, 0);

    QGridLayout* passwordColumnLayout = new QGridLayout;
    passwordColumnLayout->addWidget(addPassword, 0, 0);
    passwordColumnLayout->addWidget(removePassword, 0, 1);
    passwordColumnLayout->addWidget(passwordColumn, 1, 0, 1, 0);

    QFormLayout* detailsLayout = new QFormLayout;
    detailsLayout->addRow(tr("Service name:"), serviceName);
    detailsLayout->addRow(tr("Username:"), username);
    detailsLayout->addRow(tr("Password:"), password);
    detailsLayout->addRow(tr("Notes:"), notes);

    QWidget* categorySection = new QWidget;
    categorySection->setLayout(categoryColumnLayout);
    QWidget* passwordSection = new QWidget;
    passwordSection->setLayout(passwordColumnLayout);
    QWidget* detailsSection = new QWidget;
    detailsSection->setLayout(detailsLayout);
    int buttonHeight = addCategory->height();
    detailsSection->setContentsMargins(0, buttonHeight + 4, 0, 0);

    QSplitter* mainSplitter = new QSplitter;
    mainSplitter->addWidget(categorySection);
    mainSplitter->addWidget(passwordSection);
    mainSplitter->addWidget(detailsSection);

    mainLayout->addWidget(mainSplitter);
    ui->centralWidget->setLayout(mainLayout);
}

void MainWindow::open() {
    int rval;
    QString filePath = QFileDialog::getOpenFileName(this);
    if (!filePath.isEmpty()) {
        // remove all current category rows
        int numCategoryRows = categoryColumnModel->rowCount();
        categoryColumnModel->removeRows(0, numCategoryRows);

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

void MainWindow::createNewCategory() {
    NewCategoryDialog *dialog = new NewCategoryDialog(this);
    dialog->exec();
}

void MainWindow::createNewPassword() {
    NewPasswordDialog *dialog = new NewPasswordDialog(this);
    dialog->exec();
}

void MainWindow::addCategoryEntry(QString categoryToAdd) {
    int lastRow = categoryColumnModel->rowCount();
    categoryColumnModel->insertRow(lastRow);
    QModelIndex lastRowModelIndex = categoryColumnModel->index(lastRow);
    categoryColumnModel->setData(lastRowModelIndex, categoryToAdd, Qt::DisplayRole);
    //categoryColumn->selectionModel()->select(lastRowModelIndex, QItemSelectionModel::ClearAndSelect);
}

void MainWindow::removeSelectedCategoryEntry() {
    QList<QModelIndex> selectedRowIndexes = categoryColumn->selectionModel()->selectedRows();
    QModelIndex selectedRowModelIndex = selectedRowIndexes.first();

    QString categoryString = selectedRowModelIndex.data().toString();
    userdata->DeleteCategory(categoryString);

    int selectedRow = selectedRowModelIndex.row();
    categoryColumnModel->removeRow(selectedRow);
    categoryColumn->selectionModel()->clear();
}

void MainWindow::categorySelected(const QItemSelection &selectedCategoryItem, const QItemSelection &deselectedCategoryItem) {
    QList<QModelIndex> selectedCategoryIndexes = selectedCategoryItem.indexes();
    QList<QModelIndex> deselectedCategoryIndexes = deselectedCategoryItem.indexes();

    if (selectedCategoryIndexes.size() == 0) {
        removeCategory->setEnabled(false);
        addPassword->setEnabled(false);
        refreshPasswordEntries();
        return;
    }

    removeCategory->setEnabled(true);
    addPassword->setEnabled(true);

    int selectedRow = selectedCategoryIndexes[0].row();
    QString selectedCategoryName = categoryColumnModel->stringList()[selectedRow];
    Category* selectedCategory = userdata->GetCategory(selectedCategoryName);

    // authenticate category if encrypted
    if (!selectedCategory->isDecrypted()) {
        int auth_rval;

        do {
            bool accepted;
            QString password = QInputDialog::getText(this, "Authentication", "Password:",
                                                     QLineEdit::Password, "", &accepted);

            // if user presses cancel on the dialog
            if (!accepted) {
                if (deselectedCategoryIndexes.size() == 0) {
                    // clear selection if nothing was previously selected
                    categoryColumn->selectionModel()->clearSelection();
                } else {
                    // select previously selected row
                    categoryColumn->selectionModel()->select(deselectedCategoryIndexes[0], QItemSelectionModel::ClearAndSelect);
                }

                return;
            }

            auth_rval = selectedCategory->Authenticate(password, Category::Decrypt);

            //TODO: Add small delay between authentication attempts
        } while (auth_rval != 0);
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
    userdata->DeleteCategory(serviceName);

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
    QList<QModelIndex> selectedRowIndexes = categoryColumn->selectionModel()->selectedRows();
    if (selectedRowIndexes.size() == 0) {
        return;
    }

    int selectedRow = selectedRowIndexes.first().row();
    QString selectedCategoryName = categoryColumnModel->stringList()[selectedRow];
    Category* selectedCategory = userdata->GetCategory(selectedCategoryName);

    int numPasswordEntries = selectedCategory->password_entries.length();
    passwordColumnModel->insertRows(0, numPasswordEntries);

    int i;
    QModelIndex curRowModelIndex;
    for (i = 0; i < numPasswordEntries; i++) {
        curRowModelIndex = passwordColumnModel->index(i);
        passwordColumnModel->setData(curRowModelIndex, selectedCategory->password_entries[i].service_name, Qt::DisplayRole);
    }
}

void MainWindow::refreshDetailsPane() {
    // find the selected category
    QList<QModelIndex> selectedRowIndexes = categoryColumn->selectionModel()->selectedRows();

    if (selectedRowIndexes.size() == 0) {
        clearDetailsPane();
        return;
    }

    int selectedRow = selectedRowIndexes[0].row();
    QString selectedCategoryName = categoryColumnModel->stringList()[selectedRow];

    Category* selectedCategory = userdata->GetCategory(selectedCategoryName);

    // find the selected password entry
    selectedRowIndexes = passwordColumn->selectionModel()->selectedRows();

    if (selectedRowIndexes.size() == 0) {
        clearDetailsPane();
        return;
    }

    selectedRow = selectedRowIndexes[0].row();
    PwEntry selectedPwEntry = selectedCategory->password_entries[selectedRow];

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
