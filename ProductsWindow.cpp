#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include "ProductsWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

ProductsWindow::ProductsWindow(const QString &role, QWidget *parent)
    : QWidget(parent), role(role)
{
    setWindowTitle(tr("إدارة المنتجات"));
    setupUI();
    loadProducts();
}

void ProductsWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText(tr("بحث عن منتج..."));
    mainLayout->addWidget(searchEdit);
    table = new QTableWidget(this);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({tr("الاسم"), tr("السعر"), tr("الكمية")});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(table);
    connect(searchEdit, &QLineEdit::textChanged, this, &ProductsWindow::filterProducts);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    addBtn = new QPushButton(tr("إضافة منتج"), this);
    editBtn = new QPushButton(tr("تعديل"), this);
    deleteBtn = new QPushButton(tr("حذف"), this);
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(editBtn);
    btnLayout->addWidget(deleteBtn);
    mainLayout->addLayout(btnLayout);

    connect(addBtn, &QPushButton::clicked, this, &ProductsWindow::addProduct);
    connect(editBtn, &QPushButton::clicked, this, &ProductsWindow::editProduct);
    connect(deleteBtn, &QPushButton::clicked, this, &ProductsWindow::deleteProduct);
}

void ProductsWindow::filterProducts(const QString &text)
{
    for (int i = 0; i < table->rowCount(); ++i) {
        bool match = table->item(i, 0)->text().contains(text, Qt::CaseInsensitive);
        table->setRowHidden(i, !match);
    }
}

void ProductsWindow::loadProducts()
{
    products.clear();
    QFile file("products.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray arr = doc.array();
        for (const QJsonValue &val : arr) {
            QJsonObject obj = val.toObject();
            products.append({obj["name"].toString(), obj["price"].toDouble(), obj["quantity"].toInt()});
        }
        file.close();
    }
    table->setRowCount(products.size());
    for (int i = 0; i < products.size(); ++i) {
        table->setItem(i, 0, new QTableWidgetItem(products[i].name));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(products[i].price)));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(products[i].quantity)));
    }
}

void ProductsWindow::saveProducts()
{
    QJsonArray arr;
    for (const auto &p : products) {
        QJsonObject obj;
        obj["name"] = p.name;
        obj["price"] = p.price;
        obj["quantity"] = p.quantity;
        arr.append(obj);
    }
    QFile file("products.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(arr);
        file.write(doc.toJson());
        file.close();
    }
}

void ProductsWindow::addProduct()
{
    showAddProductDialog();
}

void ProductsWindow::showAddProductDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("إضافة منتج جديد"));
    QFormLayout form(&dialog);
    QLineEdit nameEdit, priceEdit, qtyEdit;
    form.addRow(tr("اسم المنتج:"), &nameEdit);
    form.addRow(tr("السعر:"), &priceEdit);
    form.addRow(tr("الكمية:"), &qtyEdit);
    QPushButton okBtn(tr("إضافة"));
    QPushButton cancelBtn(tr("إلغاء"));
    QHBoxLayout btnLayout;
    btnLayout.addWidget(&okBtn);
    btnLayout.addWidget(&cancelBtn);
    form.addRow(&btnLayout);
    QObject::connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    QObject::connect(&okBtn, &QPushButton::clicked, [&]() {
        if (nameEdit.text().isEmpty() || priceEdit.text().isEmpty() || qtyEdit.text().isEmpty()) {
            QMessageBox::warning(&dialog, tr("خطأ"), tr("يرجى تعبئة جميع الحقول"));
            return;
        }
        bool ok1, ok2;
        double price = priceEdit.text().toDouble(&ok1);
        int qty = qtyEdit.text().toInt(&ok2);
        if (!ok1 || !ok2) {
            QMessageBox::warning(&dialog, tr("خطأ"), tr("القيم غير صحيحة"));
            return;
        }
        products.append({nameEdit.text(), price, qty});
        saveProducts();
        loadProducts();
        dialog.accept();
    });
    dialog.exec();
}

void ProductsWindow::editProduct()
{
    int row = table->currentRow();
    if (row < 0 || row >= products.size()) {
        QMessageBox::warning(this, tr("خطأ"), tr("يرجى اختيار منتج للتعديل"));
        return;
    }
    QDialog dialog(this);
    dialog.setWindowTitle(tr("تعديل المنتج"));
    QFormLayout form(&dialog);
    QLineEdit nameEdit(products[row].name), priceEdit(QString::number(products[row].price)), qtyEdit(QString::number(products[row].quantity));
    form.addRow(tr("اسم المنتج:"), &nameEdit);
    form.addRow(tr("السعر:"), &priceEdit);
    form.addRow(tr("الكمية:"), &qtyEdit);
    QPushButton okBtn(tr("حفظ")), cancelBtn(tr("إلغاء"));
    QHBoxLayout btnLayout;
    btnLayout.addWidget(&okBtn);
    btnLayout.addWidget(&cancelBtn);
    form.addRow(&btnLayout);
    QObject::connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    QObject::connect(&okBtn, &QPushButton::clicked, [&]() {
        if (nameEdit.text().isEmpty() || priceEdit.text().isEmpty() || qtyEdit.text().isEmpty()) {
            QMessageBox::warning(&dialog, tr("خطأ"), tr("يرجى تعبئة جميع الحقول"));
            return;
        }
        bool ok1, ok2;
        double price = priceEdit.text().toDouble(&ok1);
        int qty = qtyEdit.text().toInt(&ok2);
        if (!ok1 || !ok2) {
            QMessageBox::warning(&dialog, tr("خطأ"), tr("القيم غير صحيحة"));
            return;
        }
        products[row] = {nameEdit.text(), price, qty};
        saveProducts();
        loadProducts();
        dialog.accept();
    });
    dialog.exec();
}

void ProductsWindow::deleteProduct()
{
    int row = table->currentRow();
    if (row < 0 || row >= products.size()) {
        QMessageBox::warning(this, tr("خطأ"), tr("يرجى اختيار منتج للحذف"));
        return;
    }
    if (QMessageBox::question(this, tr("تأكيد الحذف"), tr("هل أنت متأكد من حذف المنتج؟")) == QMessageBox::Yes) {
        products.removeAt(row);
        saveProducts();
        loadProducts();
    }
}