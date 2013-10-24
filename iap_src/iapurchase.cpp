/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#include "iapurchase.h"

#include <QDebug>
#include <QDesktopServices>
#include <QtCore/QMetaType>
#include <QtNetwork/QNetworkReply>
#include <QTimer>
#include <QUrl>

#include "downloadmanager.h"
#include "product.h"

Q_DECLARE_METATYPE(IAPClient::ProductDataList)


/*!
  \class IAPurchase
  \brief The main class for managing IAP. Provides the interface for QML code.
*/



/*!
  Constructor.
*/
IAPurchase::IAPurchase(QObject *parent) :
    QObject(parent),
    m_iapClient(0),
    m_downloadMgr(0),
    m_buyProductRequestId(0),
    m_invalidProductPurchaseRequestId(0),
    m_iapReady(false)
{
    // Register new metatypes so that these can be queued in the signal
    qRegisterMetaType<Request::RequestType>("Request::RequestType");

    // Construct the download manager and connect its signals.
    m_downloadMgr = new DownloadManager(this);
    connect(m_downloadMgr, SIGNAL(downloadCompleted(Request::RequestType, QString, QString, int)),
            this, SLOT(downloadCompleted(Request::RequestType, QString, QString, int)));
    connect(m_downloadMgr, SIGNAL(productIdsCompleted(QList<QObject*>)),
            this, SLOT(productIdsCompleted(QList<QObject*>)));
    connect(m_downloadMgr, SIGNAL(downloadProgress(QString,qint64,qint64)),
            this, SLOT(onDownloadProgress(QString,qint64,qint64)));


    // Delayed creation
    QTimer::singleShot(500, this, SLOT(createIAP()));
}


/*!
  Property getter. Returns true if IAP is ready, false otherwise.
*/
bool IAPurchase::iapready() const
{
    return m_iapReady;
}


/*!
  Returns product data at \a index as QVariant.
*/
QVariant IAPurchase::productDataAt(int index)
{
    if (index >= 0 && index < m_productCache.count()) {
        return QVariant::fromValue(m_productCache[index]);
    }

    // Invalid index!
    return QVariant();
}


/*!
  Returns the product data count in product cache list.
*/
int IAPurchase::productDataCount()
{
    return m_productCache.length();
}


/*!
  Cancels all requests and downloads.
*/
void IAPurchase::cancel()
{
    m_downloadMgr->cancel();
}


/*!
  Retrieves product specific data from the back-end by \a productName.
*/
void IAPurchase::getIapDataFromBackend(QString productName)
{
    QString request = "http://fn-iap-repo.cloudapp.net/api/" + productName;
    m_downloadMgr->requestProductsIds(QUrl(request));
    // Response to productIdsCompleted()
}


/*!
  Retrieves product specific data from the Nokia Store by \a iapId.
*/
void IAPurchase::getIapDataFromStore(QString iapId)
{
    Product *product = productFromCacheByProductId(iapId);

    if (product) {
        int requestId = m_iapClient->getProductData(iapId);
        if (requestId > 0) {
            product->setReqId(requestId);
            m_productCache.append(product);
        }
        else {
            emit productDataReadError();
        }
    }
    else {
        emit productDataReadError();
    }
}


/*!
  Tries to buy a product with \a iapId.
*/
void IAPurchase::buyProduct(QString iapId)
{
    if (m_buyProductRequestId) {
        // Purchase ongoing, reject this request
        emit busy();
        return;
    }

    Product *product = productFromCacheByProductId(iapId);

    if (!product) {
        // Product does not exist!
        emit productDoesNotExists(iapId);
        return;
    }

    // Buy the product
    int requestId = m_iapClient->purchaseProduct(iapId, IAPClient::ForcedAutomaticRestoration);
    product->setReqId(requestId);

    if (requestId > 0) {
        // Request ok
        m_buyProductRequestId = requestId;
    }
    else {
        // Error
        m_buyProductRequestId = 0;
        emit productPurchaseError();
    }
}


/*!
  Sets the product cache. This slot is called when the DownloadManager has
  retrieved the product IDs.
*/
void IAPurchase::productIdsCompleted(QList<QObject*> productList)
{
    m_productCache = productList;
    emit iapBackendDataReaded();
}


/*!
  Returns a product from the cache by \a productId or NULL if not found.
*/
Product *IAPurchase::productFromCacheByProductId(QString productId)
{
    for (int i = 0; i < m_productCache.count(); i++) {
        Product *p = (Product*)m_productCache[i];

        if (p->id() == productId) {
            return p;
        }
    }

    return 0;
}


