#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QFormLayout>
#include "CustomersWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

CustomersWindow::CustomersWindow(const QString &role, QWidget *parent)
    : QWidget(parent), role(role)
{
    setWindowTitle(tr("إدارة العملاء"));
    setupUI();
    loadCustomers();
    if (role != "admin") {
        addBtn->setEnabled(false);
        editBtn->setEnabled(false);
        deleteBtn->setEnabled(false);
    }
}

void CustomersWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText(tr("بحث عن عميل..."));
    mainLayout->addWidget(searchEdit);
    table = new QTableWidget(this);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({tr("الاسم"), tr("رقم الهاتف"), tr("العنوان")});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(table);
    connect(searchEdit, &QLineEdit::textChanged, this, &CustomersWindow::filterCustomers);
void CustomersWindow::filterCustomers(const QString &text)
{
    for (int i = 0; i < table->rowCount(); ++i) {
        bool match = table->item(i, 0)->text().contains(text, Qt::CaseInsensitive)
            || table->item(i, 1)->text().contains(text, Qt::CaseInsensitive)
            || table->item(i, 2)->text().contains(text, Qt::CaseInsensitive);
        table->setRowHidden(i, !match);
    }
}

    QHBoxLayout *btnLayout = new QHBoxLayout();
    addBtn = new QPushButton(tr("إضافة عميل"), this);
    editBtn = new QPushButton(tr("تعديل"), this);
    deleteBtn = new QPushButton(tr("حذف"), this);
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(editBtn);
    btnLayout->addWidget(deleteBtn);
    mainLayout->addLayout(btnLayout);

    connect(addBtn, &QPushButton::clicked, this, &CustomersWindow::addCustomer);
    connect(editBtn, &QPushButton::clicked, this, &CustomersWindow::editCustomer);
    connect(deleteBtn, &QPushButton::clicked, this, &CustomersWindow::deleteCustomer);
}


void CustomersWindow::loadCustomers()
{
    customers.clear();
    QFile file("customers.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray arr = doc.array();
        for (const QJsonValue &val : arr) {
            QJsonObject obj = val.toObject();
            customers.append({obj["name"].toString(), obj["phone"].toString(), obj["address"].toString()});
        }
        file.close();
    }
    table->setRowCount(customers.size());
    for (int i = 0; i < customers.size(); ++i) {
        table->setItem(i, 0, new QTableWidgetItem(customers[i].name));
        table->setItem(i, 1, new QTableWidgetItem(customers[i].phone));
        table->setItem(i, 2, new QTableWidgetItem(customers[i].address));
    }
}

void CustomersWindow::saveCustomers()
{
    QJsonArray arr;
    for (const auto &c : customers) {
        QJsonObject obj;
        obj["name"] = c.name;
        obj["phone"] = c.phone;
        obj["address"] = c.address;
        arr.append(obj);
    }
    QFile file("customers.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(arr);
        file.write(doc.toJson());
        file.close();
    }
}

void CustomersWindow::addCustomer()
{
    showAddCustomerDialog();
}

void CustomersWindow::showAddCustomerDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("إضافة عميل جديد"));
    QFormLayout form(&dialog);
    QLineEdit nameEdit, phoneEdit, addressEdit;
    form.addRow(tr("اسم العميل:"), &nameEdit);
    form.addRow(tr("رقم الهاتف:"), &phoneEdit);
    form.addRow(tr("العنوان:"), &addressEdit);
    QPushButton okBtn(tr("إضافة"));
    QPushButton cancelBtn(tr("إلغاء"));
    QHBoxLayout btnLayout;
    btnLayout.addWidget(&okBtn);
    btnLayout.addWidget(&cancelBtn);
    form.addRow(&btnLayout);
    QObject::connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    QObject::connect(&okBtn, &QPushButton::clicked, [&]() {
        if (nameEdit.text().isEmpty() || phoneEdit.text().isEmpty() || addressEdit.text().isEmpty()) {
            QMessageBox::warning(&dialog, tr("خطأ"), tr("يرجى تعبئة جميع الحقول"));
            return;
        }
        customers.append({nameEdit.text(), phoneEdit.text(), addressEdit.text()});
        saveCustomers();
        loadCustomers();
        dialog.accept();
    });
    dialog.exec();
}

void CustomersWindow::editCustomer()
{
    int row = table->currentRow();
    if (row < 0 || row >= customers.size()) {
        QMessageBox::warning(this, tr("خطأ"), tr("يرجى اختيار عميل للتعديل"));
        return;
    }
    QDialog dialog(this);
    dialog.setWindowTitle(tr("تعديل العميل"));
    QFormLayout form(&dialog);
    QLineEdit nameEdit(customers[row].name), phoneEdit(customers[row].phone), addressEdit(customers[row].address);
    form.addRow(tr("اسم العميل:"), &nameEdit);
    form.addRow(tr("رقم الهاتف:"), &phoneEdit);
    form.addRow(tr("العنوان:"), &addressEdit);
    QPushButton okBtn(tr("حفظ")), cancelBtn(tr("إلغاء"));
    QHBoxLayout btnLayout;
    btnLayout.addWidget(&okBtn);
    btnLayout.addWidget(&cancelBtn);
    form.addRow(&btnLayout);
    QObject::connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    QObject::connect(&okBtn, &QPushButton::clicked, [&]() {
        if (nameEdit.text().isEmpty() || phoneEdit.text().isEmpty() || addressEdit.text().isEmpty()) {
            QMessageBox::warning(&dialog, tr("خطأ"), tr("يرجى تعبئة جميع الحقول"));
            return;
        }
        customers[row] = {nameEdit.text(), phoneEdit.text(), addressEdit.text()};
        saveCustomers();
        loadCustomers();
        dialog.accept();
    });
    dialog.exec();
}

void CustomersWindow::deleteCustomer()
{
    int row = table->currentRow();
    if (row < 0 || row >= customers.size()) {
        QMessageBox::warning(this, tr("خطأ"), tr("يرجى اختيار عميل للحذف"));
        return;
    }
    if (QMessageBox::question(this, tr("تأكيد الحذف"), tr("هل أنت متأكد من حذف العميل؟")) == QMessageBox::Yes) {
        customers.removeAt(row);
        saveCustomers();
        loadCustomers();
    }
}
