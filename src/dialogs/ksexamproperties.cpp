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
#include "ksexamproperties.h"
#include <core/ksplattformspec.h>
#include <core/dateConverter.h>
#include <io/ksiconcatcher.h>


#include <io/xmlparser.h>

// widgets:
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QGroupBox>

// layouts
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

// other qt -objects
#include <QSizePolicy>
#include <QDate>
#include <QEvent>
#include <QMessageBox>

ksExamProperties::ksExamProperties(QWidget *parent)
 : QDialog(parent)
{
    initMembers();
    allocateWidgets();
    createLayouts();
    connectSlots();
    initWidgets();
    
    if(parent)
    {
        setWindowIcon(parent->windowIcon());
    }
    retranslateUi();
    reloadIcons();
}


ksExamProperties::~ksExamProperties()
{
}

void ksExamProperties::initMembers()
{
    
    setModal(TRUE);
    properties = NULL;
    examToEdit = NULL;
}


void ksExamProperties::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
}



void ksExamProperties::retranslateUi()
{
    btnOk->setText(tr("Ok"));
    btnCancel->setText(tr("Cancel"));
    lblNumber->setText(tr("Number:"));
    lblType->setText(tr("Type:"));
    lblPoints->setText(tr("Points:"));
    lblDate->setText(tr("Date:"));
    lblWeighting->setText(tr("Weighting:"));
    optWeightingWritten->setText(tr("written"));
    optWeightingOral->setText(tr("oral"));
    grpSemester->setTitle(tr("Semester:"));
    optSemesterAuto->setText(tr("Automatische Zuordnung"));
    spinNumber->setSpecialValueText(tr("no number"));
    resetWindowTitle();
    
}

void ksExamProperties::reloadIcons()
{
    btnOk->setIcon(ksIconCatcher::getIcon("button_ok", 16));
    btnCancel->setIcon(ksIconCatcher::getIcon("button_cancel", 16));
}

void ksExamProperties::allocateWidgets()
{
    // bottom buttons
    btnOk     = new QPushButton;
    btnCancel = new QPushButton;
    btnOk->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnCancel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    
    // left column
    lblNumber   = new QLabel;
    spinNumber  = new QSpinBox;
    lblType     = new QLabel;
    txtType     = new QLineEdit;
    lblPoints   = new QLabel;
    spinPoints  = new QSpinBox;
    lblDate     = new QLabel;
    dteDate     = new QDateEdit;
    lblWeighting        = new QLabel;
    optWeightingWritten = new QRadioButton;
    optWeightingOral    = new QRadioButton;
    
    //semester Selection
    grpSemester     = new QGroupBox;
    optSemester121  = new QRadioButton("12 / 1");
    optSemester122  = new QRadioButton("12 / 2");
    optSemester131  = new QRadioButton("13 / 1");
    optSemester132  = new QRadioButton("13 / 2");
    optSemesterAuto  = new QRadioButton;
}

void ksExamProperties::createLayouts()
{
    boxBottom = new QDialogButtonBox;
    boxBottom->addButton(btnOk, QDialogButtonBox::AcceptRole);
    boxBottom->addButton(btnCancel, QDialogButtonBox::RejectRole);
    
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
    
    
    layoutParent->addWidget(boxBottom, 5, 0, 1, 3);
    
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
    spinNumber->setMinimum(0);
    spinNumber->setMaximum(999);
    dteDate->setCalendarPopup(TRUE);
    btnOk->setDefault(TRUE);
}

void ksExamProperties::resetWindowTitle()
{
    if (!examToEdit)
    {
        return;
    }
    
    ksPlattformSpec::addMissingExamAttributes(examToEdit);
    
    cDateConverter dateFile;
    dateFile.setDateString(examToEdit->cGetObjectByAttributeValue("name", "date")->
            cGetAttributeByName("value")->value());
    
    
    QString newTitle;
    
    newTitle += tr("Editing");
    if(!newTitle.isEmpty())
    {
        newTitle += " ";
    }
    if(spinNumber->value() > 0)
    {
        newTitle += QString::number(spinNumber->value());
        newTitle += ". ";
    }
    newTitle += txtType->text();
    newTitle += " " + tr("from") + " ";
    newTitle += dateFile.humanDate();
    QString suffix = tr(" ");
    newTitle += suffix.isEmpty() ? "" : " "; // if suffix isn't empy, then add some space
    newTitle += suffix; 
    setWindowTitle(newTitle);
}

void ksExamProperties::setProperties(xmlObject* newProperties)
{
    properties = newProperties;
    if(properties == NULL)
    {
        spinPoints->setMinimum(0);
        spinPoints->setMaximum(99);
        return;
    }
    ksPlattformSpec::addMissingPropertiesAttributes(properties);
    
    int best  = properties->cGetObjectByName("rating")->cGetAttributeByName("best")->nValueToInt();
    int worst = properties->cGetObjectByName("rating")->cGetAttributeByName("worst")->nValueToInt();
    if (best > worst)
    {
        lblPoints->setText(tr("Points:"));
        spinPoints->setMinimum(worst);
        spinPoints->setMaximum(best);
    }
    else
    {
        lblPoints->setText(tr("Mark:"));
        spinPoints->setMinimum(best);
        spinPoints->setMaximum(worst);
    }
}

void ksExamProperties::setExamToEdit(xmlObject* newExamToEdit)
{
    examToEdit = newExamToEdit;
    if (!newExamToEdit)
    {
        QString errormsg = tr("Interner Pointer-Fehler: newExamToEdit = NULL !");
        setWindowTitle(errormsg);
        txtType->setText(errormsg);
        return;
    }
    ksPlattformSpec::addMissingExamAttributes(examToEdit);
    // write class attributes to widgets:
    spinNumber->setValue(examToEdit->cGetObjectByAttributeValue("name", "number")->
                                        cGetAttributeByName("value")->nValueToInt());
    
    txtType->setText(ksPlattformSpec::szToUmlauts(examToEdit->cGetObjectByAttributeValue("name", "type")->
                                        cGetAttributeByName("value")->value()));
    
    spinPoints->setValue(examToEdit->cGetObjectByAttributeValue("name", "mark")->
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
    
    // set window title
    resetWindowTitle();
}


void ksExamProperties::writeWidgetAttributesToExam()
{
    if (!examToEdit)
    {
        QString errormsg = tr("Interner Pointer-Fehler: newExamToEdit = NULL !");
        setWindowTitle(errormsg);
        txtType->setText(errormsg);
        return;
    }
    // we don't need to exec ksPlattformSpec::addMissingExamAttributes(examToEdit);
    // because examToEdit hasn't been changed , because this dialog is modal
    
    examToEdit->cGetObjectByAttributeValue("name", "number")->
            cGetAttributeByName("value")->SetValueToInt(spinNumber->value());
    
    
    examToEdit->cGetObjectByAttributeValue("name", "type")->
            cGetAttributeByName("value")->SetValue(ksPlattformSpec::qstringToSz(txtType->text()));
    
    examToEdit->cGetObjectByAttributeValue("name", "mark")->
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
            cGetAttributeByName("value")->SetValue(ksPlattformSpec::qstringToSz(semester));
    
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
            cGetAttributeByName("value")->SetValue(ksPlattformSpec::qstringToSz(weighting));
    
    
}

