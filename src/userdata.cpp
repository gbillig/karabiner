#include "../inc/userdata.h"

UserData::UserData()
{
    //users = new QVector<User>;
}

UserData* UserData::userdata_instance = 0;

UserData* UserData::GetInstance()
{
    if (!userdata_instance) {
        userdata_instance = new UserData;
    }

    return userdata_instance;
}

int UserData::ParseUserFile(QString filepath)
{
    QFile file(filepath);
    file.open(QIODevice::ReadOnly);
    QDataStream file_stream(&file);    // read the data serialized from the file

    // read and check the header
    quint32 magic;
    file_stream >> magic;
    if (magic != 0x67EB67EB) {
        // invalid file header
        return 1;
    }

    // read the version
    quint16 version;
    file_stream >> version;

    quint16 num_entries, end_of_entry, end_of_file;
    QString username;
    QByteArray auth_salt, key_salt, iv, auth_hash;

    file_stream >> num_entries;

    QVector<User> new_users;

    int i;
    for (i = 0; i < num_entries; i++) {
        file_stream >> username;
        file_stream >> auth_salt;
        file_stream >> key_salt;
        file_stream >> iv;
        file_stream >> auth_hash;
        file_stream >> end_of_entry;

        if (end_of_entry != 0xE0E0) {
            // invalid entry
            return 1;
        }

        QVector<PwEntry> password_entries = QVector<PwEntry>();
        User *user = new User(username, auth_salt, key_salt, iv, auth_hash, password_entries);
        new_users.append(*user);
    }

    file_stream >> end_of_file;
    if (end_of_file != 0xE0F0) {
        // invalid file footer
        return 1;
    }

    users = new_users;
    emit userDataChanged();

    return 0;
}

int UserData::SaveUserFile(QString filepath) {
    QFile file(filepath);

    // open file with read and write permissions
    // truncate file (erase previous contents)
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QDataStream file_stream(&file);    // read the data serialized from the file

    // write the header
    quint32 magic = 0x67EB67EB;
    file_stream << magic;

    // write the version
    quint16 version = 0;
    file_stream << version;

    quint16 num_entries = users.size();
    quint16 end_of_entry = 0xE0E0;
    quint16 end_of_file = 0xE0F0;

    file_stream << num_entries;
    int i;
    for (i = 0; i < num_entries; i++) {
        file_stream << users[i].username;
        file_stream << users[i].auth_salt;
        file_stream << users[i].key_salt;
        file_stream << users[i].iv;
        file_stream << users[i].auth_hash;

        file_stream << end_of_entry;
    }

    file_stream << end_of_file;

    return 0;

}

int UserData::AddNewUser(User user) {

    int i;

    for (i = 0; i < users.size(); i++) {
        if (users[i].username == user.username) {
            // user with this username already exists!
            return 1;
        }
    }

    users.append(user);
    emit userDataChanged();

    return 0;
}

QVector<User>* UserData::GetUsers() {
    return &users;
}
