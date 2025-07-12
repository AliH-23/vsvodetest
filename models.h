#ifndef MODELS_H
#define MODELS_H

#include <QString>

struct Product {
    QString name;
    double price;
    int quantity;
};

struct Customer {
    QString name;
    QString phone;
    QString address;
};

#endif // MODELS_H
