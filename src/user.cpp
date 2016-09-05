#include "../inc/user.h"
#include "../inc/glbcrypto/misc.h"
#include "../inc/glbcrypto/sha.h"

User::User()
{

}

User::User(QString username,
           QByteArray auth_salt,
           QByteArray key_salt,
           QByteArray iv,
           QByteArray auth_hash,
           QVector<PwEntry> password_entries)
    : username(username),
      auth_salt(auth_salt),
      key_salt(key_salt),
      iv(iv),
      auth_hash(auth_hash),
      password_entries(password_entries)
{

}

User::User(QString username, QString password)
    : username(username)
{
    // create auth_salt and key_salt
    // generate crypto secure pseudo random number for iv
    // calculate auth_hash with auth_salt + password

    auth_salt = QByteArray("", 32);
    get_random((uint8_t*) auth_salt.data(), 32);

    key_salt = QByteArray("", 32);
    get_random((uint8_t*) key_salt.data(), 32);

    iv = QByteArray("", 32);
    get_random((uint8_t*) iv.data(), 32);

    QByteArray salted_password = auth_salt.append(password.toLatin1());
    auth_hash = QByteArray("", 32);
    sha_256((uint8_t*) auth_hash.data(), (uint8_t*) salted_password.data(), salted_password.length() * 8);
}

// return 0 : auth success
// return 1 : auth fail
int User::Authenticate(QString input_password) {
    QByteArray input_auth_salted_password = auth_salt.append(input_password.toLatin1());
    QByteArray input_auth_hash = QByteArray("", 32);
    sha_256((uint8_t*) input_auth_hash.data(), (uint8_t*) input_auth_salted_password.data(), input_auth_salted_password.length() * 8);

    if (input_auth_hash != auth_hash) {
        return 1;
    }

    QByteArray key = key_salt.append(input_password.toLatin1());
    QByteArray key_hash = QByteArray("", 32);

    sha_256((uint8_t*) key_hash.data(), (uint8_t*) key.data(), key.length() * 8);

    DecryptAllPwEntries(key_hash);
    return 0;
}

void User::DecryptAllPwEntries(QByteArray key) {
    int i;
    for (i = 0; i < password_entries.size(); i++) {
        password_entries[i].DecryptEntry(key, iv);
    }
}
