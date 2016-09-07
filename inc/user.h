#ifndef USER_H
#define USER_H

#include <stdint.h>
#include <QString>
#include <QByteArray>
#include <QVector>
#include "../inc/pwentry.h"

class User
{

public:
    enum AuthenticateFlag {
        Auth    = 0x0000,
        Encrypt = 0x0001,
        Decrypt = 0x0002
    };

    User();
    User(QString username,
         QString password);
    User(QString username,
         QByteArray auth_salt,
         QByteArray key_salt,
         QByteArray iv,
         QByteArray auth_hash,
         QVector<PwEntry> password_entries);

    QString username;
    QByteArray auth_salt;
    QByteArray key_salt;
    QByteArray iv;
    QByteArray auth_hash;

    QVector<PwEntry> password_entries;

    bool isDecrypted;

    int Authenticate(QString password, User::AuthenticateFlag auth_mode);
    int AddPwEntry(PwEntry password_entry);

private:
    void EncryptAllPwEntries(QString password);
    void DecryptAllPwEntries(QString password);
};

#endif // USER_H
