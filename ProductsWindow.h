#ifndef PRODUCTSWINDOW_H
#define PRODUCTSWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QList>
#include "models.h"

class ProductsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ProductsWindow(const QString &role, QWidget *parent = nullptr);

private slots:
    void addProduct();
    void editProduct();
    void deleteProduct();
    void filterProducts(const QString &text);
    void showAddProductDialog();

private:
    QTableWidget *table;
    QPushButton *addBtn;
    QPushButton *editBtn;
    QPushButton *deleteBtn;
    QLineEdit *searchEdit;
    QList<Product> products;
    QString role;
    void setupUI();
    void loadProducts();
    void saveProducts();
};

#endif // PRODUCTSWINDOW_H