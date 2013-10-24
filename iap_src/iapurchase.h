/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef IAPURCHASE_H
#define IAPURCHASE_H

#include <QDeclarativeListProperty>
#include <QMetaType>
#include <QObject>
#include <QString>

#include <iapclient.h>

#include "request.h"

// Forward declarations
class DownloadManager;
class Product;


class IAPurchase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool iapready READ iapready NOTIFY iapCreatedAndReady)

public:
    explicit IAPurchase(QObject *parent = 0);

public:
    bool iapready() const;
    Q_INVOKABLE QVariant productDataAt(int index);
    Q_INVOKABLE int productDataCount();

public slots:
    void cancel();

    void getIapDataFromBackend(QString productName);

    // In-Application Purchase requests
    void getIapDataFromStore(QString iapId);
    void buyProduct(QString iapId);

    // Signals from DownloadManager
    void productIdsCompleted(QList<QObject*>);

private:
    Product *productFromCacheByProductId(QString productId);
    Product *productFromCacheByRequestId(int productId);

private slots:
    void createIAP();

    void onDownloadProgress(QString productId, qint64 bytesReceived, qint64 bytesTotal);
    void downloadCompleted(Request::RequestType, QString productId, QString fileName, int error);

    // In-Application Purchase specific slots
    void productDataReceived(int requestId, QString status, IAPClient::ProductDataHash);
    void purchaseCompleted(int requestId, QString status, QString purchaseTicket);
    void purchaseFlowFinished(int requestId);

signals: // Signals to the QML side for the requests
    void iapCreatedAndReady(bool ready);

    void productDataRead(QObject *product);
    void productDataReadError();

    void iapBackendDataReaded();
    void iapBackendDataReadError();

    void productPurchaseError();
    void productDoesNotExists(QString productId);
    void productPurchaseDone(QString productId);

    void downloadingProduct(QString productId);
    void downloadProgress(QString productId, qint64 bytesReceived, qint64 bytesTotal);
    void downloaded(QString productId);
    void downloadingError();

    void installingProduct(QString productId);

    void busy();

private: // Data
    IAPClient *m_iapClient; // In-Application Purchase API, owned
    DownloadManager *m_downloadMgr; // Owned
    QList<QObject*> m_productCache; // Content not owned
    int m_buyProductRequestId;
    int m_invalidProductPurchaseRequestId;
    bool m_iapReady;
};


#endif // IAPURCHASE_H
