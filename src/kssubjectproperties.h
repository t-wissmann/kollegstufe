/***************************************************************************
 *   Copyright (C) 2007 by Thorsten W.	                                   *
 *   towi89@web.de towi16.piranho.de                                       *
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
#ifndef KSSUBJECTPROPERTIES_H
#define KSSUBJECTPROPERTIES_H


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

class xmlObject;

#include <QDialog>

/**
	@author Thorsten Wissmann <towi89@web.de>
*/
class ksSubjectProperties : public QDialog
{
Q_OBJECT
public:
    ksSubjectProperties(QWidget *parent = 0);

    ~ksSubjectProperties();
    void setSubjectToEdit(xmlObject* newSubjectToEdit);
    void setCathegoryOfSubject(xmlObject* newCathegory); // needed to avoid multible same Names
public slots:
    void writeWidgetAttributesToSubject();
    void checkForMultibleNames();
private:
    void allocateWidgets();
    void createLayouts();
    void connectSlots();
    void initWidgets();
    
    // bottom widgets
    QPushButton*    btnOk;
    QPushButton*    btnCancel;
    // input widgets
    QLabel*         lblName;
    QLineEdit*      txtName;
    QLabel*         lblTeacher;
    QLineEdit*      txtTeacher;
    
    // layouts
    QHBoxLayout*    layoutBottom;
    QGridLayout*    layoutParent;

    // members
    xmlObject*      subjectToEdit;
    xmlObject*      currentCathegory;
};

#endif
