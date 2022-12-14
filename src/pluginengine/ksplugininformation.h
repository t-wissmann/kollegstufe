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
#ifndef KSPLUGININFORMATION_H
#define KSPLUGININFORMATION_H

#include <QObject>
class xmlObject;
class kollegstufeParent;
class QMenu;

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class ksPluginInformation : public QObject{
    Q_OBJECT
public:
    ksPluginInformation();

    ~ksPluginInformation();
    
signals:
    void currentDatabaseChanged();
public:
    void setMainWindow(kollegstufeParent*  newMainWindow) { m_pMainWindow = newMainWindow; };
    kollegstufeParent* mainWindow() const { return m_pMainWindow; };
    
    void setAllDatabasePointersToNull();
    
    void setCurrentDatabase(xmlObject*  newCurrentDatabase) {
        if(m_pCurrentDatabase != newCurrentDatabase)
        {
            m_pCurrentDatabase = newCurrentDatabase;
            emit currentDatabaseChanged();
        }
    };
    xmlObject* currentDatabase() const { return m_pCurrentDatabase; };
    
    void setCurrentCategory(xmlObject*  newCurrentCategory) { m_pCurrentCategory = newCurrentCategory; };
    xmlObject* currentCategory() const { return m_pCurrentCategory; };
    
    void setCurrentSubject(xmlObject*  newCurrentSubject) { m_pCurrentSubject = newCurrentSubject; };
    xmlObject* currentSubject() const { return m_pCurrentSubject; };
    
    void setCurrentExam(xmlObject*  newCurrentExam) { m_pCurrentExam = newCurrentExam; };
    xmlObject* currentExam() const { return m_pCurrentExam; };
    
    void setCurrentPropertyPart(xmlObject*  newPropertyPart) { m_pCurrentPropertyPart = newPropertyPart; };
    xmlObject* currentPropertyPart() const { return m_pCurrentPropertyPart; };
    
    void setPluginMenu(QMenu* newMenu) { m_pPluginMenu = newMenu; };
    QMenu* pluginMenu() const { return m_pPluginMenu; };
    
    void setCurrentDataPart(xmlObject* newDataPart) { m_pCurrentDataPart = newDataPart; };
    xmlObject* currentDataPart() const { return m_pCurrentDataPart; };

    
private:
    
    kollegstufeParent*  m_pMainWindow;
    
    xmlObject*  m_pCurrentDatabase;
    xmlObject*  m_pCurrentCategory;
    xmlObject*  m_pCurrentSubject;
    xmlObject*  m_pCurrentPropertyPart;
    xmlObject*  m_pCurrentDataPart;
    xmlObject*  m_pCurrentExam;
    QMenu*      m_pPluginMenu;
};

#endif
