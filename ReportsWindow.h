#ifndef REPORTSWINDOW_H
#define REPORTSWINDOW_H

#include <QWidget>
#include <QTextEdit>

class ReportsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ReportsWindow(QWidget *parent = nullptr);

private:
    QTextEdit *reportArea;
    void setupUI();
    void showSummaryReport();
};

#endif // REPORTSWINDOW_H
