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
#include "kssubjectproperties.h"
#include <core/ksplattformspec.h>
#include <io/ksiconcatcher.h>
#include <io/xmlparser.h>

#include <QString>
#include <QEvent>

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


ksSubjectProperties::ksSubjectProperties(QWidget *parent)
 : QDialog(parent)
{
    setModal(TRUE);
    currentCathegory = NULL;
    subjectToEdit    = NULL;
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


ksSubjectProperties::~ksSubjectProperties()
{
}


void ksSubjectProperties::changeEvent(QEvent* event)
{
    QDialog::changeEvent(event);
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
}


void ksSubjectProperties::retranslateUi()
{
    btnOk->setText(tr("Ok"));
    btnCancel->setText(tr("Cancel"));
    lblName->setText(tr("Name:"));
    lblTeacher->setText(tr("Teacher:"));
    
    lblWeightingPercentage->setText(tr("Weighting percentage:"));
    lblWeightingTo->setText(tr("to"));
    lblWeightingToExplanation->setText(lblWeightingTo->text());
    lblWeightingWritten->setText(tr("Written"));
    lblWeightingOral->setText(tr("Oral"));
}

void ksSubjectProperties::reloadIcons()
{
    btnOk->setIcon(ksIconCatcher::getIcon("button_ok", 16));
    btnCancel->setIcon(ksIconCatcher::getIcon("button_cancel", 16));
}


void ksSubjectProperties::allocateWidgets()
{
    btnOk     = new QPushButton;
    btnCancel = new QPushButton;
    
    lblName   = new QLabel;
    txtName   = new QLineEdit;
    lblTeacher= new QLabel;
    txtTeacher= new QLineEdit;
    
    
    lblWeightingPercentage = new QLabel;
    spinWeightingOral = new QSpinBox;
    spinWeightingWritten = new QSpinBox;
    lblWeightingTo = new QLabel;
    lblWeightingToExplanation = new QLabel;
    lblWeightingWritten = new QLabel;
    lblWeightingOral = new QLabel;
}

void ksSubjectProperties::createLayouts()
{
    layoutBottom = new QHBoxLayout;
    layoutBottom->addStretch(1);
    layoutBottom->addWidget(btnOk);
    layoutBottom->addWidget(btnCancel);
    
    layoutWeightingPercentage = new QGridLayout;
    layoutWeightingPercentage->addWidget(lblWeightingPercentage, 0, 0, 2, 1);
    layoutWeightingPercentage->addWidget(lblWeightingOral, 0, 1);
    layoutWeightingPercentage->addWidget(lblWeightingToExplanation, 0, 2);
    layoutWeightingPercentage->addWidget(lblWeightingWritten, 0, 3);
    layoutWeightingPercentage->addWidget(spinWeightingOral, 1, 1);
    layoutWeightingPercentage->addWidget(lblWeightingTo, 1, 2);
    layoutWeightingPercentage->addWidget(spinWeightingWritten, 1, 3);
    
    
    layoutParent = new QGridLayout;
    layoutParent->addWidget(lblName, 0, 0);
    layoutParent->addWidget(txtName, 0, 1);
    layoutParent->addWidget(lblTeacher, 1, 0);
    layoutParent->addWidget(txtTeacher, 1, 1);
    layoutParent->addLayout(layoutWeightingPercentage, 2,0, 1, 2);
    layoutParent->addLayout(layoutBottom, 3,0, 1, 2);
    
    setLayout(layoutParent);
    
}

void ksSubjectProperties::connectSlots()
{
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(btnOk,     SIGNAL(clicked()), this, SLOT(accept()));
    connect(this,      SIGNAL(accepted()), this, SLOT(writeWidgetAttributesToSubject()));
    connect(txtName,   SIGNAL(textChanged(QString)), this, SLOT(checkForMultibleNames()));
}

void ksSubjectProperties::initWidgets()
{
    spinWeightingOral->setMinimum(1);
    spinWeightingOral->setMaximum(99);
    spinWeightingWritten->setMinimum(1);
    spinWeightingWritten->setMaximum(99);
    
    lblWeightingTo->setAlignment(Qt::AlignHCenter);
    lblWeightingToExplanation->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    lblWeightingWritten->setAlignment(Qt::AlignHCenter);
    lblWeightingOral->setAlignment(Qt::AlignHCenter);
    resize(320, 140);
}

void ksSubjectProperties::setSubjectToEdit(xmlObject* newSubjectToEdit)
{
    subjectToEdit = newSubjectToEdit;
    if(subjectToEdit == NULL)
    {
        QString errormsg = tr("pointerfehler in ksSubjectProperties::setSubjectToEdit, NULL wurde als newSubjectToEdit uebergeben");
        setWindowTitle(errormsg);
        return;
    }
    // write attributes to widgets
    ksPlattformSpec::addMissingSubjectAttributes(subjectToEdit);
    txtName->setText(ksPlattformSpec::szToUmlauts(subjectToEdit->cGetAttributeByName("name")->value()));
    txtTeacher->setText(ksPlattformSpec::szToUmlauts(subjectToEdit->cGetAttributeByName("teacher")->value()));
    spinWeightingOral->setValue(subjectToEdit->cGetAttributeByName("weightingOral")->nValueToInt());
    spinWeightingWritten->setValue(subjectToEdit->cGetAttributeByName("weightingWritten")->nValueToInt());
    
    QString newWindowTitle;
    newWindowTitle  = tr("Properties of subject \'");
    newWindowTitle += txtName->text();
    newWindowTitle += tr("\'");
    
    setWindowTitle(newWindowTitle);
}


void ksSubjectProperties::writeWidgetAttributesToSubject()
{
    if(subjectToEdit == NULL)
    {
        QString errormsg = tr("pointerfehler in ksSubjectProperties::setSubjectToEdit, NULL wurde als newSubjectToEdit uebergeben");
        setWindowTitle(errormsg);
        return;
    }
    subjectToEdit->cGetAttributeByName("name")->SetValue(ksPlattformSpec::qstringToSz(txtName->text()));
    subjectToEdit->cGetAttributeByName("teacher")->SetValue(ksPlattformSpec::qstringToSz(txtTeacher->text()));
    subjectToEdit->cGetAttributeByName("weightingOral")->SetValueToInt(spinWeightingOral->value());
    subjectToEdit->cGetAttributeByName("weightingWritten")->SetValueToInt(spinWeightingWritten->value());
}

void ksSubjectProperties::setCathegoryOfSubject(xmlObject* newCathegory)
{
    currentCathegory = newCathegory;
    checkForMultibleNames();
}


void ksSubjectProperties::checkForMultibleNames()
{
    if(currentCathegory == NULL)
    {
        return;
    }
    if(subjectToEdit == NULL)
    {
        return;
    }
    bool uniqueName = TRUE;
    for (int i = 0; i < currentCathegory->nGetObjectCounter(); i++)
    {
        xmlObject*  currentSubject = currentCathegory->cGetObjectByIdentifier(i);
        if(currentSubject == subjectToEdit)
        {
            continue;
        }
        if (currentSubject == NULL)
        {
            return;
        }
        if(currentSubject->cGetAttributeByName("name") == NULL)
        {
            return;
        }
        if(currentSubject->cGetAttributeByName("name")->value() == txtName->text())
        {
            uniqueName = FALSE;
        }
    }
    
    btnOk->setEnabled(uniqueName);
}



