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
#ifndef KSSTATISTICSITEM_H
#define KSSTATISTICSITEM_H

#include <QString>
#include <QDate>

/**
	@author Thorsten Wissmann <towi89@web.de>
*/
class ksStatisticsItem{
public:
    ksStatisticsItem();
    ksStatisticsItem(const ksStatisticsItem& newItem);
    ksStatisticsItem(QString newCaption, QDate newX, int newY, QString newInformation = "");

    ~ksStatisticsItem();
    void operator=(const ksStatisticsItem& newItem);
    bool operator<(const ksStatisticsItem& other) const;
    bool operator>(const ksStatisticsItem& other) const;
    bool operator==(const ksStatisticsItem& other) const;
    bool operator!=(const ksStatisticsItem& other) const;
    bool operator<=(const ksStatisticsItem& other) const;
    bool operator>=(const ksStatisticsItem& other) const;
    
    
    
    QString caption() const { return szCaption; };
    void setCaption(QString newCaption){ szCaption = newCaption; };
    
    QDate   x() const { return dateX; };
    void setX(QDate newX ){ dateX = newX; };
    
    int     y() const { return nY; };
    void setY( int newY ){ nY = newY; };
    
    QString information() const { return szInformation; };
    void setInformation(QString newInformation){ szInformation = newInformation; };
    
    
private:
    QString  szCaption;
    QDate    dateX;
    int      nY;
    
    QString  szInformation;

};


#endif
