/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#include "product.h"
#include <QDebug>


/*!
  \class Product
  \brief Represents a purchasable product.
*/


/*!
  Constructor.
*/
Product::Product(QObject *parent)
    : QObject(parent)
{
}


/*!
  Destructor.
*/
Product::~Product()
{
}


// Property getters and setters

QString Product::id() const
{
    return m_productId;
}

void Product::setId(const QString s)
{
    m_productId = s;
}

QString Product::thumbnail() const
{
    return m_thumbnail;
}

void Product::setThumbnail(const QString s)
{
    m_thumbnail = s;
}

QString Product::title() const
{
    return m_title;
}

void Product::setTitle(const QString s)
{
    m_title = s;
}

QString Product::price() const
{
    return m_price;
}

void Product::setPrice(const QString s)
{
    m_price = s;
}

QString Product::url() const
{
    return m_url;
}

void Product::setUrl(const QString s)
{
    m_url = s;
}

int Product::reqId() const
{
    return m_reqId;
}

void Product::setReqId(const int i)
{
    m_reqId = i;
}


/*!
  Sets the product data according to content of \a data.
*/
void Product::setProductData(const IAPClient::ProductData data)
{
    m_productId = data.productId;
    m_title = data.title;
    m_price = data.price;
}


/*!
  Sets the product data according to content of \a dataHash.
*/
void Product::setProductData(const IAPClient::ProductDataHash dataHash)
{
    m_productId = dataHash.value("id").toString();
    m_title = dataHash.value("shortdescription").toString();
    m_price = dataHash.value("price").toString();
}


/*!
  Returns true if this product matches \a data. False otherwise.
*/
bool Product::equals(const Product *data)
{
    if (m_productId == data->id() &&
            m_title == data->title() &&
            m_price == data->price() &&
            m_url == data->url() &&
            m_reqId == data->reqId())
    {
        // It's a match!
        return true;
    }

    return false;
}

