
#ifndef INSTALLMENTSWINDOW_H
#define INSTALLMENTSWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include "models.h"
#include "models.h"


class InstallmentsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit InstallmentsWindow(const QString &role, QWidget *parent = nullptr);

private slots:
    void addInstallment();
    void editInstallment();
    void deleteInstallment();
    void filterInstallments(const QString &text);
    void showAddInstallmentDialog();

private:
    QTableWidget *table;
    QPushButton *addBtn;
    QPushButton *editBtn;
    QPushButton *deleteBtn;
    QLineEdit *searchEdit;
    QString role;
    struct Installment {
        QString customer;
        QString product;
        double upfront;
        int count;
        double value;
        double total;
    };
    QList<Installment> installments;
    void setupUI();
    void loadInstallments();
    void saveInstallments();
};

#endif // INSTALLMENTSWINDOW_H
