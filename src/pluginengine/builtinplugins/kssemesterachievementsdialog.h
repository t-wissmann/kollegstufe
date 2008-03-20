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
#ifndef KSSEMESTERACHIEVEMENTSDIALOG_H
#define KSSEMESTERACHIEVEMENTSDIALOG_H

#include <QMainWindow>
#include <QList>
#include <QString>
/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/

class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;

class QLabel;
class QComboBox;
class QSpinBox;
class QCheckBox;
class QDockWidget;

class xmlObject;
class ksPlugin;
class ksConfigContainer;

class QCloseEvent;
class QShowEvent;
class QEvent;

#include<widgets/selectablelabel.h>

class SemesterAchievementWidget : public SelectableLabel
{
public:
    SemesterAchievementWidget()
    {
        m_fMark = 0;
    }
    double m_fMark;
    QString m_szSemester;
    QString m_szSubject;
    bool operator<(const SemesterAchievementWidget& other) const{
        return m_fMark < other.m_fMark;
    }
    bool operator>(const SemesterAchievementWidget& other) const{
        return m_fMark > other.m_fMark;
    }
    bool operator==(const SemesterAchievementWidget& other) const{
        return m_fMark == other.m_fMark;
    }
    bool operator<=(const SemesterAchievementWidget& other) const{
        return m_fMark <= other.m_fMark;
    }
    bool operator>=(const SemesterAchievementWidget& other) const{
        return m_fMark >= other.m_fMark;
    }
};

class ksSemesterAchievementsDialog : public QMainWindow
{
Q_OBJECT
signals:
    void visibilityChanged(bool visible);
public:
    ksSemesterAchievementsDialog(QWidget *parent = 0);

    ~ksSemesterAchievementsDialog();
    void setArchive(xmlObject* dataPart, xmlObject* semesterList, xmlObject* currentCategory = NULL);
    
    void retranslateUi();
    void refreshSemesterLabelList();
    
    xmlObject* selectedCategory();
    
    void setColumnCount(int columnCount);
    int columnCount() const;
    
    void setPluginParent(ksPlugin* parent);
    
    void loadUiConfig(ksConfigContainer* config);
public slots:
    void categoryChanged(int index);
    void spinboxColumnCountChanged();
    void reloadLabelListToLayout();
    void sortSemesterAchievements();
    void saveUiStateToConfig();
protected:
    void closeEvent(QCloseEvent*);
    void showEvent(QShowEvent*);
    void changeEvent(QEvent* event);
private:
    void createGui();
    // widgets
    QLabel*      lblCategorySelection;
    QComboBox*   cmbCategorySelection;
    
    QLabel*      lblColumnCount;
    QSpinBox*    spinColumnCount;
    
    QLabel*      lblMaxLabelCount;
    QSpinBox*    spinMaxLabelCount;
    
    QCheckBox*   chkAscendingOrder;
    
    
    // layouts
    QGridLayout* layoutSemesterAchievements;
    
    QVBoxLayout* layoutControlSidebar; // actually left
    QDockWidget* dockControlSidebar;
    QWidget*     wdgControlSidebar;
    QHBoxLayout* layoutParent;
    QWidget*     wdgCentral;
    // sidebar layouts
    QHBoxLayout* layoutCategorySelection;
    QHBoxLayout* layoutColumnCount;
    QHBoxLayout* layoutMaxLabelCount;
    
    // members
    ksPlugin*  m_pPluginParent;
    xmlObject* m_pDataPart;
    xmlObject* m_pSemesterList;
    ksConfigContainer* m_pCurrentConfig;
    
    QList<SemesterAchievementWidget*> lstSemesterAchievements;
};

#endif