/*!
  Returns a product from the cache by \a requestId or NULL if not found.
*/
Product *IAPurchase::productFromCacheByRequestId(int requestId)
{
    for (int i = 0; i < m_productCache.count(); i++) {
        Product *p = (Product*)m_productCache[i];

        if (p->reqId() == requestId) {
            return p;
        }
    }

    return 0;
}


/*!
  Constructs the IAP client and connects its signals to slots of this class.
*/
void IAPurchase::createIAP()
{
    m_iapClient = new IAPClient(this);

    // Connect IAP API's signals to slots
    connect(m_iapClient, SIGNAL(productDataReceived(int,QString,IAPClient::ProductDataHash)),
            this, SLOT(productDataReceived(int,QString,IAPClient::ProductDataHash)));

    connect(m_iapClient, SIGNAL(purchaseCompleted(int,QString, QString)),
            this, SLOT(purchaseCompleted(int,QString,QString)));

    connect(m_iapClient, SIGNAL(purchaseFlowFinished(int)),
            this, SLOT(purchaseFlowFinished(int)));

    // Notify that IAP is ready.
    m_iapReady = true;
    emit iapCreatedAndReady(m_iapReady);
}


/*!
 Symbian SIS downloading progress
*/
void IAPurchase::onDownloadProgress(QString productId, qint64 bytesReceived, qint64 bytesTotal)
{
    emit downloadProgress(productId, bytesReceived, bytesTotal);
}


/*!
  If download of a .sis file was successful, starts installing the file.
  Otherwise a signal indicating an error is emitted.
*/
void IAPurchase::downloadCompleted(Request::RequestType type,
                                   QString productId,
                                   QString fileName,
                                   int error)
{
    if (error == QNetworkReply::NoError) {
        // OK
        switch (type) {
        case Request::EDownloadSis: {
            // Symbian SIS downloaded and it's time to install it
            emit downloaded(productId);
            emit installingProduct(productId);

            // Install Symbian sis file
            QDesktopServices::openUrl(QUrl("file:///" + fileName, QUrl::TolerantMode));
            break;
        }
        case Request::EProductIds: {
            // No action
            break;
        }
        };
    }
    else {
        // ERROR
        switch (type) {
        case Request::EDownloadSis: {
            emit downloadingError();
            break;
        }
        case Request::EProductIds: {
            emit iapBackendDataReadError();
            break;
        }
        };
    }
}


/*!
  This slot is called when a signal is emitted by the IAP client to indicate
  that the product data has been received.
*/
void IAPurchase::productDataReceived(int requestId,
                                     QString status,
                                     IAPClient::ProductDataHash productData)
{
    if (QString::compare(status, "OK", Qt::CaseInsensitive) == 0) {
        // Find product request from our cache
        Product *product = productFromCacheByRequestId(requestId);
        if (product) {
            // Store product data from the Store to the data from the back-end.
            product->setProductData(productData);
            emit productDataRead(product);
        }
        else {
            emit productDataReadError();
        }
    }
    else {
        emit productDataReadError();
    }
}


/*!
  Starts downloading the product related .sis file from the back-end (if the
  purchase was successful).
*/
void IAPurchase::purchaseCompleted(int requestId, QString status, QString purchaseTicket)
{
    Product *product = 0;

    if (QString::compare(status, "OK", Qt::CaseInsensitive) == 0) {
        // Find product request from our cache
        product = productFromCacheByRequestId(requestId);

        if (product) {
            // Purchase done
            emit productPurchaseDone(product->id());

            // Start downloading level plugins AND send purchase ticket to
            // the back-end server
            emit downloadingProduct(product->id());

            QString productIdForDownload = product->id();

            QString request =
                    "http://fn-iap-repo.cloudapp.net/api/quickhit/"
                    + productIdForDownload;
            request = request.trimmed();
            QUrl url(request,QUrl::StrictMode);
            m_downloadMgr->downloadSis(productIdForDownload, purchaseTicket, url);
        }
        else {
            // Error
            m_invalidProductPurchaseRequestId = m_buyProductRequestId;
            productPurchaseError();
        }
    }
    else  {
        // Error
        m_invalidProductPurchaseRequestId = m_buyProductRequestId;
        productPurchaseError();
    }
}


/*!
  This slot gets called when the IAP client emits purchaseFlowFinished()
  signal.
*/
void IAPurchase::purchaseFlowFinished(int requestId)
{
    m_buyProductRequestId = 0;
    m_invalidProductPurchaseRequestId = 0;
}

