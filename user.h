#ifndef USER_H
#define USER_H

#include <stdint.h>
#include <QString>
#include <QStringList>

class User
{

public:
    User();
    void PopulateFromString(QString input);

private:
    char username[];
    uint8_t salt1[];
    uint8_t salt2[];
    uint8_t iv[];
    uint8_t pw_hash[];
};

#endif // USER_H\
