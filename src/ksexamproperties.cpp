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
#include "ksexamproperties.h"
#include "ksplattformspec.h"
#include "dateConverter.h"

#include "xmlparser.h"

// widgets:
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QGroupBox>

// layouts
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

// other qt -objects
#include <QSizePolicy>
#include <QDate>


ksExamProperties::ksExamProperties(QWidget *parent)
 : QDialog(parent)
{
    initMembers();
    allocateWidgets();
    createLayouts();
    connectSlots();
    initWidgets();
}


ksExamProperties::~ksExamProperties()
{
}

void ksExamProperties::initMembers()
{
    
    setModal(TRUE);
    examToEdit = NULL;
}

void ksExamProperties::allocateWidgets()
{
    // bottom buttons
    btnOk     = new QPushButton(tr("Ok"));
    btnCancel = new QPushButton(tr("Abbrechen"));
    btnOk->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnCancel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    
    // left column
    lblNumber   = new QLabel("Nummer:");
    spinNumber  = new QSpinBox;
    lblType     = new QLabel("Typ:");
    txtType     = new QLineEdit;
    lblPoints   = new QLabel("Punkte:");
    spinPoints  = new QSpinBox;
    lblDate     = new QLabel("Datum:");
    dteDate     = new QDateEdit;
    lblWeighting        = new QLabel("Gewichtung:");
    optWeightingWritten = new QRadioButton("schriftlich");
    optWeightingOral    = new QRadioButton(QString::fromLocal8Bit("mündlich"));
    
    //semester Selection
    grpSemester     = new QGroupBox("Semester:");
    optSemester121  = new QRadioButton("12 / 1");
    optSemester122  = new QRadioButton("12 / 2");
    optSemester131  = new QRadioButton("13 / 1");
    optSemester132  = new QRadioButton("13 / 2");
    optSemesterAuto  = new QRadioButton("Automatische Zuordnung");
}

void ksExamProperties::createLayouts()
{
    layoutBottom = new QHBoxLayout;
    layoutBottom->setMargin(0);
    layoutBottom->addStretch(1);
    layoutBottom->addWidget(btnOk);
    layoutBottom->addWidget(btnCancel);
    
    layoutSemester = new QGridLayout;
    layoutSemester->addWidget(optSemester121, 0, 0);
    layoutSemester->addWidget(optSemester122, 0, 1);
    layoutSemester->addWidget(optSemester131, 1, 0);
    layoutSemester->addWidget(optSemester132, 1, 1);
    layoutSemester->addWidget(optSemesterAuto,2, 0, 1, 2);
    grpSemester->setLayout(layoutSemester);
    
    layoutWeighting = new QHBoxLayout;
    layoutWeighting->addWidget(lblWeighting);
    layoutWeighting->addWidget(optWeightingOral);
    layoutWeighting->addWidget(optWeightingWritten);
    
    layoutParent = new QGridLayout;
    layoutParent->addWidget(lblNumber, 0, 0);
    layoutParent->addWidget(spinNumber, 0, 1);
    layoutParent->addWidget(lblType, 1, 0);
    layoutParent->addWidget(txtType, 1, 1);
    layoutParent->addWidget(lblPoints, 2, 0);
    layoutParent->addWidget(spinPoints, 2, 1);
    layoutParent->addWidget(lblDate, 3, 0);
    layoutParent->addWidget(dteDate, 3, 1);
    layoutParent->addLayout(layoutWeighting, 4, 0, 1, 2);
    
    layoutParent->addWidget(grpSemester, 0, 2, 5, 1);
    
    
    layoutParent->addLayout(layoutBottom, 5, 0, 1, 3);
    
    setLayout(layoutParent);
}

void ksExamProperties::connectSlots()
{
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(btnOk,     SIGNAL(clicked()), this, SLOT(accept()));
    connect(this,      SIGNAL(accepted()), this, SLOT(writeWidgetAttributesToExam()));
}

void ksExamProperties::initWidgets()
{
    spinPoints->setMinimum(0);
    spinPoints->setMaximum(15);
    
    spinNumber->setMinimum(0);
    spinNumber->setMaximum(999);
    spinNumber->setSpecialValueText("keine Nummer");
}

