
#ifndef CUSTOMERSWINDOW_H
#define CUSTOMERSWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include "models.h"
#include "models.h"


class CustomersWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CustomersWindow(const QString &role, QWidget *parent = nullptr);

private slots:
    void addCustomer();
    void editCustomer();
    void deleteCustomer();
    void filterCustomers(const QString &text);
    void showAddCustomerDialog();

private:
    QTableWidget *table;
    QPushButton *addBtn;
    QPushButton *editBtn;
    QPushButton *deleteBtn;
    QLineEdit *searchEdit;
    QString role;
    QList<Customer> customers;
    void setupUI();
    void loadCustomers();
    void saveCustomers();
};

#endif // CUSTOMERSWINDOW_H
