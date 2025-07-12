#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QFormLayout>
#include "ReportsWindow.h"
#include <QVBoxLayout>
#include <QPushButton>

ReportsWindow::ReportsWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("التقارير"));
    setupUI();
    showSummaryReport();
}

void ReportsWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    reportArea = new QTextEdit(this);
    reportArea->setReadOnly(true);
    mainLayout->addWidget(reportArea);
    QPushButton *refreshBtn = new QPushButton(tr("تحديث التقرير"), this);
    mainLayout->addWidget(refreshBtn);
    connect(refreshBtn, &QPushButton::clicked, this, &ReportsWindow::showSummaryReport);
}

void ReportsWindow::showSummaryReport()
{
    int productsCount = 0, customersCount = 0, salesCount = 0, installmentsCount = 0;
    double salesTotal = 0, installmentsTotal = 0;
    // قراءة المنتجات
    QFile pFile("products.json");
    if (pFile.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(pFile.readAll());
        productsCount = doc.array().size();
        pFile.close();
    }
    // قراءة العملاء
    QFile cFile("customers.json");
    if (cFile.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(cFile.readAll());
        customersCount = doc.array().size();
        cFile.close();
    }
    // قراءة عمليات البيع
    QFile sFile("sales.json");
    if (sFile.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(sFile.readAll());
        QJsonArray arr = doc.array();
        salesCount = arr.size();
        for (const QJsonValue &val : arr) {
            salesTotal += val.toObject()["total"].toDouble();
        }
        sFile.close();
    }
    // قراءة عمليات التقسيط
    QFile iFile("installments.json");
    if (iFile.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(iFile.readAll());
        QJsonArray arr = doc.array();
        installmentsCount = arr.size();
        for (const QJsonValue &val : arr) {
            installmentsTotal += val.toObject()["total"].toDouble();
        }
        iFile.close();
    }
    QString report = tr("عدد المنتجات: %1\nعدد العملاء: %2\nعدد عمليات البيع: %3\nإجمالي البيع المباشر: %4\nعدد عمليات التقسيط: %5\nإجمالي البيع بالأقساط: %6")
        .arg(productsCount).arg(customersCount).arg(salesCount).arg(salesTotal).arg(installmentsCount).arg(installmentsTotal);
    reportArea->setText(report);
}
