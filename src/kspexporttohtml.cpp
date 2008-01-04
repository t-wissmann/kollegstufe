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
#include "kspexporttohtml.h"
#include "ksplugininformation.h"
#include "configdialog.h"
#include <QAction>

kspExportToHtml::kspExportToHtml()
 : ksPlugin()
{
    setIdentifier("kspExportToHtml");
    setIsHavingAnAboutDialog(TRUE);
    setIsConfigurable(TRUE);
    retranslateUi();
}


kspExportToHtml::~kspExportToHtml()
{
}


void kspExportToHtml::retranslate()
{
    setName(tr("To Html Exporter"));
    setAuthor("Thorsten Wissmann");
    setDescription(tr("Exports database to an HTML-File"));
}


void kspExportToHtml::load()
{
    qDebug("%s:%s gets loaded", identifier().toAscii().data(), name().toAscii().data());
    mnaExportToHtml = new QAction(this);
    mnaExportToHtml->setText(tr("Export To Html"));
    addMenuAction(mnaExportToHtml);
    connect(mnaExportToHtml, SIGNAL(triggered()), this, SLOT(exportToHtml()));
}

void kspExportToHtml::refresh()
{
    
}

void kspExportToHtml::unload()
{
    qDebug("%s:%s gets unloaded", identifier().toAscii().data(), name().toAscii().data());
    
    disconnect(mnaExportToHtml, SIGNAL(triggered()), this, SLOT(exportToHtml()));
    removeMenuAction(mnaExportToHtml);
    delete mnaExportToHtml;
}

void kspExportToHtml::createConfiguration(ksConfigContainer* config)
{
    if(!config)
    {
        return;
    }
    
    /*config->setInGuiOptionPart(TRUE); // start of gui-options
    ksConfigOption* option;
    option = config->createOption(ksConfigOption("desktop size", 4));
    option->setDescription("Size of your Desktop");
    option->setMinMax(1, 10);
    option = config->createOption(ksConfigOption("name", QString("thorsten")));
    option->setDescription("Your name");
    option = config->createOption(ksConfigOption("size", 1.70));
    option->setDescription("Your size");
    option->setMinMax(0.40, 3.00);
    
    config->setInGuiOptionPart(FALSE); // end of gui - options*/
}

void kspExportToHtml::exportToHtml()
{
}


