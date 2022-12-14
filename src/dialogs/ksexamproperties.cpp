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
#include <QTextEdit>

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
    grpComment->setTitle(tr("Comment"));
    grpSemester->setTitle(tr("Semester:"));
    optSemesterAuto->setText(tr("Automatical assignment"));
    spinNumber->setSpecialValueText(tr("no number"));
    btnShowComment->setText(tr("Comment"));
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
    optSemesterAuto  = new QRadioButton;
    
    // comment
    grpComment = new QGroupBox;
    txtComment = new QTextEdit;
    txtComment->setAcceptRichText(FALSE);
    txtComment->setMinimumSize(100, 80);
    txtComment->setBaseSize(100, 80);
    txtComment->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
    btnShowComment = new QPushButton;
    btnShowComment->setCheckable(TRUE);
}

void ksExamProperties::createLayouts()
{
    boxBottom = new QDialogButtonBox;
    boxBottom->addButton(btnOk, QDialogButtonBox::AcceptRole);
    boxBottom->addButton(btnCancel, QDialogButtonBox::RejectRole);
    // semester
    layoutSemester = new QGridLayout;
    layoutSemester->addWidget(optSemesterAuto, 0, 0, 1, 2);
    grpSemester->setLayout(layoutSemester);
    // comment
    layoutComment = new QHBoxLayout;
    layoutComment->setMargin(0);
    layoutComment->addWidget(txtComment);
    grpComment->setLayout(layoutComment);
    
    layoutWeighting = new QHBoxLayout;
    layoutWeighting->addWidget(lblWeighting);
    layoutWeighting->addWidget(optWeightingOral);
    layoutWeighting->addWidget(optWeightingWritten);
    
    layoutBottom = new QHBoxLayout;
    layoutBottom->setMargin(0);
    layoutBottom->addWidget(btnShowComment);
    layoutBottom->addWidget(boxBottom);
    
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
    layoutParent->addWidget(grpComment, 5, 0, 1, 3);
    grpComment->setVisible(FALSE);
    
    layoutParent->addLayout(layoutBottom, 6, 0, 1, 3);
    
    setLayout(layoutParent);
}

void ksExamProperties::connectSlots()
{
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(btnOk,     SIGNAL(clicked()), this, SLOT(accept()));
    connect(this,      SIGNAL(accepted()), this, SLOT(writeWidgetAttributesToExam()));
    connect(btnShowComment, SIGNAL(toggled(bool)), grpComment, SLOT(setVisible(bool)));
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
    
    // set best and worst rating
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
    
    // set exam list
    xmlObject* propertiesSemester = properties->cGetObjectByName("time");
    QRadioButton* currentRadioButton = NULL;
    while(lstSemesterList.size() > propertiesSemester->nGetObjectCounter())
    {// while there are to much radio buttons in gui
        currentRadioButton = lstSemesterList.last();
        lstSemesterList.removeLast();
        if(currentRadioButton != NULL)
        {
            layoutSemester->removeWidget(currentRadioButton);
            delete currentRadioButton;
        }
    }
    while(lstSemesterList.size() < propertiesSemester->nGetObjectCounter())
    {// while there are to few radio buttons in gui
        currentRadioButton = new QRadioButton;
        lstSemesterList.append(currentRadioButton);
        int radioButtonIndex = lstSemesterList.size() - 1;
        layoutSemester->addWidget(currentRadioButton, (int)(radioButtonIndex / 2) + 1, radioButtonIndex % 2);
    }
    if(lstSemesterList.size() <= 0)
    {
        // hide semester box if there aren't any semesters
        grpSemester->setVisible(FALSE);
    }
    else
    {
        grpSemester->setVisible(TRUE);
        // copy names
        for(int i = 0; i < lstSemesterList.size(); i++)
        {
            currentRadioButton = lstSemesterList[i];
            currentRadioButton->setText(propertiesSemester->cGetObjectByIdentifier(i)->cGetAttributeByName("name")->value());
        }
    }
    
    
}

void ksExamProperties::setExamToEdit(xmlObject* newExamToEdit)
{
    examToEdit = newExamToEdit;
    if (!newExamToEdit)
    {
        QString errormsg = tr("Internal Pointer-Error: newExamToEdit = NULL !");
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
    else // find semester radio button
    {
        QRadioButton* currentRadioButton = NULL;
        for(int i = 0; i < lstSemesterList.size(); i++)
        {
            currentRadioButton = lstSemesterList[i];
            if(!currentRadioButton)
            {
                continue;
            }
            if(currentRadioButton->text() == semester)
            {
                currentRadioButton->setChecked(TRUE);
            }
        }
    }
    
    QString weighting = examToEdit->cGetObjectByAttributeValue("name", "weighting")->
            cGetAttributeByName("value")->value();
    if(weighting == "written")
        optWeightingWritten->setChecked(TRUE);
    else
        optWeightingOral->setChecked(TRUE);
    
    // comment
    char* szComment = examToEdit->cGetObjectByAttributeValue("name", "comment")->
            szGetContent();
    txtComment->setPlainText(ksPlattformSpec::szToUmlauts(szComment));
    
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
    else // find checked radio button
    {
        QRadioButton* currentRadioButton = NULL;
        for(int i = 0; i < lstSemesterList.size(); i++)
        {
            currentRadioButton = lstSemesterList[i];
            if(currentRadioButton && currentRadioButton->isChecked())
            {
                semester = currentRadioButton->text();
                break;
            }
        }
        
    }
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
    
    // COMMENT
    QString szComment = txtComment->toPlainText();
    
    examToEdit->cGetObjectByAttributeValue("name", "comment")->
            nSetContent(ksPlattformSpec::qstringToSz(szComment));
    
}

