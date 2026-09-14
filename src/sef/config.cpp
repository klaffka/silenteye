//  This file is part of SilentEye.
//
//  SilentEye is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  SilentEye is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SilentEye. If not, see <http://www.gnu.org/licenses/>.

#include "config.h"

#include <QXmlStreamWriter>
#include <QBuffer>

namespace SilentEyeFramework {

    Config::Config(QObject* parent)
        : QObject(parent)
    {
        setObjectName("Config");
        m_logger = new Logger(this);
        m_isLoaded = false;
        m_filename = "se-noname.conf";
        m_filepath = "/tmp/";
    }

    Config::Config(QString filePath, QString filename,
                   bool hasExt, QObject* parent)
                       : QObject(parent)
    {
        setObjectName("Config");
        m_logger = new Logger(this);
        if (hasExt) {
            m_filename = filename;
        } else {
            m_filename = filename+".conf";
        }

        m_filepath = filePath;

        m_isLoaded = load();
    }

    Config::Config(const QString& content, QObject* parent)
        : QObject(parent)
    {
        setObjectName("Config");
        m_logger = new Logger(this);
        m_filename = "se-noname.conf";
        m_filepath = "/tmp/";

        m_content = content;
        m_isLoaded = load();
    }

    Config::Config(const Config& config)
    {
        setObjectName("Config");
        m_logger = new Logger(this);
        m_filename = config.filename();
        m_valueMap = config.values();
    }

    Config::~Config(){
      //if (!m_logger.isNull())
      //{
      //   delete m_logger.data();
      //}
    }

    QString Config::filename() const
    {
        return m_filename;
    }

    QString Config::filepath() const
    {
        return m_filepath;
    }

    QString Config::fileAbsoluteName() const
    {
        return m_filepath + m_filename;
    }


    QMap<QString, QString> Config::values() const
    {
        return m_valueMap;
    }

    bool Config::isLoaded() const
    {
        return m_isLoaded;
    }

    bool Config::contains(QString name) const
    {
        return m_valueMap.contains(name);
    }

    bool Config::isEmpty(const QString name) const
    {
        return !contains(name) || get(name).trimmed() == "";
    }

    void Config::set(QString name, QString value)
    {
        m_valueMap[name] = value;
    }

    QString Config::get(QString name) const
    {
        if (m_valueMap.contains(name))
            return m_valueMap.value(name);
        else
            return QString();
    }

    bool Config::getBool(const QString name) const
    {
        QString value = get(name);
        return value == "1" || value.toLower() == "true" || value.toLower() == "on";
    }

    int Config::getInt(const QString name) const
    {
        QString value = get(name);
        return value.toInt();
    }

    void Config::save()
    {
        /* open the output file */
        QFile file(m_filepath+m_filename);
        if( !file.open( QIODevice::WriteOnly ) )
        {
            m_logger->warning("Can't save configuration file to "
                              + fileAbsoluteName());
            return;
        }

        /* write data to the output file */
        QXmlStreamWriter xml( &file );
        xml.setAutoFormatting(true);
        xml.writeStartDocument();
        xml.writeStartElement("configuration");

        QMap<QString, QString>::const_iterator i = m_valueMap.constBegin();
        while (i != m_valueMap.constEnd()) {
            xml.writeTextElement(i.key(), i.value());
            ++i;
        }

        xml.writeEndElement();
        xml.writeEndDocument();
        file.close();
    }

    bool Config::load()
    {
        QBuffer buffer;
        QFile file;

        QXmlStreamReader xml;
        bool rootSeen = false;
        if (!m_content.isEmpty()) {
            buffer.setData(m_content.toUtf8());
            buffer.open(QIODevice::ReadOnly);
            xml.setDevice(&buffer);
        }
        else
        {
            file.setFileName(m_filepath+m_filename);
            if(!file.open( QIODevice::ReadOnly ) )
            {
                m_logger->warning("Can't open configuration file: "
                                  + fileAbsoluteName());
                return false;
            }
            xml.setDevice(&file);
        }

        while(!xml.atEnd())
        {
            xml.readNext();
            if(xml.isStartElement())
            {
                // skip the root element (e.g. <configuration> or <version>)
                if(!rootSeen)
                {
                    rootSeen = true;
                    continue;
                }
                m_valueMap[xml.name().toString()] = xml.readElementText();
            }
        }

        if (xml.hasError())
        {
            m_logger->warning("Can't load file content to xml document (XML syntax error?): "
                              + fileAbsoluteName());
            return false;
        }

        return true;
    }

    Config& Config::operator=(const Config& config)
                             {
        m_filename = config.filename();
        m_filepath = config.filepath();
        m_valueMap = config.values();
        return *this;
    }

}
