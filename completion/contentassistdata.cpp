/*
   Copyright (C) 2009 Niko Sams <niko.sams@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "contentassistdata.h"

#include <KDE/KStandardDirs>
#include <QtCore/QXmlStreamReader>
#include <QFile>

using namespace Css;

Css::ContentAssistData::ContentAssistData()
{
    QString fileName = KStandardDirs::locate("data", "kdevcsssupport/completion/CSS.xml");
    QFile f(fileName);
    Q_ASSERT(f.exists());
    f.open(QIODevice::ReadOnly);
    QXmlStreamReader xml(&f);

    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == "elements") {
            while (!xml.atEnd()) {
                xml.readNext();
                if (xml.isEndElement() && xml.name() == "elements") break;
                if (xml.isStartElement() && xml.name() == "element") {
                    Element el;
                    el.name = xml.attributes().value("name").toString();
                    el.fullName = xml.attributes().value("full-name").toString();
                    while (!xml.atEnd()) {
                        xml.readNext();
                        if (xml.isEndElement() && xml.name() == "element") break;
                        if (xml.isStartElement() && xml.name() == "description") {
                            el.description = xml.readElementText();
                        }
                        if (xml.isStartElement() && xml.name() == "browsers") {
                            while (!xml.atEnd()) {
                                xml.readNext();
                                if (xml.isEndElement() && xml.name() == "browsers") break;
                                if (xml.isStartElement() && xml.name() == "browser") {
                                    Browser br;
                                    br.platform = xml.attributes().value("platform").toString();
                                    br.version = xml.attributes().value("version").toString();
                                    br.os = xml.attributes().value("os").toString();
                                    while (!xml.atEnd()) {
                                        xml.readNext();
                                        if (xml.isEndElement() && xml.name() == "browser") break;
                                        if (xml.isStartElement() && xml.name() == "description") {
                                            br.description = xml.readElementText();
                                        }
                                    }
                                    el.browsers << br;
                                }
                            }
                        }
                        if (xml.isStartElement() && xml.name() == "example") {
                            el.example = xml.readElementText();
                        }
                        if (xml.isStartElement() && xml.name() == "remarks") {
                            el.remarks = xml.readElementText();
                        }
                        if (xml.isStartElement() && xml.name() == "fields") {
                            while (!xml.atEnd()) {
                                xml.readNext();
                                if (xml.isEndElement() && xml.name() == "fields") break;
                                if (xml.isStartElement() && xml.name() == "field") {
                                    el.fields << xml.attributes().value("name").toString();
                                }
                            }
                        }
                        m_elements[el.name] = el;
                    }
                }
            }
        }
    }
}
