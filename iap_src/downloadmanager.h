/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkSession>
#include <QtScript>

#include "request.h"

// Forward declarations
class QNetworkReply;
class QUrl;
class Product;


class DownloadManager : public QObject
{
    Q_OBJECT

public:
    explicit DownloadManager(QObject *parent = 0);
    virtual ~DownloadManager();

public:
    void requestProductsIds(const QUrl &url);
    void downloadSis(QString productId, QString purchaseTicket, const QUrl &url);
    Request *findRequestForReply(QNetworkReply *reply);
    void removeRequest(Request *request);

public slots:
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished(QNetworkReply *reply);
    void cancel();

private:
    QString saveFileName(const QUrl &url);
    bool saveToDisk(const QString &filename, QIODevice *data);
    void processJsonReply(Request *request, QNetworkReply *reply);

signals:
    void productIdsCompleted(QList<QObject*>);
    void downloadProgress(QString productId, qint64 bytesReceived, qint64 bytesTotal);
    void downloadCompleted(Request::RequestType type, QString productId, QString fileName, int error);

private: // Data
    QNetworkAccessManager *m_manager; // Owned
    QNetworkSession *m_session; // Owned
    QScriptEngine *m_scriptEngine; // Owned
    QList<Request*> m_currentDownloads;
};

#endif // DOWNLOADMANAGER_H
