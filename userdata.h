#ifndef USERDATA_H
#define USERDATA_H

#include "user.h"

class UserData
{

public:
    UserData();
    void Open(QString filepath);

private:
    User users[];
};

#endif // USERDATA_H
