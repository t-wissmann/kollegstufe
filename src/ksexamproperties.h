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
#ifndef KSEXAMPROPERTIES_H
#define KSEXAMPROPERTIES_H

#include <QDialog>


// layouts
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;

// widgets
class QPushButton;
class QLabel;
class QLineEdit;
class QSpinBox;
class QRadioButton;
class QDateEdit;
class QGroupBox;

class QEvent;
class xmlObject;
/**
	@author Thorsten Wissmann <towi89@web.de>
*/
class ksExamProperties : public QDialog
{
Q_OBJECT

public:
    ksExamProperties(QWidget *parent = 0);
    ~ksExamProperties();
    void retranslateUi();
public slots:
    void setProperties(xmlObject*     newProperties);
    void setExamToEdit(xmlObject* newExamToEdit);
    void writeWidgetAttributesToExam();
protected:
    virtual void changeEvent(QEvent* event);
private:
    
    void    initMembers();
    void    allocateWidgets();
    void    createLayouts();
    void    connectSlots();
    void    initWidgets();
    
    // widgets
    // accept / reject buttons
    QPushButton*    btnOk;
    QPushButton*    btnCancel;

    // left column
    QLabel*         lblNumber;
    QSpinBox*       spinNumber;
    
    QLabel*         lblType;
    QLineEdit*      txtType;
    
    QLabel*         lblPoints;
    QSpinBox*       spinPoints;
    
    QLabel*         lblDate;
    QDateEdit*      dteDate;
    
    QLabel*         lblWeighting;
    QRadioButton*   optWeightingOral;
    QRadioButton*   optWeightingWritten;
    QHBoxLayout*    layoutWeighting;
    
    QGroupBox*      grpSemester;
    QRadioButton*   optSemester121;
    QRadioButton*   optSemester122;
    QRadioButton*   optSemester131;
    QRadioButton*   optSemester132;
    QRadioButton*   optSemesterAuto;
    
    //layouts
    QGridLayout*    layoutSemester;
    QHBoxLayout*    layoutBottom;
    QGridLayout*    layoutParent;
    
    // members
    xmlObject*  examToEdit;
    xmlObject*  properties;
    
    
};

#endif
