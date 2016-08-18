#ifndef USERDATA_H
#define USERDATA_H

#include "user.h"
#include "QFile"
#include "QDataStream"
#include "QVector"

class UserData
{

public:
    UserData();
    static UserData* GetInstance();

    int ParseUserFile(QString filepath);
    int AddNewUser(User user);

private:
    static UserData *userdata_instance;
    QVector<User> *users;
};

#endif // USERDATA_H
