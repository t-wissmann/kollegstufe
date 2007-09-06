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
#include "kssubjectproperties.h"
#include "ksplattformspec.h"

#include "xmlparser.h"

#include <QString>
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
}


ksSubjectProperties::~ksSubjectProperties()
{
}




void ksSubjectProperties::allocateWidgets()
{
    btnOk     = new QPushButton("Ok");
    btnCancel = new QPushButton("Abbrechen");
    
    lblName   = new QLabel("Name:");
    txtName   = new QLineEdit;
    lblTeacher= new QLabel("Lehrer:");
    txtTeacher= new QLineEdit;
    
}

void ksSubjectProperties::createLayouts()
{
    layoutBottom = new QHBoxLayout;
    layoutBottom->addStretch(1);
    layoutBottom->addWidget(btnOk);
    layoutBottom->addWidget(btnCancel);
    
    layoutParent = new QGridLayout;
    layoutParent->addWidget(lblName, 0, 0);
    layoutParent->addWidget(txtName, 0, 1);
    layoutParent->addWidget(lblTeacher, 1, 0);
    layoutParent->addWidget(txtTeacher, 1, 1);
    layoutParent->addLayout(layoutBottom, 2,0, 1, 2);
    
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
    resize(320, 140);
}

void ksSubjectProperties::setSubjectToEdit(xmlObject* newSubjectToEdit)
{
    subjectToEdit = newSubjectToEdit;
    if(subjectToEdit == NULL)
    {
        QString errormsg = QString::fromLocal8Bit("pointerfehler in ksSubjectProperties::setSubjectToEdit, NULL wurde als newSubjectToEdit übergeben");
        setWindowTitle(errormsg);
        return;
    }
    ksPlattformSpec::addMissingSubjectAttributes(subjectToEdit);
    txtName->setText(subjectToEdit->cGetAttributeByName("name")->value());
    txtTeacher->setText(subjectToEdit->cGetAttributeByName("teacher")->value());
    
    QString newWindowTitle;
    newWindowTitle  = "Eigenschaften des Faches \'";
    newWindowTitle += txtName->text();
    newWindowTitle += "\'";
    
    setWindowTitle(newWindowTitle);
}


void ksSubjectProperties::writeWidgetAttributesToSubject()
{
    if(subjectToEdit == NULL)
    {
        QString errormsg = QString::fromLocal8Bit("pointerfehler in ksSubjectProperties::setSubjectToEdit, NULL wurde als newSubjectToEdit übergeben");
        setWindowTitle(errormsg);
        return;
    }
    subjectToEdit->cGetAttributeByName("name")->SetValue(txtName->text().toAscii().data());
    subjectToEdit->cGetAttributeByName("teacher")->SetValue(txtTeacher->text().toAscii().data());
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



