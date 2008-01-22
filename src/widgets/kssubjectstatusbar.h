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
#ifndef KSSUBJECTSTATUSBAR_H
#define KSSUBJECTSTATUSBAR_H

#include <QWidget>

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/

//widgets
class QPushButton;
class QLabel;

//layouts
class QHBoxLayout;
// classes
class QEvent;
class xmlObject;


class ksSubjectStatusbar : public QWidget
{
Q_OBJECT
signals:
    void statisticsButtonToggled(bool pressed);
public:
    ksSubjectStatusbar(QWidget *parent = 0);

    ~ksSubjectStatusbar();
    //getter and setter functions
    void setAlignment(Qt::Alignment nNewAlignment); // alignment is only horizontal
    Qt::Alignment alignment();
    
    void setSubject(xmlObject* newSubject);
    xmlObject* subject();
    
    void calculateAverage();
    void setEntireAverageTitle(double average);
    
public slots:
    void retranslateUi();
    void toggleStatisticsButton(bool pressed);
private slots:
    void btnStatisticsToggled(bool pressed){ emit statisticsButtonToggled(pressed); };
protected:
    virtual void changeEvent(QEvent* event);
    
protected:
    // init - functions
    void resetLayout();
private:
    void initMembers();
    void allocateWidgets();
    void initWidgets();
    void createLayouts();
    void connectSlots();
    
    
    // widgets
    QPushButton*    btnStatistics;

    
    // layouts
    QHBoxLayout*    layoutParent;
    
    
    
    // member
    Qt::Alignment m_nAlignment;
    xmlObject*    m_cSubjectToShow;
    
};

#endif
