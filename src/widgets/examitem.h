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
#ifndef EXAMITEM_H
#define EXAMITEM_H

#include <QTreeWidgetItem>
#include <QString>
class QTreeWidget;
/**
	@author Thorsten Wissmann <towi89@web.de>
*/
class ExamItem : public QTreeWidgetItem
{
public:
    ExamItem(QTreeWidget* parent):QTreeWidgetItem(parent) {};
    static bool     hasMatchOn( QTreeWidgetItem* item, QString keyword);
    bool     hasMatchOn(QString keyword);
    
    
    bool     operator< ( const QTreeWidgetItem & other ) const;
    bool     thisIsEarlierDate( const QTreeWidgetItem & other ) const;

};

#endif
