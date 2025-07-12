#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QFormLayout>
#include "InstallmentsWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

InstallmentsWindow::InstallmentsWindow(const QString &role, QWidget *parent)
    : QWidget(parent), role(role)
{
    setWindowTitle(tr("البيع بالأقساط"));
    setupUI();
    loadInstallments();
    if (role != "admin") {
        addBtn->setEnabled(false);
        editBtn->setEnabled(false);
        deleteBtn->setEnabled(false);
    }
}

void InstallmentsWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText(tr("بحث في عمليات التقسيط..."));
    mainLayout->addWidget(searchEdit);
    table = new QTableWidget(this);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({tr("العميل"), tr("المنتج"), tr("المقدم"), tr("عدد الأقساط"), tr("قيمة القسط"), tr("الإجمالي")});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(table);
    connect(searchEdit, &QLineEdit::textChanged, this, &InstallmentsWindow::filterInstallments);
void InstallmentsWindow::filterInstallments(const QString &text)
{
    for (int i = 0; i < table->rowCount(); ++i) {
        bool match = table->item(i, 0)->text().contains(text, Qt::CaseInsensitive)
            || table->item(i, 1)->text().contains(text, Qt::CaseInsensitive);
        table->setRowHidden(i, !match);
    }
}

    QHBoxLayout *btnLayout = new QHBoxLayout();
    addBtn = new QPushButton(tr("إضافة عملية تقسيط"), this);
    editBtn = new QPushButton(tr("تعديل"), this);
    deleteBtn = new QPushButton(tr("حذف"), this);
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(editBtn);
    btnLayout->addWidget(deleteBtn);
    mainLayout->addLayout(btnLayout);

    connect(addBtn, &QPushButton::clicked, this, &InstallmentsWindow::addInstallment);
    connect(editBtn, &QPushButton::clicked, this, &InstallmentsWindow::editInstallment);
    connect(deleteBtn, &QPushButton::clicked, this, &InstallmentsWindow::deleteInstallment);
}


void InstallmentsWindow::loadInstallments()
{
    installments.clear();
    QFile file("installments.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray arr = doc.array();
        for (const QJsonValue &val : arr) {
            QJsonObject obj = val.toObject();
            installments.append({obj["customer"].toString(), obj["product"].toString(), obj["upfront"].toDouble(), obj["count"].toInt(), obj["value"].toDouble(), obj["total"].toDouble()});
        }
        file.close();
    }
    table->setRowCount(installments.size());
    for (int i = 0; i < installments.size(); ++i) {
        table->setItem(i, 0, new QTableWidgetItem(installments[i].customer));
        table->setItem(i, 1, new QTableWidgetItem(installments[i].product));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(installments[i].upfront)));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(installments[i].count)));
        table->setItem(i, 4, new QTableWidgetItem(QString::number(installments[i].value)));
        table->setItem(i, 5, new QTableWidgetItem(QString::number(installments[i].total)));
    }
}

void InstallmentsWindow::saveInstallments()
{
    QJsonArray arr;
    for (const auto &ins : installments) {
        QJsonObject obj;
        obj["customer"] = ins.customer;
        obj["product"] = ins.product;
        obj["upfront"] = ins.upfront;
        obj["count"] = ins.count;
        obj["value"] = ins.value;
        obj["total"] = ins.total;
        arr.append(obj);
    }
    QFile file("installments.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(arr);
        file.write(doc.toJson());
        file.close();
    }
}

void InstallmentsWindow::addInstallment()
{
    showAddInstallmentDialog();
}

