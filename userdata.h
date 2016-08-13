#ifndef USERDATA_H
#define USERDATA_H

#include "user.h"
#include "QFile"
#include "QDataStream"

class UserData
{

public:
    UserData();
    int ReadFromFile(QString filepath);

private:
    User users[];
};

#endif // USERDATA_H
