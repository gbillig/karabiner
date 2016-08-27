#ifndef USERDATA_H
#define USERDATA_H


#include "user.h"
#include <QFile>
#include <QDataStream>
#include <QVector>

class UserData : public QObject
{
    Q_OBJECT

public:
    UserData();
    static UserData* GetInstance();

    int SaveUserFile(QString filepath);
    int ParseUserFile(QString filepath);
    int AddNewUser(User user);

    QVector<User>* GetUsers();

signals:
    void userDataChanged();

private:
    static UserData *userdata_instance;
    QVector<User> users;
};

#endif // USERDATA_H
