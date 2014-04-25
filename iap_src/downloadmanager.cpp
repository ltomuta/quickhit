/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

#include "downloadmanager.h"

#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <QtGui/QApplication>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkConfiguration>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QStringList>

#include "product.h"


/*!
  \class DownloadManager
  \brief Manages the downloads of purchased products.
*/


/*!
  Constructor.
*/
DownloadManager::DownloadManager(QObject *parent)
    : QObject(parent),
      m_manager(0),
      m_session(0),
      m_scriptEngine(0)
{
    m_manager = new QNetworkAccessManager(this);

    QNetworkConfigurationManager cfgManager;
    QNetworkConfiguration cfg = cfgManager.defaultConfiguration();
    m_session = new QNetworkSession(cfg, this);
    m_session->open();
    m_session->waitForOpened(10000);
    connect(m_manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));

    m_scriptEngine = new QScriptEngine(this);
}


/*!
  Destructor.
*/
DownloadManager::~DownloadManager()
{
    cancel(); // Aborts and deletes all requests
    m_session->close();
}


/*!
  Requests product IDs from \a url.
*/
void DownloadManager::requestProductsIds(const QUrl &url)
{
    QNetworkRequest request(url);
    Request *req = new Request();
    req->type = Request::EProductIds;
    req->netRequest = m_manager->get(request);
    m_currentDownloads.append(req);
}


/*!
  Starts downloading a .sis file. The correct file is identified with
  \a productId and the purchase is validated with \a purchaseTicket.
*/
void DownloadManager::downloadSis(QString productId,
                                  QString purchaseTicket,
                                  const QUrl &url)
{
    QNetworkRequest request(url);
    Request* req = new Request();
    req->type = Request::EDownloadSis;
    req->id = productId;

    QString bodyString;
    bodyString.append("{");
    bodyString.append("\"purchaseTicket\":");
    bodyString.append("\""+purchaseTicket+"\"");
    bodyString.append("}");

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader,
                      QString::number(bodyString.length()));
    request.setRawHeader("User-Agent","QuickHit");

    QByteArray bodyBytes = bodyString.toAscii();
    req->netRequest = m_manager->post(request,bodyBytes);
    QObject::connect(req->netRequest, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(onDownloadProgress(qint64,qint64)));
    m_currentDownloads.append(req);
}


/*!
  Returns a request from the list of downloads by \a reply or NULL if not
  found.
*/
Request *DownloadManager::findRequestForReply(QNetworkReply *reply)
{
    for (int i = 0; i < m_currentDownloads.count(); i++) {
        if (m_currentDownloads[i]->netRequest == reply) {
            return m_currentDownloads[i];
        }
    }

    return 0;
}


/*!
  Removes \a request from the list of downloads if found. Note that the
  removed instance is not destroyed.
*/
void DownloadManager::removeRequest(Request *request)
{
    if (request) {
        for (int i = 0; i < m_currentDownloads.count(); i++) {
            if (m_currentDownloads[i]->netRequest == request->netRequest) {
                m_currentDownloads.removeAt(i);
                break;
            }
        }
    }
}


/*!
  Handles a finished download.
*/
void DownloadManager::downloadFinished(QNetworkReply *reply)
{
    Request *request = findRequestForReply(reply);
    if (request) {
        if (reply->error() != QNetworkReply::NoError) {
            // ERROR
            emit downloadCompleted(request->type, request->id, "", reply->error());
        }
        else {
            // NO ERRORS
            switch (request->type) {
            case Request::EDownloadSis :
            {
                QUrl url = reply->url();
                QString filename = saveFileName(url);

                if (saveToDisk(filename, reply)) {
                    emit(downloadCompleted(request->type, request->id, filename, 0));
                }
                else {
                    // Failed to save the file to disk!
                    emit(downloadCompleted(request->type, request->id, filename, -1));
                }

                break;
            }
            case Request::EProductIds :
            {
                processJsonReply(request,reply);
                break;
            }
            };
        }
    }

    removeRequest(request);
    reply->deleteLater();
    delete request;
}


/*!
  Symbian SIS downloading progress
*/
void DownloadManager::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    QNetworkReply* nr = (QNetworkReply*)sender();
    if (nr) {
        Request* request = findRequestForReply(nr);
        if (request) {
            // Emit progress
            emit downloadProgress(request->id, bytesReceived, bytesTotal);
        }
    }
}


/*!
  Cancels and deletes all requests.
*/
void DownloadManager::cancel()
{
    Request *request(0);

    while (m_currentDownloads.count()) {
        request = m_currentDownloads[0];

        if (request && request->netRequest) {
            request->netRequest->abort();
            request->netRequest->deleteLater();
        }

        m_currentDownloads.removeAt(0);
        delete request;
    }
}


/*!
  Resolves and returns the file name (including the path), based on \a url, for
  the file to save.
*/
QString DownloadManager::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    // Verify the file name.
    if (basename.isEmpty()) {
        basename = "level.sis";
    }

    if (!basename.contains(".sis", Qt::CaseInsensitive)) {
        basename.append(".sis");
    }

    basename = QString("c:/Data/") + basename;

    if (QFile::exists(basename)) {
        // File already exists, remove.
        QFile::remove(basename);
    }

    return basename;
}


/*!
  Saves \a data to a file with \a filename. Returns true if successful, false
  otherwise.
*/
bool DownloadManager::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(data->readAll());
    file.close();
    return true;
}


/*!
  Parses and processes the JSON reply, in \a reply, received from the server.
*/
void DownloadManager::processJsonReply(Request *request, QNetworkReply *reply)
{
    switch (request->type) {
    case Request::EProductIds: {
        QByteArray bytes = reply->readAll();
        QString data(bytes);

        // JSON parsing
        QScriptValue sc =
                m_scriptEngine->evaluate("JSON.parse").call(QScriptValue(),
                                                            QScriptValueList() << data);

        QList<QObject*> productList;
        if (sc.property("products").isArray()) {
            QStringList items;
            qScriptValueToSequence(sc.property("products"), items);
            QScriptValueIterator it(sc.property("products"));

            while (it.hasNext()) {
                it.next();

                if (it.value().property("id").isNull()) {
                    break;
                }

                // Single product data exists
                Product *product = new Product();

                if (it.value().property("id").isString()) {
                    QString id = it.value().property("id").toString();
                    product->setId(id);

                    if (it.value().property("image").isString()) {
                        QString image = it.value().property("image").toString();
                        product->setThumbnail(image);
                        productList.append(product);
                    }
                    else {
                        product->setThumbnail("");
                        productList.append(product);
                    }
                }
                else {
                    delete product;
                }
            } // while ()
        }
        else {
            emit downloadCompleted(request->type, request->id, "", -1);
        }

        // Send result if products exist
        if (productList.length() > 0) {
            emit productIdsCompleted(productList);
        }

        break;
    } // case Request::EProductIds
    }; // switch (request->type)
}

