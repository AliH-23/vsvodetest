
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPointer>
class ProductsWindow;
class CustomersWindow;
class SalesWindow;
class InstallmentsWindow;
class ReportsWindow;


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(const QString &role, QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupMenus();
    QPointer<ProductsWindow> productsWindow;
    QPointer<CustomersWindow> customersWindow;
    QPointer<SalesWindow> salesWindow;
    QPointer<InstallmentsWindow> installmentsWindow;
    QPointer<ReportsWindow> reportsWindow;
    QString role;
private slots:
    void openProductsWindow();
    void openCustomersWindow();
    void openSalesWindow();
    void openInstallmentsWindow();
    void openReportsWindow();
};


#endif // MAINWINDOW_H
