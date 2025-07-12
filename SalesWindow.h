
#ifndef SALESWINDOW_H
#define SALESWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include "models.h"
#include "models.h"


class SalesWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SalesWindow(const QString &role, QWidget *parent = nullptr);

private slots:
    void addSale();
    void editSale();
    void deleteSale();
    void filterSales(const QString &text);
    void showAddSaleDialog();

private:
    QTableWidget *table;
    QPushButton *addBtn;
    QPushButton *editBtn;
    QPushButton *deleteBtn;
    QLineEdit *searchEdit;
    QString role;
    struct Sale {
        QString customer;
        QString product;
        int quantity;
        double total;
    };
    QList<Sale> sales;
    void setupUI();
    void loadSales();
    void saveSales();
};

#endif // SALESWINDOW_H
