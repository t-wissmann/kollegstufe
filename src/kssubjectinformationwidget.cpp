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
#include "kssubjectinformationwidget.h"

#include "ksplattformspec.h"
#include "xmlparser.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QMessageBox>
#include <QSizePolicy>
#include <QLabel>
#include <QEvent>
#include <QString>

ksSubjectInformationWidget::ksSubjectInformationWidget(QWidget *parent)
 : QWidget(parent)
{
    subjectToShow = NULL;
    properties = NULL;
    createWidgets();
    createLayouts();
    retranslateUi();
    semesterToShow = "all";
    setSubjectToShow(NULL);
    
}


ksSubjectInformationWidget::~ksSubjectInformationWidget()
{
}


void ksSubjectInformationWidget::createWidgets()
{
    lblOralLabel = new QLabel;
    lblWrittenLabel = new QLabel;
    lblEntireLabel = new QLabel;
    lblOralAverage = new QLabel;
    lblWrittenAverage = new QLabel;
    lblEntireAverage = new QLabel;
}

void ksSubjectInformationWidget::createLayouts()
{
    layoutParent = new QGridLayout;
    layoutParent->addWidget(lblOralLabel, 0, 0);
    layoutParent->addWidget(lblWrittenLabel, 1, 0);
    layoutParent->addWidget(lblEntireLabel, 2, 0);
    layoutParent->addWidget(lblOralAverage, 0, 1);
    layoutParent->addWidget(lblWrittenAverage, 1, 1);
    layoutParent->addWidget(lblEntireAverage, 2, 1);
    setLayout(layoutParent);
}

void ksSubjectInformationWidget::initWidgets()
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void ksSubjectInformationWidget::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
}


void ksSubjectInformationWidget::retranslateUi()
{
    lblOralLabel->setText(tr("Oral average:"));
    lblWrittenLabel->setText(tr("Written average:"));
    lblEntireLabel->setText(tr("Entire average:"));
}


void ksSubjectInformationWidget::setSubjectToShow(xmlObject* subject)
{
    subjectToShow = subject;
    computeAverages();
    printDataToWidgets();
}

void ksSubjectInformationWidget::computeAverages()
{
    if(!subjectToShow || !properties)
    {
        averageOral = -1;
        averageWritten = -1;
        averageEntire = -1;
        return;
    }
    ksPlattformSpec::addMissingSubjectAttributes(subjectToShow);
    
    int weightingOral = subjectToShow->cGetAttributeByName("weightingOral")->nValueToInt();
    int weightingWritten = subjectToShow->cGetAttributeByName("weightingWritten")->nValueToInt();
    
    averageOral = ksPlattformSpec::computeAverageOfSubject(subjectToShow, "oral", semesterToShow, properties->cGetObjectByName("time"));
    averageWritten = ksPlattformSpec::computeAverageOfSubject(subjectToShow, "written", semesterToShow, properties->cGetObjectByName("time"));
    
    averageEntire = ksPlattformSpec::computeEntireAverageFromPartAverages(averageOral, averageWritten, weightingOral, weightingWritten);
    
    averageOral = (double)((int)(averageOral*100))/100; // only show two decimals
    averageWritten = (double)((int)(averageWritten*100))/100;
    //averageEntire = (double)((int)(averageEntire*100))/100;
    
}

void ksSubjectInformationWidget::printDataToWidgets()
{
    if(averageOral != -1)
    {
        lblOralAverage->setText(QString::number(averageOral));
    }
    else
    {
        lblOralAverage->setText("-");
    }
    if(averageWritten != -1)
    {
        lblWrittenAverage->setText(QString::number(averageWritten));
    }
    else
    {
        lblWrittenAverage->setText("-");
    }
    if(averageEntire != -1)
    {
        lblEntireAverage->setText(QString::number(averageEntire));
    }
    else
    {
        lblEntireAverage->setText("-");
    }
    
}

void ksSubjectInformationWidget::setProperties(xmlObject* newProperties)
{
    properties = newProperties;
}

void ksSubjectInformationWidget::setSemesterToShow(QString semester)
{
    semesterToShow = semester;
    setSubjectToShow(subjectToShow); // refresh gui
}




