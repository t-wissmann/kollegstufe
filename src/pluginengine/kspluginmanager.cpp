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
#include "kspluginmanager.h"
#include "ksplugin.h"
#include "kspluginitemwidget.h"
#include "kspluginengine.h"

#include <widgets/nokeyscrollarea.h>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QScrollArea>
#include <QPalette>

ksPluginManager::ksPluginManager(QWidget *parent)
 : QWidget(parent)
{
    m_pSelectedItem = NULL;
    m_pPluginEngine = NULL;
    createLayouts();
    connectSlots();
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    //setBoxLook(FALSE, FALSE);
}


ksPluginManager::~ksPluginManager()
{
}
void ksPluginManager::createLayouts()
{
    layoutPluginList = new QVBoxLayout;
    layoutPluginList->setSpacing(0);
    layoutPluginList->setMargin(0);
    
    scrollPlugins = new NoKeyScrollArea;
    scrollPlugins->setPluginManagerParent(this);
    scrollPlugins->setWidgetResizable(TRUE);
    scrollPlugins->setContentsMargins(0, 0, 0, 0);
    scrollPlugins->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollPlugins->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    m_wdgPluginListContainer = new QWidget;
    m_wdgPluginListContainer->setContentsMargins(0, 0, 0, 0);
    m_wdgPluginListContainer->setLayout(layoutPluginList);
    scrollPlugins->setWidget(m_wdgPluginListContainer);
    
    layoutParent = new QVBoxLayout;
    layoutParent->setMargin(0);
    //layoutParent->addWidget(m_wdgPluginListContainer);
    layoutParent->addWidget(scrollPlugins);
    setLayout(layoutParent);
}

void ksPluginManager::connectSlots()
{
    
}


void ksPluginManager::setPluginList(QList<ksPlugin*> newPluginList)
{
    m_pPluginList = newPluginList;
    resetPluginWidgetList();
}


void ksPluginManager::applyChanges()
{
    if(m_pPluginEngine != NULL)
    {
        m_pPluginEngine->applyPluginStateChanges();
    }
}

void ksPluginManager::setPluginEngine(ksPluginEngine* newPluginEngine)
{
    m_pPluginEngine = newPluginEngine;
    if(m_pPluginEngine == NULL)
    {
        setPluginList(QList<ksPlugin*>());
    }
    else
    {
        setPluginList(m_pPluginEngine->pluginList());
    }
}


void ksPluginManager::itemSelected(ksPluginItemWidget* item)
{
    if(m_pSelectedItem != NULL)
    {
        m_pSelectedItem->setSelected(FALSE);
    }
    if(item != NULL && !item->isSelected())
    {
        scrollPlugins->ensureWidgetVisible(item, 0, 0);
        item->setSelected();
    }
    m_pSelectedItem = item;
}

void ksPluginManager::keyPressEvent(QKeyEvent* event)
{
    if(event == NULL)
    {
        return;
    }
    int key = event->key();
    if(key == Qt::Key_Up || key == Qt::Key_Down)
    {
        int currentIndex = m_wdgPluginWidgets.indexOf(m_pSelectedItem);
        if(key == Qt::Key_Up)
        {
            currentIndex--;
        }
        else if (key == Qt::Key_Down)
        {
            currentIndex++;
        }
        if(currentIndex >= 0 && currentIndex < m_wdgPluginWidgets.size())
        {
            ksPluginItemWidget* newItem = m_wdgPluginWidgets[currentIndex];
            
            // unselect former selected item
            itemSelected(newItem);
        }
    }
}

void ksPluginManager::resetPluginWidgetList()
{
    ksPluginItemWidget* currentWidget = NULL;
    while(m_pPluginList.size() < m_wdgPluginWidgets.size())
    {// remove widgets if there are to much
        if(m_wdgPluginWidgets.size() < 1)
        {
            break;
        }
        currentWidget = m_wdgPluginWidgets.last();
        if(currentWidget == NULL)
        {
            m_wdgPluginWidgets.removeLast();
            continue;
        }
        layoutPluginList->removeWidget(currentWidget);
        disconnect(currentWidget, SIGNAL(selected(ksPluginItemWidget*)), this, SLOT(itemSelected(ksPluginItemWidget*)));
        delete currentWidget;
        m_wdgPluginWidgets.removeLast();
    }
    currentWidget = NULL;
    while(m_pPluginList.size() > m_wdgPluginWidgets.size())
    {// remove widgets if there are to much
        currentWidget = new ksPluginItemWidget;
        if(currentWidget == NULL)
        {
            continue;
        }
        m_wdgPluginWidgets.append(currentWidget);
        layoutPluginList->addWidget(currentWidget);
        connect(currentWidget, SIGNAL(selected(ksPluginItemWidget*)), this, SLOT(itemSelected(ksPluginItemWidget*)));
    }
    currentWidget = NULL;
    for(int i = 0; i < m_wdgPluginWidgets.size() && i < m_pPluginList.size(); i++)
    {
        currentWidget = m_wdgPluginWidgets[i];
        if(currentWidget == NULL)
        {
            continue;
        }
        currentWidget->setPlugin(m_pPluginList[i]);
        currentWidget->setSelected(FALSE);
    }
    m_pSelectedItem = NULL;
    
}


void ksPluginManager::setBoxLook(bool frame, bool textboxbackground)
{
    if(frame)
    {
        scrollPlugins->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken);
    }
    else
    {
        scrollPlugins->setFrameStyle( QFrame::NoFrame);
    }
    if(textboxbackground)
    {
        scrollPlugins->setBackgroundRole(QPalette::Base);
        scrollPlugins->setForegroundRole(QPalette::Text);
    }else
    {
        scrollPlugins->setBackgroundRole(QPalette::Window);
        scrollPlugins->setForegroundRole(QPalette::WindowText);
    }
}





