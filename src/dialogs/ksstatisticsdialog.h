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
#ifndef KSSTATISTICSDIALOG_H
#define KSSTATISTICSDIALOG_H

#include <QDialog>

class xmlObject;
class ksStatisticsWidget;
class ksSubjectInformationWidget;
class ksFilterWidget;

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QPushButton;
class QComboBox;
class QLabel;

class QPushButton;
class QEvent;
#include <QStringList>

/**
	@author Thorsten Wissmann <towi89@web.de>
*/
class ksStatisticsDialog : public QDialog
{
Q_OBJECT
public:
    ksStatisticsDialog(QWidget* parent = 0);

    ~ksStatisticsDialog();
    
    void setSubject(xmlObject* newSubject);
    void setProperties(xmlObject* newProperties);
    
    void retranslateUi();
    void reloadIcons();
    void setSelectedExam(xmlObject* exam);
    
public slots:
    void refreshUiAndChildren();
    void refreshExamListFromXmlSubject();
    void currentSemesterChanged(int newSelection);
    
    void increaseSelection();
    void decreaseSelection();
    
protected:
    virtual void changeEvent(QEvent* event);
private:
    void initMembers();
    void allocateWidgets();
    void createLayouts();
    void connectSlots();
    void initWidgets();
    
    //widgets
    ksStatisticsWidget* statistics;
    ksSubjectInformationWidget* information;
    ksFilterWidget*     wdgFilter;
    QPushButton*        btnClose;
    QComboBox*          cmbSemesterSelection;
    QLabel*             lblSemesterSelection;
    
    QPushButton*        btnIncreaseSelection;
    QPushButton*        btnDecreaseSelection;
    
    //layouts
    QVBoxLayout*        layoutParent;
    QHBoxLayout*        layoutChartAndInformation;
    QVBoxLayout*        layoutInformation;
    QHBoxLayout*        layoutSemesterSelection;
    QHBoxLayout*        layoutBottom;
    
    //members
    QStringList semesterList; 
    xmlObject*  subject;
    xmlObject*  properties;

};

#endif
