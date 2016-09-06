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
#include "userdata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

private slots:
    void open();
    void createNewUser();
    void saveAs();
    void updateUserColumn();
    void updatePasswordColumn(QItemSelection selected_item, QItemSelection previous_item);

private:
    Ui::MainWindow* ui;
    void createUi();

    UserData* userdata;

    QGridLayout* mainLayout;

    QListView* userColumn;
    QStringListModel* userColumnModel;

    QListView* passwordColumn;
    QStringListModel* passwordColumnModel;
};

#endif // MAINWINDOW_H
