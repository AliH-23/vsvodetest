#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QString>
#include <QList>

struct User {
    QString username;
    QString password;
    QString role; // "admin" or "user"
};

class UserManager {
public:
    UserManager();
    bool login(const QString &username, const QString &password, QString &roleOut);
    bool addUser(const User &user);
    QList<User> getUsers() const;
private:
    QList<User> users;
    void loadUsers();
    void saveUsers();
};

#endif // USERMANAGER_H
