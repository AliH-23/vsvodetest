
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QInputDialog>
#include "mainwindow.h"
#include "UserManager.h"
#include "LoginWindow.h"

int main(int argc, char *argv[])
{
    // اختيار اللغة قبل إنشاء QApplication
    QStringList langs = {QObject::tr("العربية"), QObject::tr("English")};
    bool ok = false;
    QString lang = QInputDialog::getItem(nullptr, QObject::tr("اختيار اللغة / Select Language"), QObject::tr("اللغة / Language"), langs, 0, false, &ok);

    QApplication app(argc, argv);
    QTranslator translator;
    if (ok && lang == "English") {
        translator.load("translations/en.qm", ".");
    } else {
        translator.load("translations/ar.qm", ".");
    }
    app.installTranslator(&translator);

    UserManager userManager;
    LoginWindow login(&userManager);
    if (login.exec() != QDialog::Accepted)
        return 0;
    MainWindow w(login.getRole());
    w.show();
    return app.exec();
}
