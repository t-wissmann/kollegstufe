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
#include "../widgets/kssemesterlisteditor.h"
#include "kssemesteritemwidget.h"
#include <io/xmlparser.h>
#include <io/ksdebugoutput.h>
#include <io/ksiconcatcher.h>

#include <QLabel>
#include <QPushButton>
#include <QSizePolicy>

#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>

ksSemesterListEditor::ksSemesterListEditor(QWidget *parent)
 : QWidget(parent)
{
    m_pSelectedItem = NULL;
    m_pSourceList = NULL;
    allocateWidgets();
    createLayouts();
    connectSlots();
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    retranslateUi();
    reloadIcons();
}


ksSemesterListEditor::~ksSemesterListEditor()
{
}

void ksSemesterListEditor::allocateWidgets()
{
    btnAdd    = new QPushButton;
    btnDelete = new QPushButton;
    btnMoveUp = new QPushButton;
    btnMoveDown = new QPushButton;
    
    // for the first time, we don't need to (and can't) use move up and down buttons
    btnMoveUp->setVisible(FALSE);
    btnMoveDown->setVisible(FALSE);
    
    scrollSemesterList = new QScrollArea;
    wdgSemesterListContainter = new QWidget;
    
    
}

void ksSemesterListEditor::createLayouts()
{
    layoutToolButtons = new QVBoxLayout;
    layoutToolButtons->setMargin(0);
    layoutToolButtons->addWidget(btnAdd);
    layoutToolButtons->addWidget(btnDelete);
    layoutToolButtons->addWidget(btnMoveUp);
    layoutToolButtons->addWidget(btnMoveDown);
    
    layoutSemesterList = new QVBoxLayout;
    layoutSemesterList->setSpacing(0);
    layoutSemesterList->setMargin(0);
    
    wdgSemesterListContainter->setLayout(layoutSemesterList);
    scrollSemesterList->setWidget(wdgSemesterListContainter);
    scrollSemesterList->setWidgetResizable(TRUE);
    scrollSemesterList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    layoutParent = new QHBoxLayout;
    layoutParent->setMargin(0);
    layoutParent->addWidget(scrollSemesterList);
    layoutParent->addLayout(layoutToolButtons);
    
    setLayout(layoutParent);
}

void ksSemesterListEditor::connectSlots()
{
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(addSemester()));
    connect(btnDelete, SIGNAL(clicked()), this, SLOT(deleteSemester()));
}


void ksSemesterListEditor::retranslateUi()
{
    btnAdd->setText(tr("Add"));
    btnDelete->setText(tr("Delete"));
    
    btnMoveUp->setText(tr("Move Up"));
    btnMoveDown->setText(tr("Move Down"));
}


void ksSemesterListEditor::reloadIcons()
{
    btnAdd->setIcon(ksIconCatcher::getIcon("add", 16));
    btnDelete->setIcon(ksIconCatcher::getIcon("remove", 16));
    btnMoveUp->setIcon(ksIconCatcher::getIcon("up", 16));
    btnMoveDown->setIcon(ksIconCatcher::getIcon("down", 16));
}

void ksSemesterListEditor::setSemesterList(xmlObject* semesterlist)
{
    m_pSourceList = semesterlist;
    
    int semesterCount = m_pSourceList ? m_pSourceList->nGetObjectCounter() : 0;
    ksSemesterItemWidget* currentItem = NULL;
    
    while(semesterCount < lstSemesterItems.size())
    {// delete items if there are to much
        currentItem = lstSemesterItems.last();
        if(!currentItem)
        {
            lstSemesterItems.removeLast();
            continue;
        }
        disconnect(currentItem, SIGNAL(getsSelected(ksSemesterItemWidget*)), this, SLOT(selectionChanged(ksSemesterItemWidget*)));
        layoutSemesterList->removeWidget(currentItem);
        lstSemesterItems.removeLast();
        delete currentItem;
    }
    
    while(semesterCount > lstSemesterItems.size())
    {// add items if there are to few
        currentItem = new ksSemesterItemWidget;
        if(!currentItem)
        {
            break;
        }
        connect(currentItem, SIGNAL(getsSelected(ksSemesterItemWidget*)), this, SLOT(selectionChanged(ksSemesterItemWidget*)));
        lstSemesterItems.append(currentItem);
        layoutSemesterList->addWidget(currentItem);
    }
    
    
    //copy semesters from xml to widgets
    xmlObject* currentXmlSemester = NULL;
    for(int i = 0; i < semesterCount; i++)
    {
        currentXmlSemester = m_pSourceList->cGetObjectByIdentifier(i);
        if(!currentXmlSemester)
        {
            continue;
        }
        lstSemesterItems[i]->setSemester(currentXmlSemester);
        lstSemesterItems[i]->setSelected(FALSE);
    }
    
    // clear selection
    m_pSelectedItem = NULL;
    
    
    
}


void ksSemesterListEditor::addSemester()
{
    lstSemesterItems.append(new ksSemesterItemWidget);
    layoutSemesterList->addWidget(lstSemesterItems.last());
    connect(lstSemesterItems.last(), SIGNAL(getsSelected(ksSemesterItemWidget*)), this, SLOT(selectionChanged(ksSemesterItemWidget*)));
}

ksSemesterItemWidget* ksSemesterListEditor::selectedSemester()
{
    return m_pSelectedItem;
}

int ksSemesterListEditor::selectedSemesterIndex()
{
    int widgetIndex = -1;
    for(int i = 0; i < lstSemesterItems.size(); i++)
    {
        if(lstSemesterItems[i] && lstSemesterItems[i]->isSelected())
        {
            widgetIndex = i;
            break;
        }
    }
    return widgetIndex;
}


void ksSemesterListEditor::selectionChanged(ksSemesterItemWidget* selectedItem)
{
    if(m_pSelectedItem == selectedItem)
    {
        return;
    }
    if(m_pSelectedItem)
    {
        m_pSelectedItem->setSelected(FALSE);
    }
    m_pSelectedItem = selectedItem;
}


void ksSemesterListEditor::deleteSemester()
{
    ksSemesterItemWidget* semester = m_pSelectedItem;
    if(!semester)
    {
        ksDebugOutput::print("ksSemesterListEditor::deleteSemester(): no semester selected");
        return;
    }
    semester->setAboutToDelete(TRUE);
    semester->setSelected(FALSE);
    m_pSelectedItem = NULL; // clear selection
}

void ksSemesterListEditor::applyChanges()
{
    if(!m_pSourceList)
    {
        return;
    }
    ksSemesterItemWidget* currentWdg = NULL;
    for(int i = 0; i < lstSemesterItems.size(); i++)
    {
        currentWdg = lstSemesterItems[i];
        if(!currentWdg)
        {
            continue;
        }
        if(currentWdg->semester() == NULL) // if this semester is NEW
        {
            currentWdg->writeDataTo(m_pSourceList->cAddObject("semester"));
        }
        else if(currentWdg->isAboutToDelete()) // if this semester should be deleted
        {
            m_pSourceList->nDeleteObject(currentWdg->semester());
        }
        else
        {
            // write currentWdg's date to it's semester
            currentWdg->applyChanges();
        }
    }
}

