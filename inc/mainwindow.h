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
#include "../inc/newcategorydialog.h"
#include "../inc/newpassworddialog.h"
#include "../build/ui_mainwindow.h"
#include "../inc/category.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QListView* categoryColumn;
    QStringListModel* categoryColumnModel;

public slots:

private slots:
    void open();
    void createNewCategory();
    void createNewPassword();
    void saveAs();

    void addCategoryEntry(QString categoryToAdd);
    void removeSelectedCategoryEntry();
    void categorySelected(const QItemSelection &selectedCategoryItem, const QItemSelection &deselectedCategoryItem);

    void addPasswordEntry(QString newServiceName);
    void removeSelectedPasswordEntry();
    void passwordEntrySelected(const QItemSelection &selectedPasswordItem, const QItemSelection &deselectedPasswordItem);
    void refreshPasswordEntries();

    void refreshDetailsPane();
    void clearDetailsPane();

private:
    Ui::MainWindow* ui;
    void createUi();

    UserData* userdata;

    QGridLayout* mainLayout;

    QListView* passwordColumn;
    QStringListModel* passwordColumnModel;

    QPushButton* addCategory;
    QPushButton* removeCategory;
    QPushButton* addPassword;
    QPushButton* removePassword;

    QLabel* serviceName;
    QLabel* username;
    QLabel* password;
    QLabel* notes;
};

#endif // MAINWINDOW_H
