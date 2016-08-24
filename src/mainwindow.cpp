#include "../inc/mainwindow.h"
#include "../inc/newdialog.h"
#include "../build/ui_mainwindow.h"
#include "../inc/user.h"
#include "../inc/userdata.h"
#include <QtWidgets>

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
    //create toolbar
    QToolBar *fileToolBar = addToolBar("File");

    //create actions
    const QPixmap *new_pixmap = new QPixmap(":/res/create_new-48.png");
    QIcon *new_icon = new QIcon(*new_pixmap);
    QAction *newDocument = new QAction(*new_icon, "New document", this);
    connect(newDocument, &QAction::triggered, this, &MainWindow::createNew);

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

    const QPixmap *plus_pixmap = new QPixmap(":/res/plus-48.png");
    QIcon *plus_icon = new QIcon(*plus_pixmap);
    QAction *newEntry = new QAction(*plus_icon, "New entry", this);

    const QPixmap *minus_pixmap = new QPixmap(":/res/minus-48.png");
    QIcon *minus_icon = new QIcon(*minus_pixmap);
    QAction *removeEntry = new QAction(*minus_icon, "Remove entry", this);

    QList<QAction*> *actions = new QList<QAction*>();
    actions->append(newDocument);
    actions->append(openDocument);
    actions->append(saveDocument);
    actions->append(saveAsDocument);
    actions->append(newEntry);
    actions->append(removeEntry);

    fileToolBar->addActions(*actions);
    //ui->toolBar->addActions(*actions);
    //ui->menuBar->addActions(*actions);

    /*
    QFile inputFile(":/input.txt");
    inputFile.open(QIODevice::ReadOnly);

    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();

    ui->textEdit->setPlainText(line);
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
    */


    //User *myUser = new User();
    //myUser->PopulateFromString("Hello,ae5343934bc2393d,22,twenty-two");
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


void MainWindow::createNew()
{
    //QDialog *createNewDialog = new QDialog(this);
    NewDialog *dialog = new NewDialog(this);
    dialog->exec();
    //createNewDialog->exec();

}
