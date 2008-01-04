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
#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QString>
#include <QList>

class QDialogButtonBox;
class QPushButton;
class QLabel;
class QScrollArea;
class QMenu;

class QEvent;
class QVBoxLayout;
class QHBoxLayout;

class ksConfigContainer;
class ksConfigOptionWidget;

class QAction;
class QActionGroup;
/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/

class ConfigItem
{
public:
    ConfigItem(){
        mnaMenuItem = NULL;
        m_pConfig = NULL;
    };
    QAction*    mnaMenuItem;
    ksConfigContainer*  m_pConfig;
    void operator=(const ConfigItem &other){
        mnaMenuItem = other.mnaMenuItem;
        m_pConfig = other.m_pConfig;
    };
    
};

class ConfigDialog : public QDialog
{
Q_OBJECT
signals:
    void configurationChanged();
public:
    ConfigDialog(QWidget *parent = 0);
    
    ~ConfigDialog();
    
    void retranslateUi();
    void setTitle(QString title, QString description = "");
    void setConfigContainer(ksConfigContainer* config);
    ksConfigContainer* currentConfigContainer() const { return m_pConfigContainer; };
    void refreshGuiFromConfig(ksConfigContainer* config);
    
    void clearConfigSelectionMenu();
    void addToConfigSelectionMenu(ksConfigContainer* config, const QString& name);
    
    void setConfigIdentifier(QString id);
    QString configIdentifier() const { return m_szConfigIdentifier; };
    
public slots:
    void applyChanges();
    void enterWhatsThisMode(bool on = TRUE);
    void configSelected(QAction* action);
    void exportConfig();
    void importConfig();
    
protected:
    virtual void changeEvent(QEvent* event);
private:
    
    void  allocateWidgets();
    void  createLayouts();
    void  connectSlots();
    
    void  refreshHeaderFromTitle();
    
    //layouts
    QVBoxLayout*        layoutParent;
    QVBoxLayout*        layoutOptionWidgets;
    QHBoxLayout*        layoutConfigSelection;
    
    // widgets
    QDialogButtonBox*   btnBoxBottom;
    QPushButton*        btnOk;
    QPushButton*        btnApply;
    QPushButton*        btnCancel;
    QPushButton*        btnWhatsThis;
    QScrollArea*        scrollCentralOptions;
    QLabel*             lblHeader;
    QList<ksConfigOptionWidget*> listConfigOptions;
    QWidget*            wdgConfigOptions;
    QPushButton*        btnConfigSelection;
    QLabel*             lblConfigSelection;
    QMenu*              mnuConfigSelection;
    QAction*            mnaConfigSelectionSeparator;
    QAction*            mnaConfigExport;
    QAction*            mnaConfigImport;
    QActionGroup*       acgrpConfigSelection;
    QList<ConfigItem>   listConfigMenuItems;
    
    
    
    // members
    ksConfigContainer*  m_pConfigContainer;
    QString             m_szConfigIdentifier;
    QString             m_szTitle;
    QString             m_szDescription;
    
};

#endif
