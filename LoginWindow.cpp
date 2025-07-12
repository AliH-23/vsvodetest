#include "LoginWindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

LoginWindow::LoginWindow(UserManager *userManager, QWidget *parent)
    : QDialog(parent), userManager(userManager) {
    setWindowTitle(tr("تسجيل الدخول"));
    QFormLayout *form = new QFormLayout(this);
    userEdit = new QLineEdit(this);
    passEdit = new QLineEdit(this);
    passEdit->setEchoMode(QLineEdit::Password);
    form->addRow(tr("اسم المستخدم:"), userEdit);
    form->addRow(tr("كلمة المرور:"), passEdit);
    loginBtn = new QPushButton(tr("دخول"), this);
    form->addWidget(loginBtn);
    connect(loginBtn, &QPushButton::clicked, this, &LoginWindow::tryLogin);
}

void LoginWindow::tryLogin() {
    QString roleOut;
    if (userManager->login(userEdit->text(), passEdit->text(), roleOut)) {
        role = roleOut;
        accept();
    } else {
        QMessageBox::warning(this, tr("خطأ"), tr("بيانات الدخول غير صحيحة"));
    }
}

QString LoginWindow::getRole() const {
    return role;
}
