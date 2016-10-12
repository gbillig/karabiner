#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QListView>
#include <QStringListModel>
#include <QStringList>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QInputDialog>
#include <QSplitter>
#include <QFont>

#include "../inc/userdata.h"
#include "../inc/newuserdialog.h"
#include "../inc/newpassworddialog.h"
#include "../build/ui_mainwindow.h"
#include "../inc/user.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QListView* userColumn;
    QStringListModel* userColumnModel;

public slots:

private slots:
    void open();
    void createNewUser();
    void createNewPassword();
    void saveAs();
    void userSelected(const QItemSelection &selectedUser, const QItemSelection &deselectedUser);
    void addUserEntry(QString usernameToAdd);
    void removeSelectedUserEntry();
    void updateUserColumn(QString usernameToSelect);
    void updatePasswordColumn(QString newServiceName);
    void updateDetailsPane();

private:
    Ui::MainWindow* ui;
    void createUi();

    UserData* userdata;

    QGridLayout* mainLayout;

    QListView* passwordColumn;
    QStringListModel* passwordColumnModel;

    QLabel* serviceName;
    QLabel* username;
    QLabel* password;
    QLabel* notes;
};

#endif // MAINWINDOW_H