void ksExamProperties::setExamToEdit(xmlObject* newExamToEdit)
{
    examToEdit = newExamToEdit;
    if (!newExamToEdit)
    {
        QString errormsg = "Interner Pointer-Fehler: newExamToEdit = NULL !";
        setWindowTitle(errormsg);
        txtType->setText(errormsg);
        return;
    }
    ksPlattformSpec::addMissingExamAttributes(examToEdit);
    // write class attributes to widgets:
    spinNumber->setValue(examToEdit->cGetObjectByAttributeValue("name", "number")->
                                        cGetAttributeByName("value")->nValueToInt());
    
    txtType->setText(examToEdit->cGetObjectByAttributeValue("name", "type")->
                                        cGetAttributeByName("value")->value());
    
    spinPoints->setValue(examToEdit->cGetObjectByAttributeValue("name", "points")->
                                        cGetAttributeByName("value")->nValueToInt());
    
    cDateConverter dateFile;
    dateFile.setDateString(examToEdit->cGetObjectByAttributeValue("name", "date")->
                                    cGetAttributeByName("value")->value());
    
    QDate dateWidget;
    dateWidget.setYMD(dateFile.Year(), dateFile.Month(), dateFile.Day());
    dteDate->setDate( dateWidget);
    QString semester = examToEdit->cGetObjectByAttributeValue("name", "semester")->
            cGetAttributeByName("value")->value();
    if(semester == "auto")
        optSemesterAuto->setChecked(TRUE);
    else if(semester == "12/1")
        optSemester121->setChecked(TRUE);
    else if(semester == "12/2")
        optSemester122->setChecked(TRUE);
    else if(semester == "13/1")
        optSemester131->setChecked(TRUE);
    else if(semester == "13/2")
        optSemester132->setChecked(TRUE);
    else
        optSemesterAuto->setChecked(TRUE);
    
    QString weighting = examToEdit->cGetObjectByAttributeValue("name", "weighting")->
            cGetAttributeByName("value")->value();
    if(weighting == "written")
        optWeightingWritten->setChecked(TRUE);
    else
        optWeightingOral->setChecked(TRUE);
    
    
}


void ksExamProperties::writeWidgetAttributesToExam()
{
    if (!examToEdit)
    {
        QString errormsg = "Interner Pointer-Fehler: newExamToEdit = NULL !";
        setWindowTitle(errormsg);
        txtType->setText(errormsg);
        return;
    }
    // we don't need to exec ksPlattformSpec::addMissingExamAttributes(examToEdit);
    // because examToEdit hasn't been changed , because this dialog is modal
    
    examToEdit->cGetObjectByAttributeValue("name", "number")->
            cGetAttributeByName("value")->SetValueToInt(spinNumber->value());
    
    
    examToEdit->cGetObjectByAttributeValue("name", "type")->
            cGetAttributeByName("value")->SetValue(txtType->text().toAscii().data());
    
    examToEdit->cGetObjectByAttributeValue("name", "points")->
            cGetAttributeByName("value")->SetValueToInt(spinPoints->value());
    
    
    // DATE
    cDateConverter dateFile;
    dateFile.setYear(dteDate->date().year());
    dateFile.setMonth(dteDate->date().month());
    dateFile.setDay(dteDate->date().day());
    examToEdit->cGetObjectByAttributeValue("name", "date")->
            cGetAttributeByName("value")->SetValue(dateFile.getDateString());
    
    // SEMESTER
    QString semester = "auto";
    if(optSemesterAuto->isChecked())
        semester = "auto";
    else if(optSemester121->isChecked())
        semester = "12/1";
    else if(optSemester122->isChecked())
        semester = "12/2";
    else if(optSemester131->isChecked())
        semester = "13/1";
    else if(optSemester132->isChecked())
        semester = "13/2";
    examToEdit->cGetObjectByAttributeValue("name", "semester")->
            cGetAttributeByName("value")->SetValue(semester.toAscii().data());
    
    // WEIGHTING
    QString weighting = "oral";
    if(optWeightingWritten->isChecked())
    {
        weighting = "written";
    }
    else
    {
        weighting = "oral";
    }
    
    examToEdit->cGetObjectByAttributeValue("name", "weighting")->
            cGetAttributeByName("value")->SetValue(weighting.toAscii().data());
    
    
}

