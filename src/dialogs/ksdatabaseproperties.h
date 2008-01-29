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
#ifndef KSDATABASEPROPERTIES_H
#define KSDATABASEPROPERTIES_H

#include <QDialog>

class ksCathegoryEditorWidget;
class xmlObject;

class QEvent;

class QPushButton;
class QDateEdit;
class QLineEdit;
class QGroupBox;
class QRadioButton;
class QLabel;
class QSpinBox;
class QStackedWidget;
class QListWidget;
class QTextEdit;

// layouts
class QDialogButtonBox;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;


/**
	@author Thorsten Wissmann <towi89@web.de>
*/
class ksDatabaseProperties : public QDialog
{
Q_OBJECT
public:
    ksDatabaseProperties(QWidget *parent = 0);

    ~ksDatabaseProperties();
    void setDatabasePropertiesToEdit(xmlObject* newProperties);
    void setDatabaseToEdit(xmlObject* newDatabase);
    void retranslateUi();
    void reloadIcons();
public slots:
    void writeWidgetAttributesToProperties();
    void setMinMaxDate();
    void setBtnOkNotDefault(bool newDefaultValue);
protected:
    virtual void changeEvent(QEvent* event);
private:
    void    allocateWidgets();
    void    createLayouts();
    void    connectSlots();
    
    // date - min max 
    void    connectMinMaxDate();
    void    disconnectMinMaxDate();
    
    QPushButton*    btnOk;
    QPushButton*    btnCancel;
    
    QLabel*         lblName;
    QLineEdit*      txtName;
    
    // mode selection:
    QLabel*         lblRating;
    QRadioButton*   optRatingMarks;
    QRadioButton*   optRatingPoints;
    QRadioButton*   optRatingOther;
    QSpinBox*       spinRatingBest;
    QSpinBox*       spinRatingWorst;
    QLabel*         lblRatingBest;
    QLabel*         lblRatingWorst;
    QGridLayout*    layoutRatingOther;
    
    // semester List
    QGroupBox*      grpSemesterList;
    QTextEdit*      lblSemesterListInformation;
    QLabel*         lblStart;
    QLabel*         lblEnd;
    
    QLabel*         lbl121;
    QLabel*         lbl122;
    QLabel*         lbl131;
    QLabel*         lbl132;
    
    QDateEdit*      dteSemester121Start;
    QDateEdit*      dteSemester122Start;
    QDateEdit*      dteSemester131Start;
    QDateEdit*      dteSemester132Start;
    QDateEdit*      dteSemester121End;
    QDateEdit*      dteSemester122End;
    QDateEdit*      dteSemester131End;
    QDateEdit*      dteSemester132End;
    
    QGroupBox*                  grpCathegories;
    QHBoxLayout*                layoutCathegories;
    ksCathegoryEditorWidget*    wdgCathegoryEditor;
    
    // Layouts
    QDialogButtonBox* boxBottom;
    QGridLayout*    layoutSemesterList;
    QStackedWidget* stackMain;
    QGridLayout*    layoutParent;
    
    QListWidget*    lstStackMainControl;
    xmlObject*      propertiesToEdit;
    xmlObject*      databaseToEdit;
    
};

#endif
