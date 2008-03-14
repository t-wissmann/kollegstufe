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
#include "../../pluginengine/builtinplugins/kssemesterachievementsdialog.h"

#include "../ksplugin.h"
#include <io/xmlparser.h>
#include <widgets/selectablelabel.h>
#include <core/ksplattformspec.h>
#include <core/ksconfigoption.h>
#include <core/ksconfigcontainer.h>

#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QDockWidget>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>


class SAWPointer{ // semester achievement pointer 
public:
    SAWPointer(){
        widget = 0;
    }
    SAWPointer(SemesterAchievementWidget* target){
        widget = target;
    }
    // operators
    bool operator <(const SAWPointer& other) const{
        return (*widget) < *(other.widget);
    }
    bool operator >(const SAWPointer& other) const{
        return (*widget) > *(other.widget);
    }
    bool operator >=(const SAWPointer& other) const{
        return (*widget) >= *(other.widget);
    }
    bool operator <=(const SAWPointer& other) const{
        return (*widget) <= *(other.widget);
    }
    bool operator ==(const SAWPointer& other) const{
        return (*widget) == *(other.widget);
    }
    void operator =(const SAWPointer& other)
    {
        widget = other.widget;
    }
    // member
    SemesterAchievementWidget* widget;
    
};

ksSemesterAchievementsDialog::ksSemesterAchievementsDialog(QWidget *parent)
    : QMainWindow(parent)
{
    m_pDataPart = NULL;
    m_pPluginParent = NULL;
    m_pSemesterList = NULL;
    m_pCurrentConfig = NULL;
    setAttribute(Qt::WA_QuitOnClose, FALSE);
    createGui();
    retranslateUi();
}


ksSemesterAchievementsDialog::~ksSemesterAchievementsDialog()
{
}


void ksSemesterAchievementsDialog::retranslateUi()
{
    lblCategorySelection->setText(tr("Category:"));
    lblColumnCount->setText(tr("Column Count:"));
    lblColumnCount->setWhatsThis(tr("Here you can configure, in how many columns the semester achievements shall be shown."));
    spinColumnCount->setWhatsThis(lblColumnCount->whatsThis());
    spinMaxLabelCount->setSpecialValueText(tr("show all"));
    lblMaxLabelCount->setText(tr("Number of shown\nsemester achievements:"));
    chkAscendingOrder->setText(tr("Sort items to ascending order"));
    chkAscendingOrder->setWhatsThis(tr("If checked, the order Semester Achievements "
            "will show these items by an ascending average. If not checked, the average order is descending."));
    setWindowTitle(tr("Semester Achievements"));
    dockControlSidebar->setWindowTitle(tr("Configuration"));
}

void ksSemesterAchievementsDialog::createGui()
{
    // category selection
    lblCategorySelection = new QLabel;
    cmbCategorySelection = new QComboBox;
    
    // column count
    spinColumnCount = new QSpinBox;
    lblColumnCount  = new QLabel;
    spinColumnCount->setMinimum(1);
    spinColumnCount->setMaximum(20);
    spinColumnCount->setValue(5);
    // max label count
    lblMaxLabelCount = new QLabel;
    spinMaxLabelCount = new QSpinBox;
    spinMaxLabelCount->setMinimum(0);
    spinMaxLabelCount->setMaximum(99);
    spinMaxLabelCount->setValue(22);
    // sorting order
    chkAscendingOrder = new QCheckBox;
    chkAscendingOrder->setChecked(FALSE);
    
    // connect slots
    connect(cmbCategorySelection, SIGNAL(currentIndexChanged( int )), this, SLOT(categoryChanged(int)));
    connect(spinColumnCount, SIGNAL(valueChanged( int )), this, SLOT(spinboxColumnCountChanged()));
    connect(spinMaxLabelCount, SIGNAL(valueChanged(int)), this, SLOT(spinboxColumnCountChanged()));
    connect(chkAscendingOrder, SIGNAL(stateChanged(int)), this, SLOT(sortSemesterAchievements()));
    
    
    
    // layouts
    layoutSemesterAchievements = new QGridLayout;
    layoutSemesterAchievements->setMargin(0);
    
    layoutCategorySelection = new QHBoxLayout;
    layoutCategorySelection->setMargin(0);
    layoutCategorySelection->addWidget(lblCategorySelection);
    layoutCategorySelection->addWidget(cmbCategorySelection);
    
    layoutColumnCount = new QHBoxLayout;
    layoutColumnCount->setMargin(0);
    layoutColumnCount->addWidget(lblColumnCount);
    layoutColumnCount->addWidget(spinColumnCount);
    
    layoutMaxLabelCount = new QHBoxLayout;
    layoutMaxLabelCount->setMargin(0);
    layoutMaxLabelCount->addWidget(lblMaxLabelCount);
    layoutMaxLabelCount->addWidget(spinMaxLabelCount);
    
    layoutControlSidebar = new QVBoxLayout;
    //layoutControlSidebar->setMargin(0);
    layoutControlSidebar->addLayout(layoutCategorySelection);
    layoutControlSidebar->addLayout(layoutMaxLabelCount);
    layoutControlSidebar->addWidget(chkAscendingOrder);
    layoutControlSidebar->addLayout(layoutColumnCount);
    layoutControlSidebar->addStretch(0);
    
    wdgControlSidebar = new QWidget;
    wdgControlSidebar->setLayout(layoutControlSidebar);
    
    dockControlSidebar = new QDockWidget;
    dockControlSidebar->setWidget(wdgControlSidebar);
    dockControlSidebar->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockControlSidebar->setFeatures(QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::LeftDockWidgetArea, dockControlSidebar);
    
    layoutParent = new QHBoxLayout;
    //layoutParent->addLayout(layoutControlSidebar);
    layoutParent->addLayout(layoutSemesterAchievements);
    
    wdgCentral = new QWidget;
    wdgCentral->setLayout(layoutParent);
    setCentralWidget(wdgCentral);
    
    // connect other slots
    connect(spinColumnCount, SIGNAL(valueChanged(int)), this, SLOT(saveUiStateToConfig()));
    connect(spinMaxLabelCount, SIGNAL(valueChanged(int)), this, SLOT(saveUiStateToConfig()));
    connect(chkAscendingOrder, SIGNAL(stateChanged(int)), this, SLOT(saveUiStateToConfig()));
    connect(dockControlSidebar, SIGNAL(dockLocationChanged (Qt::DockWidgetArea)), this, SLOT(saveUiStateToConfig()));
    
    
}


