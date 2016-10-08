#include "../inc/userdata.h"
#include "../inc/pwentry.h"
#include "../inc/mainwindow.h"

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

    if (version < 2) {
        return 1;
    }

    quint16 num_users, end_of_entry, end_of_file, num_passwords;
    QString username;
    QByteArray auth_salt, key_salt, iv, auth_hash, encrypted_data;

    file_stream >> num_users;

    QVector<User> new_users;
    QVector<PwEntry> password_entries;

    int i, j, rval;
    for (i = 0; i < num_users; i++) {
        file_stream >> username;
        file_stream >> auth_salt;
        file_stream >> key_salt;
        file_stream >> iv;
        file_stream >> auth_hash;

        file_stream >> num_passwords;
        password_entries.clear();

        for (j = 0; j < num_passwords; j++) {
            file_stream >> encrypted_data;
            PwEntry *password_entry = new PwEntry(encrypted_data);
            password_entries.append(*password_entry);
        }

        file_stream >> end_of_entry;

        if (end_of_entry != 0xE0E0) {
            // invalid entry
            return 1;
        }

        User *user = new User(username, auth_salt, key_salt, iv, auth_hash, password_entries);
        new_users.append(*user);
    }

    file_stream >> end_of_file;
    if (end_of_file != 0xE0F0) {
        // invalid file footer
        return 1;
    }

    users.clear();

    // add parsed users to userdata
    for (i = 0; i < new_users.length(); i++) {
        rval = AddNewUser(new_users[i]);
        if (rval != 0) {
            // unable to add new user to userdata
            return 1;
        }
    }

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
    quint16 version = 2;
    file_stream << version;

    quint16 num_users = users.size();
    quint16 end_of_entry = 0xE0E0;
    quint16 end_of_file = 0xE0F0;
    quint16 num_passwords;

    file_stream << num_users;
    int i, j;
    for (i = 0; i < num_users; i++) {

        // users[i].EncryptAllPwEntries();
        bool authenticated = false;
        while (!authenticated) {
            bool accepted;

            // TODO: Refactor ---
            MainWindow* mainWindow;
            QWidgetList widgets = QApplication::topLevelWidgets();
            for (QWidgetList::iterator i = widgets.begin(); i != widgets.end(); ++i) {
                if ((*i)->objectName() == "MainWindow") {
                    mainWindow = (MainWindow*) (*i);
                    break;
                }
            }

            if (!mainWindow) {
                return 1;
            }
            // ------------

            QString password = QInputDialog::getText(mainWindow, "Encryption", "Password for " + QString(users[i].username) + ":",
                                                     QLineEdit::Password, "", &accepted);

            if (!accepted) {
                return 1;
            }

            authenticated = users[i].Authenticate(password, User::Encrypt);
        }

        file_stream << users[i].username;
        file_stream << users[i].auth_salt;
        file_stream << users[i].key_salt;
        file_stream << users[i].iv;
        file_stream << users[i].auth_hash;

        num_passwords = users[i].password_entries.size();
        file_stream << num_passwords;

        for (j = 0; j < num_passwords; j++) {
            file_stream << users[i].password_entries[j].encrypted_data;
        }

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
    emit userDataChanged(user.username);

    return 0;
}

int UserData::DeleteUser(QString username) {
    int i;
    for (i = 0; i < users.size(); i++) {
        if (users[i].username == username) {
            users.remove(i);
            emit userDataChanged("");
            return 0;
        }
    }

    // user not found
    return 1;
}

int UserData::AddNewPwEntry(User* user, PwEntry password_entry) {
    user->AddPwEntry(password_entry);
    emit PwEntryChanged(password_entry.service_name);

    return 0;
}

QVector<User>* UserData::GetUsers() {
    return &users;
}

User* UserData::GetUser(QString username) {
    int i;
    for (i = 0; i < users.size(); i++) {
        if (users[i].username == username) {
            return &users[i];
        }
    }

    return 0;
}

