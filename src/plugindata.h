/**
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef PLUGINDATA_H
#define PLUGINDATA_H

#include <QObject>
#include <QString>


class PluginData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayname READ displayname WRITE setDisplayname)
    Q_PROPERTY(QString thumbnailPath READ thumbnailPath WRITE setThumbnailPath)
    Q_PROPERTY(QString filename READ filename WRITE setFilename)
    Q_PROPERTY(QString iapid READ iapid WRITE setIapid)

public:
    explicit PluginData(QObject *parent = 0);
    ~PluginData();

public: // Property getters and setters
    QString displayname() const;
    void setDisplayname(const QString);
    QString thumbnailPath() const;
    void setThumbnailPath(const QString);
    QString filename() const;
    void setFilename(const QString);
    QString iapid() const;
    void setIapid(const QString);

private:
    QString m_displayname;
    QString m_thumbnailpath;
    QString m_filename;
    QString m_iapid;
};


#endif // PLUGINDATA_H
