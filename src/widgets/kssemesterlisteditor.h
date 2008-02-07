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
#ifndef KSSEMESTERLISTEDITOR_H
#define KSSEMESTERLISTEDITOR_H

#include <QWidget>
#include <QList>
class QVBoxLayout;
class QHBoxLayout;
class xmlObject;

class QPushButton;
class QLabel;
class QScrollArea;

class ksSemesterItemWidget;

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class ksSemesterListEditor : public QWidget
{
Q_OBJECT
public:
    ksSemesterListEditor(QWidget *parent = 0);
    ~ksSemesterListEditor();
    
    void retranslateUi();
    void reloadIcons();
    
    void setSemesterList(xmlObject* semesterlist);
    ksSemesterItemWidget* selectedSemester();
    int selectedSemesterIndex();
    
    void applyChanges();
    
public slots:
    void addSemester();
    void deleteSemester();
    
protected slots:
    void selectionChanged(ksSemesterItemWidget* selectedItem);
private:
    void allocateWidgets();
    void createLayouts();
    void connectSlots();
    
    // tool buttons
    QPushButton* btnAdd;
    QPushButton* btnDelete;
    QPushButton* btnMoveUp;
    QPushButton* btnMoveDown;
    
    QScrollArea* scrollSemesterList;
    QWidget*     wdgSemesterListContainter;
    
    QHBoxLayout* layoutParent;
    QVBoxLayout* layoutToolButtons;
    QVBoxLayout* layoutSemesterList;
    
    QList<ksSemesterItemWidget*> lstSemesterItems;
    
    xmlObject*  m_pSourceList;
    ksSemesterItemWidget*  m_pSelectedItem;
    
};

#endif
