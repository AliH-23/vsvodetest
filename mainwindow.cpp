#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "ProductsWindow.h"
#include "CustomersWindow.h"
#include "SalesWindow.h"
#include "InstallmentsWindow.h"
#include "ReportsWindow.h"

MainWindow::MainWindow(const QString &role, QWidget *parent)
    : QMainWindow(parent), role(role)
{
    setupMenus();
}

MainWindow::~MainWindow() {}

void MainWindow::setupMenus()
{
    QMenu *productsMenu = menuBar()->addMenu(tr("المنتجات"));
    QAction *manageProductsAction = new QAction(tr("إدارة المنتجات"), this);
    productsMenu->addAction(manageProductsAction);
    connect(manageProductsAction, &QAction::triggered, this, &MainWindow::openProductsWindow);

    QMenu *customersMenu = menuBar()->addMenu(tr("العملاء"));
    QAction *manageCustomersAction = new QAction(tr("إدارة العملاء"), this);
    customersMenu->addAction(manageCustomersAction);
    connect(manageCustomersAction, &QAction::triggered, this, &MainWindow::openCustomersWindow);

    QMenu *salesMenu = menuBar()->addMenu(tr("البيع المباشر"));
    QAction *manageSalesAction = new QAction(tr("إدارة عمليات البيع"), this);
    salesMenu->addAction(manageSalesAction);
    connect(manageSalesAction, &QAction::triggered, this, &MainWindow::openSalesWindow);

    QMenu *installmentsMenu = menuBar()->addMenu(tr("البيع بالأقساط"));
    QAction *manageInstallmentsAction = new QAction(tr("إدارة عمليات التقسيط"), this);
    installmentsMenu->addAction(manageInstallmentsAction);
    connect(manageInstallmentsAction, &QAction::triggered, this, &MainWindow::openInstallmentsWindow);

    QMenu *reportsMenu = menuBar()->addMenu(tr("التقارير"));
    QAction *summaryReportAction = new QAction(tr("تقرير ملخص"), this);
    reportsMenu->addAction(summaryReportAction);
    connect(summaryReportAction, &QAction::triggered, this, &MainWindow::openReportsWindow);
    // يمكن إضافة إجراءات (Actions) لاحقًا لكل قائمة
}

void MainWindow::openProductsWindow()
{
    if (!productsWindow) {
        productsWindow = new ProductsWindow(role, this);
    }
    productsWindow->show();
    productsWindow->raise();
    productsWindow->activateWindow();
}

void MainWindow::openCustomersWindow()
{
    if (!customersWindow) {
        customersWindow = new CustomersWindow(role, this);
    }
    customersWindow->show();
    customersWindow->raise();
    customersWindow->activateWindow();
}

void MainWindow::openSalesWindow()
{
    if (!salesWindow) {
        
        salesWindow = new SalesWindow(role, this);
    }
    salesWindow->show();
    salesWindow->raise();
    salesWindow->activateWindow();
}

void MainWindow::openInstallmentsWindow()
{
    if (!installmentsWindow) {
        installmentsWindow = new InstallmentsWindow(role, this);
    }
    installmentsWindow->show();
    installmentsWindow->raise();
    installmentsWindow->activateWindow();
}

void MainWindow::openReportsWindow()
{
    if (!reportsWindow) {
        reportsWindow = new ReportsWindow(this);
    }
    reportsWindow->show();
    reportsWindow->raise();
    reportsWindow->activateWindow();
}
