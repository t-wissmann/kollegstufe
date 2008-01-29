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
#ifndef KSCONFIGURE_H
#define KSCONFIGURE_H

#include <QDialog>
#include <QTranslator>
#include <QStringList>

class ksPluginManager;

class QEvent;
class QPushButton;
class QLabel;
class QFrame;
class QTextEdit;
class QGroupBox;
class QListWidget;

// layouts
class QDialogButtonBox;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;

class xmlObject;

/**
	@author Thorsten Wissmann <towi89@web.de>
*/
class ksConfigure : public QDialog
{
Q_OBJECT
        
public slots:
    void applyChanges();
public:
    ksConfigure(QWidget *parent = 0);
    ~ksConfigure();
    
    void retranslateUi();
    void setConfigToEdit(xmlObject* newConfig);
    void reloadIcons();
    
protected:
    virtual void changeEvent(QEvent* event);
    
private:
    void allocateWidgets();
    void createLayouts();
    void connectSlots();
    void initWidgets();
    void catchLanguageList();
    
    //layouts
    QGridLayout* layoutParent;
    QDialogButtonBox* boxBottom;
    
    // Widgets
    QPushButton* btnOk;
    QPushButton* btnCancel;
    
    QGridLayout* layoutLanguageSelection;
    QListWidget* lstLanguageSelection;
    QGroupBox*   grpLanguageSelection;
    
    
    QStringList  languageList;
    xmlObject*   config;
};

#endif
