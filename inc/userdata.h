#ifndef USERDATA_H
#define USERDATA_H


#include "category.h"
#include <QFile>
#include <QDataStream>
#include <QVector>
#include <QApplication>
#include <QStringList>

class UserData : public QObject
{
    Q_OBJECT

public:
    UserData();
    static UserData* GetInstance();

    int SaveUserFile(QString filepath);
    int ParseUserFile(QString filepath);
    int AddNewCategory(Category category);
    int DeleteCategory(QString category);
    int AddNewPwEntry(Category* category, PwEntry password_entry);

    QVector<Category>* GetCategory();
    Category* GetCategory(QString category);

signals:
    void categoryAdded(QString newCategory);
    void passwordEntryAdded(QString newServiceName);

private:
    static UserData *userdata_instance;
    QVector<Category> categories;
};

#endif // USERDATA_H
