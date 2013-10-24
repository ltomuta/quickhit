/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef REQUEST_H
#define REQUEST_H

// Forward declaration
class QNetworkReply;


class Request
{
public:
    enum RequestType {
        EDownloadSis,
        EProductIds
    };

public: // Constructor
    Request() : netRequest(0) {}

public: // Data
    QNetworkReply *netRequest; // Reply for the request, not owned
    QString id; // Product id
    RequestType type; // Request type
};


#endif // REQUEST_H
