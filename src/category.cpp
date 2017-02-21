#include "../inc/category.h"
#include "../inc/glbcrypto/misc.h"
#include "../inc/glbcrypto/sha.h"

Category::Category() {}

Category::Category(QString category,
           QByteArray auth_salt,
           QByteArray key_salt,
           QByteArray iv,
           QByteArray auth_hash,
           QVector<PwEntry> password_entries)
    : category(category),
      password_entries(password_entries),
      auth_salt(auth_salt),
      key_salt(key_salt),
      iv(iv),
      auth_hash(auth_hash),
      pristine(true),
      decrypted(false)
{

}

Category::Category(QString category, QString password)
    : category(category),
      pristine(false),
      decrypted(true)
{
    // create auth_salt and key_salt
    // generate crypto secure pseudo random number for iv
    // calculate auth_hash with auth_salt + password

    auth_salt = QByteArray("", 32);
    get_random((uint8_t*) auth_salt.data(), 32);

    key_salt = QByteArray("", 32);
    get_random((uint8_t*) key_salt.data(), 32);

    iv = QByteArray("", 16);
    get_random((uint8_t*) iv.data(), 16);

    QByteArray salted_password = auth_salt + password.toLatin1();
    auth_hash = QByteArray("", 32);
    sha_256((uint8_t*) auth_hash.data(), (uint8_t*) salted_password.data(), salted_password.length() * 8);
}

int Category::AddPwEntry(PwEntry password_entry) {
    int i;
    for (i = 0; i < password_entries.size(); i++) {
        if (password_entries[i].service_name == password_entry.service_name) {
            return 1;
        }
    }

    password_entries.append(password_entry);
    pristine = false;
    return 0;
}

// return 0     : auth success
// return other : auth fail
int Category::Authenticate(QString input_password, AuthenticateFlag auth_mode) {
    QByteArray input_auth_salted_password = auth_salt + input_password.toLatin1();
    QByteArray input_auth_hash = QByteArray("", 32);
    sha_256((uint8_t*) input_auth_hash.data(), (uint8_t*) input_auth_salted_password.data(), input_auth_salted_password.length() * 8);

    if (input_auth_hash != auth_hash) {
        return 1;
    }

    int rval = 0;
    if (auth_mode == Encrypt) {
        rval = EncryptAllPwEntries(input_password);
    } else if (auth_mode == Decrypt) {
        rval = DecryptAllPwEntries(input_password);
    }

    return rval;
}

int Category::EncryptAllPwEntries(QString password) {
    QByteArray key = key_salt + password.toLatin1();
    QByteArray key_hash = QByteArray("", 32);

    sha_256((uint8_t*) key_hash.data(), (uint8_t*) key.data(), key.length() * 8);

    int rval = 0;
    int i;
    for (i = 0; i < password_entries.size(); i++) {
        rval += password_entries[i].EncryptEntry(key_hash, iv);
    }

    return rval;
}

int Category::DecryptAllPwEntries(QString password) {
    QByteArray key = key_salt + password.toLatin1();
    QByteArray key_hash = QByteArray("", 32);

    sha_256((uint8_t*) key_hash.data(), (uint8_t*) key.data(), key.length() * 8);

    int rval = 0;
    int i;
    for (i = 0; i < password_entries.size(); i++) {
        password_entries[i].DecryptEntry(key_hash, iv);
    }

    decrypted = true;

    return rval;
}

void Category::SerializeCategory(QDataStream* stream) {
    *stream << category;
    *stream << auth_salt;
    *stream << key_salt;
    *stream << iv;
    *stream << auth_hash;

    quint16 num_passwords = password_entries.size();
    *stream << num_passwords;

    for (int i = 0; i < num_passwords; i++) {
        *stream << password_entries[i].encrypted_data;
    }
}

bool Category::isPristine() {
    return pristine;
}

bool Category::isDecrypted() {
    return decrypted;
}
