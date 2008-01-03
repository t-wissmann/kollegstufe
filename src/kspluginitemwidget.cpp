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
#include "kspluginitemwidget.h"

#include "ksplugin.h"
#include "qclickablelabel.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>
#include <QEvent>

#include <QGradient>
#include <QPalette>
#include <QPainter>
#include <QColor>
#include <QMouseEvent>
#include <QPen>


ksPluginItemWidget::ksPluginItemWidget(QWidget *parent)
 : QWidget(parent)
{
    m_pCurrentPlugin = NULL;
    m_bIsSelected = FALSE;
    allocateWidgets();
    createLayouts();
    connectSlots();
    retranslateUi();
}


ksPluginItemWidget::~ksPluginItemWidget()
{
}


void ksPluginItemWidget::allocateWidgets()
{
    chkIsLoaded = new QCheckBox;
    chkIsLoaded->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    lblNameAndDescription = new QClickableLabel;
    lblNameAndDescription->setWordWrap(TRUE);
    btnAbout    = new QPushButton;
    btnAbout->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnConfigure= new QPushButton;
    btnConfigure->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    cmbScope    = new QComboBox;
    cmbScope->addItem("Global");
    cmbScope->addItem("Local");
    cmbScope->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}


void ksPluginItemWidget::createLayouts()
{
    layoutRightEnd = new QVBoxLayout;
    layoutRightEnd->setMargin(0);
    layoutRightEnd->addWidget(btnAbout);
    layoutRightEnd->addWidget(btnConfigure);
    
    layoutParent = new QHBoxLayout;
    layoutParent->addWidget(chkIsLoaded);
    layoutParent->addWidget(cmbScope);
    layoutParent->addWidget(lblNameAndDescription);
    layoutParent->addLayout(layoutRightEnd);
    
    
    
    setLayout(layoutParent);
}

void ksPluginItemWidget::connectSlots()
{
    // about selection
    connect(lblNameAndDescription, SIGNAL(clicked()), this, SLOT(setSelected()));
    connect(this, SIGNAL(clicked()), this, SLOT(setSelected()));
    connect(btnAbout, SIGNAL(clicked()), this, SLOT(setSelected()));
    connect(btnConfigure, SIGNAL(clicked()), this, SLOT(setSelected()));
    connect(chkIsLoaded, SIGNAL(toggled(bool)), this, SLOT(setSelected()));
    connect(chkIsLoaded, SIGNAL(toggled(bool)), cmbScope, SLOT(setEnabled(bool)));
    connect(cmbScope, SIGNAL(highlighted(int)), this, SLOT(setSelected()));
    
    connect(chkIsLoaded, SIGNAL(toggled(bool)), this, SLOT(setPluginStateFromGui()));
    connect(cmbScope, SIGNAL(currentIndexChanged(int)), this, SLOT(setPluginStateFromGui()));
    // about actions
    connect(btnAbout, SIGNAL(clicked()), this, SLOT(showAboutDialog()));
    connect(btnConfigure, SIGNAL(clicked()), this, SLOT(showConfigurationDialog()));
    
}

void ksPluginItemWidget::retranslateUi()
{
    
    if(m_pCurrentPlugin == NULL)
    {
        setEnabled(FALSE);
    }
    else
    {
        m_pCurrentPlugin->retranslateUi(); // To be sure, that is already translated
        setEnabled(TRUE);
        QString text = "<b>";
        text += m_pCurrentPlugin->name() + "</b><br>";
        text += m_pCurrentPlugin->description();
        lblNameAndDescription->setText(text);
        btnAbout->setVisible(m_pCurrentPlugin->hasAboutDialog());
        btnConfigure->setEnabled(m_pCurrentPlugin->isConfigurable());
    }
    btnAbout->setText(tr("About"));
    btnConfigure->setText(tr("Configure"));
    cmbScope->setItemText(0, tr("Global"));
    cmbScope->setItemText(1, tr("Local"));
    cmbScope->setWhatsThis(tr("The scope of the plugin can be configured here:")
            + "<li><b>" + tr("Global") + "</b> " + tr("means, that this plugin will be loaded for all archives")
            + "</li><li><b>" + tr("Local") + "</b> " + tr("means, that this plugin only will be loaded when opening this current archive")
            + "</li><br> ");
    chkIsLoaded->setWhatsThis(tr("Here you can load/unload this plugin."));
    btnConfigure->setWhatsThis(tr("Click to configure this plugin."));
    btnAbout->setWhatsThis(tr("Click to get some information about this plugin."));
}

void ksPluginItemWidget::showConfigurationDialog()
{
    if(m_pCurrentPlugin != NULL)
    {
        m_pCurrentPlugin->configure();
    }
}


void ksPluginItemWidget::showAboutDialog()
{
    if(m_pCurrentPlugin != NULL)
    {
        m_pCurrentPlugin->about();
    }
}

void ksPluginItemWidget::setPluginStateFromGui()
{
    if(m_pCurrentPlugin != NULL)
    {
        if(chkIsLoaded->isChecked())
        {
            if(cmbScope->currentIndex() == 0)
            {
                m_pCurrentPlugin->setWantedState(ksPlugin::StateGlobalLoaded);
            }
            else
            {
                m_pCurrentPlugin->setWantedState(ksPlugin::StateLocalLoaded);
            }
        }
        else
        {
            m_pCurrentPlugin->setWantedState(ksPlugin::StateUnloaded);
        }
    }
}

void ksPluginItemWidget::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
}

void ksPluginItemWidget::mousePressEvent(QMouseEvent * event)
{
    if(event && event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
}

void ksPluginItemWidget::setSelected()
{
    setSelected(TRUE);
}

void ksPluginItemWidget::paintEvent ( QPaintEvent * event )
{
    if(m_bIsSelected)
    {
        QLinearGradient gradient(0, 0, 0, height());
        QColor main = palette().highlight().color();
        QColor c1 = main;
        QColor c2 = main;
        c1.setHsv(c1.hue(), c1.saturation(), c1.value()+30, main.alpha());
        c2.setHsv(c2.hue(), c2.saturation(), c2.value()-30, main.alpha());
        gradient.setColorAt(0, c1);
        gradient.setColorAt(1, c2);
        QBrush brush(gradient);
        QPen   pen;
        pen.setColor(palette().highlight().color());
        
        QPainter painter(this);
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.drawRect(0, 0, width()-1, height()-1);
    }
}


void ksPluginItemWidget::setSelected(bool ifIsSelected)
{
    // if nothing has been changed, we don't need to do anything
    if(m_bIsSelected == ifIsSelected)
    {
        return;
    }
    m_bIsSelected = ifIsSelected;
    if(ifIsSelected)
    {
        emit(selected(this));
        lblNameAndDescription->setForegroundRole(QPalette::HighlightedText);
    }
    else
    {
        lblNameAndDescription->setForegroundRole(QPalette::WindowText);
    }
    update();
}

void ksPluginItemWidget::setPlugin(ksPlugin*    newPlugin)
{
    m_pCurrentPlugin = newPlugin;
    if(m_pCurrentPlugin != NULL)
    {
        chkIsLoaded->setChecked(m_pCurrentPlugin->isLoaded());
        cmbScope->setEnabled(chkIsLoaded->isChecked());
    }
    retranslateUi();
    
}




