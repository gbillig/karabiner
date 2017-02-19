#ifndef CATEGORY_H
#define CATEGORY_H

#include <stdint.h>
#include <QString>
#include <QByteArray>
#include <QVector>
#include "../inc/pwentry.h"

class Category
{

public:
    enum AuthenticateFlag {
        Auth    = 0x0000,
        Encrypt = 0x0001,
        Decrypt = 0x0002
    };

    Category();
    Category(QString category,
         QString password);
    Category(QString category,
         QByteArray auth_salt,
         QByteArray key_salt,
         QByteArray iv,
         QByteArray auth_hash,
         QVector<PwEntry> password_entries);

    QString category;
    QVector<PwEntry> password_entries;

    bool isPristine();
    bool isDecrypted();

    int Authenticate(QString password, Category::AuthenticateFlag auth_mode);
    int AddPwEntry(PwEntry password_entry);
    void SerializeCategory(QDataStream *stream);

private:
    QByteArray auth_salt;
    QByteArray key_salt;
    QByteArray iv;
    QByteArray auth_hash;

    void EncryptAllPwEntries(QString password);
    void DecryptAllPwEntries(QString password);
    bool pristine;
    bool decrypted;

};

#endif // CATEGORY_H
