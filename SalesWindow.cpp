#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QFormLayout>
#include "SalesWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

SalesWindow::SalesWindow(const QString &role, QWidget *parent)
    : QWidget(parent), role(role)
{
    setWindowTitle(tr("البيع المباشر"));
    setupUI();
    loadSales();
    if (role != "admin") {
        addBtn->setEnabled(false);
        editBtn->setEnabled(false);
        deleteBtn->setEnabled(false);
    }
}

void SalesWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText(tr("بحث في عمليات البيع..."));
    mainLayout->addWidget(searchEdit);
    table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({tr("العميل"), tr("المنتج"), tr("الكمية"), tr("الإجمالي")});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(table);
    connect(searchEdit, &QLineEdit::textChanged, this, &SalesWindow::filterSales);
void SalesWindow::filterSales(const QString &text)
{
    for (int i = 0; i < table->rowCount(); ++i) {
        bool match = table->item(i, 0)->text().contains(text, Qt::CaseInsensitive)
            || table->item(i, 1)->text().contains(text, Qt::CaseInsensitive);
        table->setRowHidden(i, !match);
    }
}

    QHBoxLayout *btnLayout = new QHBoxLayout();
    addBtn = new QPushButton(tr("إضافة عملية بيع"), this);
    editBtn = new QPushButton(tr("تعديل"), this);
    deleteBtn = new QPushButton(tr("حذف"), this);
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(editBtn);
    btnLayout->addWidget(deleteBtn);
    mainLayout->addLayout(btnLayout);

    connect(addBtn, &QPushButton::clicked, this, &SalesWindow::addSale);
    connect(editBtn, &QPushButton::clicked, this, &SalesWindow::editSale);
    connect(deleteBtn, &QPushButton::clicked, this, &SalesWindow::deleteSale);
}


void SalesWindow::loadSales()
{
    sales.clear();
    QFile file("sales.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray arr = doc.array();
        for (const QJsonValue &val : arr) {
            QJsonObject obj = val.toObject();
            sales.append({obj["customer"].toString(), obj["product"].toString(), obj["quantity"].toInt(), obj["total"].toDouble()});
        }
        file.close();
    }
    table->setRowCount(sales.size());
    for (int i = 0; i < sales.size(); ++i) {
        table->setItem(i, 0, new QTableWidgetItem(sales[i].customer));
        table->setItem(i, 1, new QTableWidgetItem(sales[i].product));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(sales[i].quantity)));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(sales[i].total)));
    }
}

void SalesWindow::saveSales()
{
    QJsonArray arr;
    for (const auto &s : sales) {
        QJsonObject obj;
        obj["customer"] = s.customer;
        obj["product"] = s.product;
        obj["quantity"] = s.quantity;
        obj["total"] = s.total;
        arr.append(obj);
    }
    QFile file("sales.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(arr);
        file.write(doc.toJson());
        file.close();
    }
}

void SalesWindow::addSale()
{
    showAddSaleDialog();
}

void SalesWindow::showAddSaleDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("إضافة عملية بيع جديدة"));
    QFormLayout form(&dialog);
    QLineEdit customerEdit, productEdit, qtyEdit, totalEdit;
    form.addRow(tr("اسم العميل:"), &customerEdit);
    form.addRow(tr("اسم المنتج:"), &productEdit);
    form.addRow(tr("الكمية:"), &qtyEdit);
    form.addRow(tr("الإجمالي:"), &totalEdit);
    QPushButton okBtn(tr("إضافة"));
    QPushButton cancelBtn(tr("إلغاء"));
    QHBoxLayout btnLayout;
    btnLayout.addWidget(&okBtn);
    btnLayout.addWidget(&cancelBtn);
    form.addRow(&btnLayout);
    QObject::connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    QObject::connect(&okBtn, &QPushButton::clicked, [&]() {
        if (customerEdit.text().isEmpty() || productEdit.text().isEmpty() || qtyEdit.text().isEmpty() || totalEdit.text().isEmpty()) {
            QMessageBox::warning(&dialog, tr("خطأ"), tr("يرجى تعبئة جميع الحقول"));
            return;
        }
        bool ok1, ok2;
        int qty = qtyEdit.text().toInt(&ok1);
        double total = totalEdit.text().toDouble(&ok2);
        if (!ok1 || !ok2) {
            QMessageBox::warning(&dialog, tr("خطأ"), tr("القيم غير صحيحة"));
            return;
        }
        sales.append({customerEdit.text(), productEdit.text(), qty, total});
        saveSales();
        loadSales();
        dialog.accept();
    });
    dialog.exec();
}

void SalesWindow::editSale()
{
    int row = table->currentRow();
    if (row < 0 || row >= sales.size()) {
        QMessageBox::warning(this, tr("خطأ"), tr("يرجى اختيار عملية بيع للتعديل"));
        return;
    }
    QDialog dialog(this);
    dialog.setWindowTitle(tr("تعديل عملية البيع"));
    QFormLayout form(&dialog);
    QLineEdit customerEdit(sales[row].customer), productEdit(sales[row].product), qtyEdit(QString::number(sales[row].quantity)), totalEdit(QString::number(sales[row].total));
    form.addRow(tr("اسم العميل:"), &customerEdit);
    form.addRow(tr("اسم المنتج:"), &productEdit);
    form.addRow(tr("الكمية:"), &qtyEdit);
    form.addRow(tr("الإجمالي:"), &totalEdit);
    QPushButton okBtn(tr("حفظ")), cancelBtn(tr("إلغاء"));
    QHBoxLayout btnLayout;
    btnLayout.addWidget(&okBtn);
    btnLayout.addWidget(&cancelBtn);
    form.addRow(&btnLayout);
    QObject::connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    QObject::connect(&okBtn, &QPushButton::clicked, [&]() {
        if (customerEdit.text().isEmpty() || productEdit.text().isEmpty() || qtyEdit.text().isEmpty() || totalEdit.text().isEmpty()) {
            QMessageBox::warning(&dialog, tr("خطأ"), tr("يرجى تعبئة جميع الحقول"));
            return;
        }
        bool ok1, ok2;
        int qty = qtyEdit.text().toInt(&ok1);
        double total = totalEdit.text().toDouble(&ok2);
        if (!ok1 || !ok2) {
            QMessageBox::warning(&dialog, tr("خطأ"), tr("القيم غير صحيحة"));
            return;
        }
        sales[row] = {customerEdit.text(), productEdit.text(), qty, total};
        saveSales();
        loadSales();
        dialog.accept();
    });
    dialog.exec();
}

void SalesWindow::deleteSale()
{
    int row = table->currentRow();
    if (row < 0 || row >= sales.size()) {
        QMessageBox::warning(this, tr("خطأ"), tr("يرجى اختيار عملية بيع للحذف"));
        return;
    }
    if (QMessageBox::question(this, tr("تأكيد الحذف"), tr("هل أنت متأكد من حذف عملية البيع؟")) == QMessageBox::Yes) {
        sales.removeAt(row);
        saveSales();
        loadSales();
    }
}
