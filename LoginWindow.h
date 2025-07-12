#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "UserManager.h"

class LoginWindow : public QDialog {
    Q_OBJECT
public:
    explicit LoginWindow(UserManager *userManager, QWidget *parent = nullptr);
    QString getRole() const;
private slots:
    void tryLogin();
private:
    QLineEdit *userEdit;
    QLineEdit *passEdit;
    QPushButton *loginBtn;
    UserManager *userManager;
    QString role;
};

#endif // LOGINWINDOW_H
