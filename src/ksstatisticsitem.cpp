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
#include "ksstatisticsitem.h"

ksStatisticsItem::ksStatisticsItem()
{
    nY = 0;
    pSourceItem = NULL;
}

ksStatisticsItem::ksStatisticsItem(const ksStatisticsItem &other)
{
    nY = 0;
    setCaption ( other.caption() );
    setX ( other.x() );

    setY ( other.y() );
    setInformation ( other.information() );
    setSourceItem(other.sourceItem());
}

ksStatisticsItem::ksStatisticsItem ( QString newCaption, QDate newX, int newY, QString newInformation )
{
    pSourceItem = NULL;
    szCaption = newCaption;
    dateX = newX;
    nY = newY;
    szInformation = newInformation;
}


ksStatisticsItem::~ksStatisticsItem()
{
}

void ksStatisticsItem::operator= (const ksStatisticsItem& newItem )
{
    setCaption ( newItem.caption() );
    setX ( newItem.x() );
    setY ( newItem.y() );
    setInformation ( newItem.information() );
    setSourceItem(newItem.sourceItem());
}


bool ksStatisticsItem::operator<(const ksStatisticsItem& other) const
{
    return x() < other.x();
}

bool ksStatisticsItem::operator>(const ksStatisticsItem& other) const
{
    return x() > other.x();
}

bool ksStatisticsItem::operator<=(const ksStatisticsItem& other) const
{
    return x() <= other.x();
}

bool ksStatisticsItem::operator>=(const ksStatisticsItem& other) const
{
    return x() >= other.x();
}


bool ksStatisticsItem::operator==(const ksStatisticsItem& other) const
{
    return x() == other.x();
}

bool ksStatisticsItem::operator!=(const ksStatisticsItem& other) const
{
    return x() != other.x();
}




