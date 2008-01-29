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
#include "../widgets/ksfilterwidget.h"
#include <io/ksiconcatcher.h>

#include <QEvent>
#include <QLabel>
#include <QToolButton>
#include <QLineEdit>

#include <QHBoxLayout>

#include <QKeyEvent>

ksFilterWidget::ksFilterWidget(QWidget *parent)
 : QWidget(parent)
{
    m_bIsCustomLabel = FALSE;
    createGui();
    retranslateUi();
    reloadIcons();
    setCaption("");
}


ksFilterWidget::~ksFilterWidget()
{
}

void ksFilterWidget::createGui()
{
    // allocate widgets
    lblCaption = new QLabel;
    txtFilter = new QLineEdit;
    btnClear = new QToolButton;
    
    // createLayouts
    layoutParent = new QHBoxLayout;
    layoutParent->setMargin(0);
    layoutParent->addWidget(lblCaption);
    layoutParent->addWidget(txtFilter);
    layoutParent->addWidget(btnClear);
    
    setLayout(layoutParent);
    // connect slots
    connect(txtFilter, SIGNAL(textChanged(QString)), this, SLOT(filterTextChanged()));
    connect(btnClear, SIGNAL(clicked()), this, SLOT(clear()));
    
}


void ksFilterWidget::changeEvent(QEvent* event)
{
    if(!event)
    {
        return;
    }
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
    
}

void ksFilterWidget::retranslateUi()
{
    m_szDefaultCaption = tr("Filter:");
    if(!m_bIsCustomLabel)
    {
        lblCaption->setText(m_szDefaultCaption);
    }
    btnClear->setToolTip(tr("Clear Filter Field"));
}

void ksFilterWidget::reloadIcons()
{
    btnClear->setIcon(ksIconCatcher::getIcon("editclear", 16));
}


void ksFilterWidget::filterTextChanged()
{
    emit filterChanged(txtFilter->text());
}

void ksFilterWidget::clear()
{
    txtFilter->clear();
}



void ksFilterWidget::setCaption(QString caption) // commit "" for default caption text
{
    if(caption.isEmpty())
    {// if default caption is wanted
        lblCaption->setText(m_szDefaultCaption);
        m_bIsCustomLabel = FALSE;
    }
    else
    {
        lblCaption->setText(caption);
        m_bIsCustomLabel = TRUE;
    }
}

QString ksFilterWidget::caption() const
{
    return lblCaption->text();
}

void ksFilterWidget::setCaptionVisible(bool visible)
{
    lblCaption->setVisible(visible);
}

bool ksFilterWidget::isLabelVisible() const
{
    return lblCaption->isVisible();
}

QString ksFilterWidget::filter() const
{
    return txtFilter->text();
}

void ksFilterWidget::setFilter(QString filter)
{
    if(filter != txtFilter->text())
    {
        txtFilter->setText(filter);
    }
}


void ksFilterWidget::focusTextInputField()
{
    txtFilter->setFocus();
}



void ksFilterWidget::keyPressEvent(QKeyEvent* event)
{
    if(!event)
    {
        return;
    }
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        emit returnPressed();
    }
}


