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
#ifndef NOKEYSCROLLAREA_H
#define NOKEYSCROLLAREA_H

#include <QScrollArea>

#include <pluginengine/kspluginmanager.h>
/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class NoKeyScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    NoKeyScrollArea(QWidget* parent = 0) : QScrollArea(parent)
    {
        m_pPluginManagerParent = NULL;
    }
    void setPluginManagerParent(ksPluginManager* newPluginManagerParent)
    {
        m_pPluginManagerParent = newPluginManagerParent;
    };

protected:
    virtual void keyPressEvent(QKeyEvent* event)
    {
        if(m_pPluginManagerParent)
        {
            m_pPluginManagerParent->pressKey(event);
        }
        else
        {
            QScrollArea::keyPressEvent(event);
        }
    };
private:
    ksPluginManager* m_pPluginManagerParent;

};

#endif
