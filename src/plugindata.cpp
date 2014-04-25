/**
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

#include "plugindata.h"


/*!
  \class PluginData
  \brief Container class for plugin metadata.
*/


/*!
  Constructor.
*/
PluginData::PluginData(QObject *parent)
    : QObject(parent)
{
}


/*!
  Destructor.
*/
PluginData::~PluginData()
{
}


// Property getters and setters:

QString PluginData::displayname() const
{
    return m_displayname;
}

void PluginData::setDisplayname(const QString s)
{
    m_displayname = s;
}

QString PluginData::thumbnailPath() const
{
    return m_thumbnailpath;
}

void PluginData::setThumbnailPath(const QString s)
{
    m_thumbnailpath = s;
}

QString PluginData::filename() const
{
    return m_filename;
}

void PluginData::setFilename(const QString s)
{
    m_filename = s;
}

QString PluginData::iapid() const
{
    return m_iapid;
}

void PluginData::setIapid(const QString s)
{
    m_iapid = s;
}
