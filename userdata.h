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
    int ParseUserFile(QString filepath);
    int AddNewUser(User user);

private:
    QVector<User> users;
};

#endif // USERDATA_H
