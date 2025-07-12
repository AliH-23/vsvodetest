#include "UserManager.h"
#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

UserManager::UserManager() {
    loadUsers();
}

bool UserManager::login(const QString &username, const QString &password, QString &roleOut) {
    for (const User &u : users) {
        if (u.username == username && u.password == password) {
            roleOut = u.role;
            return true;
        }
    }
    return false;
}

bool UserManager::addUser(const User &user) {
    for (const User &u : users) {
        if (u.username == user.username)
            return false;
    }
    users.append(user);
    saveUsers();
    return true;
}

QList<User> UserManager::getUsers() const {
    return users;
}

void UserManager::loadUsers() {
    users.clear();
    QFile file("users.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray arr = doc.array();
        for (const QJsonValue &val : arr) {
            QJsonObject obj = val.toObject();
            users.append({obj["username"].toString(), obj["password"].toString(), obj["role"].toString()});
        }
        file.close();
    }
    if (users.isEmpty()) {
        users.append({"admin", "admin", "admin"});
        saveUsers();
    }
}

void UserManager::saveUsers() {
    QJsonArray arr;
    for (const auto &u : users) {
        QJsonObject obj;
        obj["username"] = u.username;
        obj["password"] = u.password;
        obj["role"] = u.role;
        arr.append(obj);
    }
    QFile file("users.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(arr);
        file.write(doc.toJson());
        file.close();
    }
}