void ksSemesterAchievementsDialog::setArchive(xmlObject* dataPart, xmlObject* semesterList, xmlObject* currentCategory)
{
    m_pDataPart = dataPart;
    m_pSemesterList = semesterList;
    
    // refresh category combo box
    xmlObject* categoryIterator = NULL;
    cmbCategorySelection->clear();
    int indexToSelect = 0;
    for(int i = 0; i < m_pDataPart->nGetObjectCounter(); ++i)
    {
        categoryIterator = m_pDataPart->cGetObjectByIdentifier(i);
        if(!categoryIterator)
        {
            continue;
        }
        ksPlattformSpec::addMissingCathegoryAttributes(categoryIterator);
        cmbCategorySelection->addItem(ksPlattformSpec::szToUmlauts(categoryIterator->cGetAttributeByName("name")->value()));
        if(categoryIterator == currentCategory)
        {
            indexToSelect = i;
        }
    }
    cmbCategorySelection->setCurrentIndex(indexToSelect);
    
    refreshSemesterLabelList();
}

void ksSemesterAchievementsDialog::refreshSemesterLabelList()
{
    if(!m_pDataPart || !m_pSemesterList)
    {
        qDebug("m_pDataPart is %d\n m_pSemesterList is %d", m_pDataPart, m_pSemesterList);
        return;
    }
    // add missing semester properties
    for(int i = 0; i < m_pSemesterList->nGetObjectCounter(); i++)
    {
        ksPlattformSpec::addMissingSemesterAttributes(m_pSemesterList->cGetObjectByIdentifier(i));
        
    }
    
    xmlObject* currentSubject = NULL;
    int currentLabelIndex = 0;
    xmlObject* currentCategory = selectedCategory();
    SemesterAchievementWidget* currentLabel = NULL;
    double average = -1;
    xmlObject* currentSemester = NULL;
    if(!currentCategory)
    {
        return;
    }
    for(int subject = 0; subject < currentCategory->nGetObjectCounter(); subject++)
    {
        currentSubject = currentCategory->cGetObjectByIdentifier(subject);
        if(!currentSubject)
        {
            continue;
        }
        if(currentSubject->name() != QString("subject"))
        {
            continue;
        }
        ksPlattformSpec::addMissingSubjectAttributes(currentSubject);
        for(int semester = 0;  semester < m_pSemesterList->nGetObjectCounter(); semester++)
        {
            currentSemester = m_pSemesterList->cGetObjectByIdentifier(semester);
            if(!currentSemester)
            {
                continue;
            }
            
            average = ksPlattformSpec::computeEntireAverageOfSubject(currentSubject,
                    currentSemester->cGetAttributeByName("name")->value(), m_pSemesterList);
            /*
            qDebug("Subject: %s\t semester: %s\t avarage: %f", currentSubject->cGetAttributeByName("name")->value(),
                   currentSemester->cGetAttributeByName("name")->value(), average);
            */
            if(average < 0)
            {
                // go to next semester, if average is invalid
                continue;
            }
            // add widget for this current SemesterAchievement
            if(currentLabelIndex >= lstSemesterAchievements.size())
            {
                currentLabel = new SemesterAchievementWidget;
                lstSemesterAchievements.append(currentLabel);
            }else
            {
                currentLabel = lstSemesterAchievements[currentLabelIndex];
                layoutSemesterAchievements->removeWidget(currentLabel);
            }
            
            
            // configure label
            QString text;
            text += currentSemester->cGetAttributeByName("name")->value();
            text += "\n";
            text += currentSubject->cGetAttributeByName("name")->value();
            text += "\n";
            text += QString::number(average);
            currentLabel->setText(text);
            currentLabel->m_fMark = average;
            ++currentLabelIndex;
        }// end: for each semester
    }// end: for each subject
    
    while(currentLabelIndex < lstSemesterAchievements.size())
    {// delete labels, if there are too much
        currentLabel = lstSemesterAchievements.last();
        if(currentLabel)
        {
            layoutSemesterAchievements->removeWidget(currentLabel);
            delete currentLabel;
        }
        lstSemesterAchievements.removeLast();
    }
    sortSemesterAchievements();
}

