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
#ifndef KSSUBJECTINFORMATIONWIDGET_H
#define KSSUBJECTINFORMATIONWIDGET_H

#include <QWidget>
#include <QString>

class xmlObject;

class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;

class QEvent;
class QLabel;
/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class ksSubjectInformationWidget : public QWidget
{
Q_OBJECT
public:
    ksSubjectInformationWidget(QWidget *parent = 0);

    ~ksSubjectInformationWidget();
    
    void setSubjectToShow(xmlObject* subject);
    void retranslateUi();
    void computeAverages();
    void printDataToWidgets();
    void setSemesterToShow(QString semester);
    void setProperties(xmlObject* newProperties);
protected:
    virtual void changeEvent(QEvent* event);
private:
    void createWidgets();
    void createLayouts();
    void initWidgets();
    
    //widgets
    QLabel*         lblOralLabel;
    QLabel*         lblWrittenLabel;
    QLabel*         lblEntireLabel;
    QLabel*         lblOralAverage;
    QLabel*         lblWrittenAverage;
    QLabel*         lblEntireAverage;
    
    
    //layouts
    QGridLayout*    layoutParent;
    
    //member
    double      averageOral;
    double      averageWritten;
    double      averageEntire;
    QString     semesterToShow;
    xmlObject*  subjectToShow;
    xmlObject*  properties;
};

#endif
