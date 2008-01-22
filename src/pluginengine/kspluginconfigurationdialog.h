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
#ifndef KSPLUGINCONFIGURATIONDIALOG_H
#define KSPLUGINCONFIGURATIONDIALOG_H

#include <QDialog>
class ksPluginEngine;
class ksPluginManager;
class QVBoxLayout;
class QDialogButtonBox;
class QPushButton;
class QEvent;

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class ksPluginConfigurationDialog : public QDialog
{
Q_OBJECT
public:
    ksPluginConfigurationDialog(QWidget *parent = 0);

    ~ksPluginConfigurationDialog();
    
    void setPluginEngine(ksPluginEngine* newPluginEngine);
    void retranslateUi();
    void reloadIcons();
public slots:
    void applyChanges();
    void enterWhatsThisMode();
    
protected:
    void changeEvent(QEvent* event);
private:
    
    void allocateWidgets();
    void createLayouts();
    void connectSlots();
    
    
    // layouts
    QVBoxLayout*        layoutParent;
    
    // widgets
    ksPluginManager*    wdgPlugins;
    QDialogButtonBox*   btnBoxOkCancel;
    QPushButton*        btnOk;
    QPushButton*        btnApply;
    QPushButton*        btnCancel;
    QPushButton*        btnWhatsThis;
    // members
    ksPluginEngine* m_pluginEngine;
};

#endif
