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
#include "examitem.h"
#include <QString>
#include <QHeaderView>
#include <QTreeWidget>
#include <QObject>

#include "dateConverter.h"

bool ExamItem::operator< ( const QTreeWidgetItem & other ) const
{
    if (!treeWidget())
        return FALSE;
    if (!treeWidget()->headerItem())
        return FALSE;
    
    int sortCol = treeWidget()->sortColumn();
    
    if(treeWidget()->headerItem()->text(sortCol) == QObject::tr("Date"))
    {
        return thisIsEarlierDate(other);
    }
    if(treeWidget()->headerItem()->text(sortCol) == QObject::tr("Points"))
    {
        int ownValue = this->text(sortCol).toInt();
        int otherValue = other.text(sortCol).toInt();
        
        return (ownValue < otherValue);
    }
    return ( 0 < QString::compare(this->text(sortCol), other.text(sortCol), Qt::CaseInsensitive));
}


bool ExamItem::thisIsEarlierDate( const QTreeWidgetItem & other ) const
{
    int col = treeWidget()->sortColumn();
    cDateConverter dateOwn;
    cDateConverter dateToCompare;
    dateOwn.setHumanDate(this->text(col).toAscii().data());
    dateToCompare.setHumanDate(other.text(col).toAscii().data());
    return (-1 == cDateConverter::compareDates( dateOwn, dateToCompare )); // compare dates returns -1 
                                                                           //if dateOwn was earlier than dateToCompare
}


