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

#include <QMenu>
#include <QAction>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPalette>
#include <QStringList>

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
    
    btnTemplates = new QPushButton;
    mnuTemplates = new QMenu;
    mnaEmptyList = mnuTemplates->addAction("empty");
    mnaBavarianKollegstufe = mnuTemplates->addAction("bavarian ks");
    btnTemplates->setMenu(mnuTemplates);
    
    scrollSemesterList = new QScrollArea;
    
    scrollSemesterList->setBackgroundRole(QPalette::NoRole);
    QPalette pal = scrollSemesterList->palette();
    pal.setBrush(scrollSemesterList->backgroundRole(), QBrush(QColor(255, 255, 255, 0)));
    scrollSemesterList->setPalette(pal);
    //scrollSemesterList->setAutoFillBackground(TRUE);
    scrollSemesterList->setFrameStyle(QFrame::NoFrame);
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
    layoutToolButtons->addWidget(btnTemplates);
    
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
    connect(btnMoveUp, SIGNAL(clicked()), this, SLOT(moveUp()));
    connect(btnMoveDown, SIGNAL(clicked()), this, SLOT(moveDown()));
    connect(mnaBavarianKollegstufe, SIGNAL(triggered()), this, SLOT(createBavarianKollegstufeTemplate()));
    connect(mnaEmptyList, SIGNAL(triggered()), this, SLOT(clearSemesterList()));
}


void ksSemesterListEditor::retranslateUi()
{
    btnAdd->setText(tr("Add"));
    btnDelete->setText(tr("Delete"));
    
    btnMoveUp->setText(tr("Move Up"));
    btnMoveDown->setText(tr("Move Down"));
    btnTemplates->setText(tr("Templates"));
    
    mnaEmptyList->setText(tr("Empty List"));
    mnaBavarianKollegstufe->setText(tr("Bavarian Kollegstufe"));
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

void ksSemesterListEditor::moveUp()
{
    int currentIndex = selectedSemesterIndex();
    if(currentIndex <= 0)
    {//if currentIndex is 0, we can't move it up
        return;
    }
    layoutSemesterList->removeWidget(lstSemesterItems[currentIndex]);
    layoutSemesterList->insertWidget(currentIndex-1, lstSemesterItems[currentIndex]);
    lstSemesterItems.move(currentIndex, currentIndex-1);
    scrollSemesterList->ensureWidgetVisible(lstSemesterItems[currentIndex-1]);
}

void ksSemesterListEditor::moveDown()
{
    int currentIndex = selectedSemesterIndex();
    if(currentIndex < 0 || currentIndex == lstSemesterItems.count()-1)
    {// if currentIndex is the last in the list, we can't move it down
        return;
    }
    layoutSemesterList->removeWidget(lstSemesterItems[currentIndex]);
    layoutSemesterList->insertWidget(currentIndex+1, lstSemesterItems[currentIndex]);
    lstSemesterItems.move(currentIndex, currentIndex+1);
    scrollSemesterList->ensureWidgetVisible(lstSemesterItems[currentIndex+1]);
    
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
            xmlObject* newSemester = m_pSourceList->cAddObject("semester");
            currentWdg->writeDataTo(newSemester);
            currentWdg->setSemester(newSemester);
        }
        else 
        {
            // write currentWdg's date to it's semester
            currentWdg->applyChanges();
        }
    }
    // move semester to their new positions
    for(int i = 0; i < lstSemesterItems.size(); i++)
    {
        currentWdg = lstSemesterItems[i];
        if(!currentWdg)
        {
            continue;
        }
        m_pSourceList->moveObjectTo(currentWdg->semester(), i);
    }
    // delete semesters, that should be deleted
    for(int i = 0; i < lstSemesterItems.size(); i++)
    {
        currentWdg = lstSemesterItems[i];
        if(!currentWdg)
        {
            continue;
        }
        if(currentWdg->isAboutToDelete()) // if this semester should be deleted
        {
            m_pSourceList->nDeleteObject(currentWdg->semester());
        }
    }
}

void ksSemesterListEditor::createBavarianKollegstufeTemplate()
{
    // at first: create a list of 4 semesters
    // so: resize list, if there are to few items
    ksSemesterItemWidget* currentWdg;
    while(lstSemesterItems.size() < 4)
    {
        // add missing semesters
        addSemester();
    }
    // unhide the first 4 items
    for(int i = 0; i < 4; i++)
    {
        if(lstSemesterItems[i])
        {
            lstSemesterItems[i]->setAboutToDelete(FALSE);
        }
    }
    // and delete all other items
    for(int i = 4; i < lstSemesterItems.size(); i++)
    {
        if(lstSemesterItems[i])
        {
            lstSemesterItems[i]->setAboutToDelete(TRUE);
        }
    }
    QDate date = QDate::currentDate();
    // get last september
    while(date.month() != 9)
    {
        date = date.addMonths(-1);
    }
    while(date.dayOfWeek() != Qt::Tuesday)
    {
        date = date.addDays(-1);
    }
    
    currentWdg = lstSemesterItems[0];
    currentWdg->setName("12/1");
    currentWdg->setStartDate(date);
    date = date.addMonths(5); // go to february
    currentWdg->setEndDate(date);
    
    date = date.addDays(1);
    currentWdg = lstSemesterItems[1];
    currentWdg->setName("12/2");
    currentWdg->setStartDate(date);
    date = date.addMonths(6); // go to August
    currentWdg->setEndDate(date);
    
    date = date.addDays(1);
    currentWdg = lstSemesterItems[2];
    currentWdg->setName("13/1");
    currentWdg->setStartDate(date);
    while(date.month() != 2)
    {
        date = date.addMonths(1); // go to February
    }
    currentWdg->setEndDate(date);
    
    
    date = date.addDays(1);
    currentWdg = lstSemesterItems[3];
    currentWdg->setName("13/2");
    currentWdg->setStartDate(date);
    date = date.addMonths(6); // go to August
    currentWdg->setEndDate(date);
    
}

void ksSemesterListEditor::clearSemesterList()
{
    for(int i = 0; i < lstSemesterItems.size(); i++)
    {
        if(lstSemesterItems[i])
        {
            lstSemesterItems[i]->setAboutToDelete(TRUE);
        }
    }
}