void InstallmentsWindow::showAddInstallmentDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("إضافة عملية تقسيط جديدة"));
    QFormLayout form(&dialog);
    QLineEdit customerEdit, productEdit, upfrontEdit, countEdit, valueEdit, totalEdit;
    form.addRow(tr("اسم العميل:"), &customerEdit);
    form.addRow(tr("اسم المنتج:"), &productEdit);
    form.addRow(tr("المقدم:"), &upfrontEdit);
    form.addRow(tr("عدد الأقساط:"), &countEdit);
    form.addRow(tr("قيمة القسط:"), &valueEdit);
    form.addRow(tr("الإجمالي:"), &totalEdit);
    QPushButton okBtn(tr("إضافة"));
    QPushButton cancelBtn(tr("إلغاء"));
    QHBoxLayout btnLayout;
    btnLayout.addWidget(&okBtn);
    btnLayout.addWidget(&cancelBtn);
    form.addRow(&btnLayout);
    QObject::connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    QObject::connect(&okBtn, &QPushButton::clicked, [&]() {
        if (customerEdit.text().isEmpty() || productEdit.text().isEmpty() || upfrontEdit.text().isEmpty() || countEdit.text().isEmpty() || valueEdit.text().isEmpty() || totalEdit.text().isEmpty()) {
            QMessageBox::warning(&dialog, tr("خطأ"), tr("يرجى تعبئة جميع الحقول"));
            return;
        }
        bool ok1, ok2, ok3, ok4;
        double upfront = upfrontEdit.text().toDouble(&ok1);
        int count = countEdit.text().toInt(&ok2);
        double value = valueEdit.text().toDouble(&ok3);
        double total = totalEdit.text().toDouble(&ok4);
        if (!ok1 || !ok2 || !ok3 || !ok4) {
            QMessageBox::warning(&dialog, tr("خطأ"), tr("القيم غير صحيحة"));
            return;
        }
        installments.append({customerEdit.text(), productEdit.text(), upfront, count, value, total});
        saveInstallments();
        loadInstallments();
        dialog.accept();
    });
    dialog.exec();
}

void InstallmentsWindow::editInstallment()
{
    int row = table->currentRow();
    if (row < 0 || row >= installments.size()) {
        QMessageBox::warning(this, tr("خطأ"), tr("يرجى اختيار عملية تقسيط للتعديل"));
        return;
    }
    QDialog dialog(this);
    dialog.setWindowTitle(tr("تعديل عملية التقسيط"));
    QFormLayout form(&dialog);
    QLineEdit customerEdit(installments[row].customer), productEdit(installments[row].product), upfrontEdit(QString::number(installments[row].upfront)), countEdit(QString::number(installments[row].count)), valueEdit(QString::number(installments[row].value)), totalEdit(QString::number(installments[row].total));
    form.addRow(tr("اسم العميل:"), &customerEdit);
    form.addRow(tr("اسم المنتج:"), &productEdit);
    form.addRow(tr("المقدم:"), &upfrontEdit);
    form.addRow(tr("عدد الأقساط:"), &countEdit);
    form.addRow(tr("قيمة القسط:"), &valueEdit);
    form.addRow(tr("الإجمالي:"), &totalEdit);
    QPushButton okBtn(tr("حفظ")), cancelBtn(tr("إلغاء"));
    QHBoxLayout btnLayout;
    btnLayout.addWidget(&okBtn);
    btnLayout.addWidget(&cancelBtn);
    form.addRow(&btnLayout);
    QObject::connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    QObject::connect(&okBtn, &QPushButton::clicked, [&]() {
        if (customerEdit.text().isEmpty() || productEdit.text().isEmpty() || upfrontEdit.text().isEmpty() || countEdit.text().isEmpty() || valueEdit.text().isEmpty() || totalEdit.text().isEmpty()) {
            QMessageBox::warning(&dialog, tr("خطأ"), tr("يرجى تعبئة جميع الحقول"));
            return;
        }
        bool ok1, ok2, ok3, ok4;
        double upfront = upfrontEdit.text().toDouble(&ok1);
        int count = countEdit.text().toInt(&ok2);
        double value = valueEdit.text().toDouble(&ok3);
        double total = totalEdit.text().toDouble(&ok4);
        if (!ok1 || !ok2 || !ok3 || !ok4) {
            QMessageBox::warning(&dialog, tr("خطأ"), tr("القيم غير صحيحة"));
            return;
        }
        installments[row] = {customerEdit.text(), productEdit.text(), upfront, count, value, total};
        saveInstallments();
        loadInstallments();
        dialog.accept();
    });
    dialog.exec();
}

void InstallmentsWindow::deleteInstallment()
{
    int row = table->currentRow();
    if (row < 0 || row >= installments.size()) {
        QMessageBox::warning(this, tr("خطأ"), tr("يرجى اختيار عملية تقسيط للحذف"));
        return;
    }
    if (QMessageBox::question(this, tr("تأكيد الحذف"), tr("هل أنت متأكد من حذف عملية التقسيط؟")) == QMessageBox::Yes) {
        installments.removeAt(row);
        saveInstallments();
        loadInstallments();
    }
}
