/***************************************************************************
 *   Copyright (C) 2007 by Thorsten W.	                                   *
 *   towi89@web.de towi16.piranho.de                                       *
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
#include "ksdatabaseproperties.h"
#include "xmlparser.h"

#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>

// layouts
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>


ksDatabaseProperties::ksDatabaseProperties(QWidget *parent)
 : QDialog(parent)
{
    allocateWidgets();
    createLayouts();
    connectSlots();
}


ksDatabaseProperties::~ksDatabaseProperties()
{
}

void ksDatabaseProperties::allocateWidgets()
{
    btnOk     = new QPushButton("Ok");
    btnCancel = new QPushButton("Abbrechen");
}

void ksDatabaseProperties::createLayouts()
{
    layoutBottom = new QHBoxLayout;
    layoutBottom->addStretch(1);
    layoutBottom->addWidget(btnOk);
    layoutBottom->addWidget(btnCancel);
    
    layoutParent = new QGridLayout;
    layoutParent->addLayout(layoutBottom, 0, 0);
    
    setLayout(layoutParent);
}

void ksDatabaseProperties::connectSlots()
{
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(this, SIGNAL(accepted()), this, SLOT(writeWidgetAttributesToProperties()));
}

void ksDatabaseProperties::setDatabasePropertiesToEdit(xmlObject* newProperties)
{
    propertiesToEdit = newProperties;
    if(propertiesToEdit == NULL)
    {
        return;
    }
}

void ksDatabaseProperties::writeWidgetAttributesToProperties()
{
    
}