void ksSemesterAchievementsDialog::reloadLabelListToLayout()
{
    int colcount = spinColumnCount->value();
    int maxLabelCount = spinMaxLabelCount->value();
    SemesterAchievementWidget* currentLabel = NULL;
    SemesterAchievementWidget* wdgBefore = NULL;
    for(int i = 0; i < lstSemesterAchievements.size(); i++)
    {
        currentLabel = lstSemesterAchievements[i];
        layoutSemesterAchievements->removeWidget(currentLabel);
        layoutSemesterAchievements->addWidget(currentLabel, (int)(i/colcount), i % colcount);
        if((maxLabelCount == 0) || i < maxLabelCount)
        {
            currentLabel->setVisible(TRUE);
            // move into tab(focus) order
            if(wdgBefore)
            {
                QWidget::setTabOrder(wdgBefore, currentLabel);
            }
        }
        else
        {
            currentLabel->setVisible(FALSE);
        }
        wdgBefore = currentLabel;
    }
}

xmlObject* ksSemesterAchievementsDialog::selectedCategory()
{
    return m_pDataPart->cGetObjectByAttributeValue("name", ksPlattformSpec::qstringToSz(cmbCategorySelection->currentText()));
}


void ksSemesterAchievementsDialog::categoryChanged(int index)
{
    refreshSemesterLabelList();
}


void ksSemesterAchievementsDialog::setColumnCount(int columnCount)
{
    spinColumnCount->setValue(columnCount);
}

int ksSemesterAchievementsDialog::columnCount() const
{
    return spinColumnCount->value();
}


void ksSemesterAchievementsDialog::spinboxColumnCountChanged()
{
    reloadLabelListToLayout();
    resize(sizeHint());
}

void ksSemesterAchievementsDialog::sortSemesterAchievements()
{
    
    bool ascending = chkAscendingOrder->isChecked();
    // create a list, containing the pointer to our labels/semester achievement widgets
    QList<SAWPointer> copy;
    for(int i = 0; i < lstSemesterAchievements.size(); i++)
    {
        copy.append(SAWPointer(lstSemesterAchievements.at(i)));
    }
    // sort this new list
    qSort(copy);
    
    // copy items back
    int size = copy.size();
    for(int i = 0; i < size; ++i)
    {
        if(ascending)
        {
            lstSemesterAchievements[i] = copy[i].widget;
        }else{
            lstSemesterAchievements[size-1-i] = copy[i].widget;
        }
    }
    
    reloadLabelListToLayout();
}


void ksSemesterAchievementsDialog::loadUiConfig(ksConfigContainer* config)
{
    m_pCurrentConfig = NULL;
    if(!config)
    {
        return;
    }
    
    chkAscendingOrder->setChecked(config->getOption("semester_achievement_dialog.ascendingorder")->valueToBool());
    spinMaxLabelCount->setValue(config->getOption("semester_achievement_dialog.semestershowcount")->valueToInt());
    spinColumnCount->setValue(config->getOption("semester_achievement_dialog.colcount")->valueToInt());
    if(config->getOption("semester_achievement_dialog.controldockposition")->valueToString() == "right")
    {
        addDockWidget(Qt::RightDockWidgetArea, dockControlSidebar);
    }
    else
    {
        addDockWidget(Qt::LeftDockWidgetArea, dockControlSidebar);
    }
    m_pCurrentConfig = config;
    
}

void ksSemesterAchievementsDialog::saveUiStateToConfig()
{
    if(!m_pCurrentConfig)
    {
        return;
    }
    ksConfigContainer* config = m_pCurrentConfig;
    config->getOption("semester_achievement_dialog.ascendingorder")->setValue(chkAscendingOrder->isChecked());
    config->getOption("semester_achievement_dialog.semestershowcount")->setValue(spinMaxLabelCount->value());
    config->getOption("semester_achievement_dialog.colcount")->setValue(spinColumnCount->value());
    if(dockWidgetArea(dockControlSidebar) == Qt::RightDockWidgetArea)
    {
        config->getOption("semester_achievement_dialog.controldockposition")->setValue(QString("right"));
    }
    else
    {
        config->getOption("semester_achievement_dialog.controldockposition")->setValue(QString("left"));
        
    }
    if(m_pPluginParent)
    {
        m_pPluginParent->configHasChanged();
    }
}


void ksSemesterAchievementsDialog::setPluginParent(ksPlugin* parent)
{
    m_pPluginParent = parent;
}


void ksSemesterAchievementsDialog::closeEvent(QCloseEvent*)
{
    emit visibilityChanged(FALSE);
}

void ksSemesterAchievementsDialog::showEvent(QShowEvent*)
{
    emit visibilityChanged(TRUE);
}

