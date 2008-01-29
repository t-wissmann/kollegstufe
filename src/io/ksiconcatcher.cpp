/***************************************************************************
 *   Copyright (C) 2007 by Thorsten Wissmann                               *
 *   E-Mail: kollegstufe@thorsten-wissmann.de                              *
 *   Homepage: www.thorsten-wissmann.de                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "ksiconcatcher.h"

#include <QDir>
#include <QApplication>

ksIconCatcher::ksIconCatcher()
{
}


ksIconCatcher::~ksIconCatcher()
{
}


    // icon catcher
QIcon ksIconCatcher::getIcon(QString name, int size)
{
    return QIcon(getIconPixmap(name, size));
}


QPixmap ksIconCatcher::getIconPixmap(QString name, int size)
{
    return getIconPixmapFromApplicationTheme(name, size);
    //return getIconPixmapFromKde3Theme(name, size);
}

QPixmap ksIconCatcher::getIconPixmapFromApplicationTheme(QString name, int size)
{
    QDir iconDir(QApplication::applicationDirPath());
    iconDir.cdUp();
    iconDir.cd("pic");
    QString filename = name;
    if(size != 128)
    {
        filename += QString::number(size);
    }
    filename += ".png";
    
    return QPixmap(iconDir.filePath(filename));
}

QPixmap ksIconCatcher::getIconPixmapFromKde3Theme(QString name, int size)
{
    QDir iconDir(QDir::home());
    iconDir.cd(".kde");
    iconDir.cd("share");
    iconDir.cd("icons");
    iconDir.cd("Flamenco");
    iconDir.cd(QString::number(size) + "x" + QString::number(size));
    iconDir.cd("actions");
    
    
    QString filename = name;
    filename += ".png";
    
    return QPixmap(iconDir.filePath(filename));
}


