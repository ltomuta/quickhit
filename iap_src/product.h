/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef PRODUCT_H
#define PRODUCT_H

#include <QObject>
#include <QString>

#include <iapclient.h>


class Product : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id WRITE setId)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString price READ price WRITE setPrice)
    Q_PROPERTY(QString url READ url WRITE setUrl)
    Q_PROPERTY(QString thumbnail READ thumbnail WRITE setThumbnail)
    Q_PROPERTY(int reqId READ reqId WRITE setReqId)

public:
    explicit Product(QObject *parent = 0);
    ~Product();

public: // Property getters and setters
    QString id() const;
    void setId(const QString s);
    QString title() const;
    void setTitle(const QString s);
    QString price() const;
    void setPrice(const QString s);
    QString url() const;
    void setUrl(const QString s);
    QString thumbnail() const;
    void setThumbnail(const QString s);
    int reqId() const;
    void setReqId(const int i);

public:
    void setProductData(const IAPClient::ProductData data);
    void setProductData(const IAPClient::ProductDataHash dataHash);
    bool equals(const Product *data);

private: // Data
    QString m_productId;
    QString m_title;
    QString m_price;
    QString m_thumbnail;
    QString m_url;
    int m_reqId;
};


#endif // PRODUCT_H
