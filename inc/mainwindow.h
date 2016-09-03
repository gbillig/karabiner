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
    void createNew();
    void saveAs();
    void updateSidebar();

private:
    Ui::MainWindow* ui;
    void createUi();

    UserData* userdata;

    QGridLayout* mainLayout;

    QListView* userSidebar;
    QStringListModel* userSidebarModel;
};

#endif // MAINWINDOW_H
