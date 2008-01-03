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
#ifndef KSPLUGINMANAGER_H
#define KSPLUGINMANAGER_H

#include <QWidget>
#include <QList>
#include <QScrollArea>
class ksPlugin;
class ksPluginItemWidget;
class ksPluginEngine;

class QKeyEvent;
class QVBoxLayout;
class NoKeyScrollArea;


/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class ksPluginManager : public QWidget
{
Q_OBJECT
public:
    ksPluginManager(QWidget *parent = 0);

    ~ksPluginManager();
    
    void setPluginList(QList<ksPlugin*> newPluginList);
    void setPluginEngine(ksPluginEngine* newPluginEngine);
    
    void pressKey(QKeyEvent* event){ keyPressEvent(event); };
    void setBoxLook(bool frame, bool textboxbackground);
    
    void applyChanges();
    
protected:
    virtual void keyPressEvent(QKeyEvent* event);
public slots:
    void itemSelected(ksPluginItemWidget* item);
private:
    
    void createLayouts();
    void connectSlots();
    void resetPluginWidgetList();
    
    
    ksPluginItemWidget* m_pSelectedItem;
    // layouts
    QVBoxLayout* layoutParent;
    QVBoxLayout* layoutPluginList;
    NoKeyScrollArea* scrollPlugins;
    
    
    //widgets
    QWidget*                    m_wdgPluginListContainer;
    QList<ksPluginItemWidget*>  m_wdgPluginWidgets;
    
    // members
    QList<ksPlugin*>   m_pPluginList;
    ksPluginEngine*    m_pPluginEngine;

};

#endif
