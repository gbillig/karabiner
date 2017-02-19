#include "../inc/userdata.h"
#include "../inc/pwentry.h"
#include "../inc/mainwindow.h"

UserData::UserData()
{
    //categories = new QVector<Category>;
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

    quint16 num_categories, end_of_entry, end_of_file, num_passwords;
    QString category_name;
    QByteArray auth_salt, key_salt, iv, auth_hash, encrypted_data;

    file_stream >> num_categories;

    QVector<Category> new_category;
    QVector<PwEntry> password_entries;

    int i, j, rval;
    for (i = 0; i < num_categories; i++) {
        file_stream >> category_name;
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

        Category *category = new Category(category_name, auth_salt, key_salt, iv, auth_hash, password_entries);
        new_category.append(*category);
    }

    file_stream >> end_of_file;
    if (end_of_file != 0xE0F0) {
        // invalid file footer
        return 1;
    }

    categories.clear();

    // add parsed categories to userdata
    for (i = 0; i < new_category.length(); i++) {
        rval = AddNewCategory(new_category[i]);
        if (rval != 0) {
            // unable to add new category to userdata
            return 1;
        }
    }

    file.close();
    return 0;
}

int UserData::SaveUserFile(QString filepath) {
    QByteArray* serialized_userdata = new QByteArray();
    QDataStream stream(serialized_userdata, QIODevice::WriteOnly);

    // write the header
    quint32 magic = 0x67EB67EB;
    stream << magic;

    // write the version
    quint16 version = 2;
    stream << version;

    quint16 num_categories = categories.size();
    quint16 end_of_entry = 0xE0E0;
    quint16 end_of_file = 0xE0F0;

    stream << num_categories;
    int i;
    for (i = 0; i < num_categories; i++) {
        if (!categories[i].isPristine()) {
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

                QString password = QInputDialog::getText(mainWindow, "Encryption", "Password for " + QString(categories[i].category) + ":",
                                                         QLineEdit::Password, "", &accepted);

                if (!accepted) {
                    return 1;
                }

                authenticated = categories[i].Authenticate(password, Category::Encrypt);
            }
        }

        categories[i].SerializeCategory(&stream);
        stream << end_of_entry;
    }

    stream << end_of_file;

    QFile file(filepath);

    // open file with read and write permissions
    // truncate file (erase previous contents)
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QDataStream file_stream(&file);
    file_stream.writeRawData(serialized_userdata->data(), serialized_userdata->length());
    file.close();

    return 0;

}

int UserData::AddNewCategory(Category category) {
    int i;
    for (i = 0; i < categories.size(); i++) {
        if (categories[i].category == category.category) {
            // category with this category name already exists!
            return 1;
        }
    }

    categories.append(category);
    emit categoryAdded(category.category);

    return 0;
}

int UserData::DeleteCategory(QString category) {
    int i;
    for (i = 0; i < categories.size(); i++) {
        if (categories[i].category == category) {
            categories.remove(i);
            return 0;
        }
    }

    // category not found
    return 1;
}

int UserData::AddNewPwEntry(Category* category, PwEntry password_entry) {
    category->AddPwEntry(password_entry);
    emit passwordEntryAdded(password_entry.service_name);

    return 0;
}

QVector<Category>* UserData::GetCategory() {
    return &categories;
}

Category* UserData::GetCategory(QString category) {
    int i;
    for (i = 0; i < categories.size(); i++) {
        if (categories[i].category == category) {
            return &categories[i];
        }
    }

    return 0;
}

