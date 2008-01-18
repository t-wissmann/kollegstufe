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

#include "kspwidgettester.h"

#include "ksplugininformation.h"
#include <QAction>
#include <QString>

#include <QDialog>
#include "selectablelabel.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

kspWidgetTester::kspWidgetTester()
    :ksPlugin()
{
    testDialog = NULL;
    setIdentifier("kspWidgetTester");
    setIsHavingAnAboutDialog(FALSE);
    setIsConfigurable(FALSE);
    retranslateUi();
}


kspWidgetTester::~kspWidgetTester()
{
}


void kspWidgetTester::retranslate()
{
    setName(tr("Widget Tester"));
    setAuthor("Thorsten Wissmann");
    setDescription(tr("Just tests some custom widgets of kollegstufe"));
}



void kspWidgetTester::load()
{
    qDebug("%s:%s gets loaded", identifier().toAscii().data(), name().toAscii().data());
    mnaTestWidgets = new QAction(this);
    mnaTestWidgets->setText(tr("Test some widges"));
    addMenuAction(mnaTestWidgets);
    connect(mnaTestWidgets, SIGNAL(triggered()), this, SLOT(testWidgets()));
}

void kspWidgetTester::refresh()
{
    
}

void kspWidgetTester::unload()
{
    qDebug("%s:%s gets unloaded", identifier().toAscii().data(), name().toAscii().data());
    
    disconnect(mnaTestWidgets, SIGNAL(triggered()), this, SLOT(testWidgets()));
    removeMenuAction(mnaTestWidgets);
    delete mnaTestWidgets;
}

void kspWidgetTester::createConfiguration(ksConfigContainer* config)
{
}

void kspWidgetTester::testWidgets()
{
    if(!m_pInformation->mainWindow())
    {
        return;
    }
    if(!testDialog)
    {
        testDialog = new QDialog((QWidget*)m_pInformation->mainWindow());
        testDialog->setAttribute(Qt::WA_QuitOnClose, FALSE);
        testDialog->setWindowTitle("Test some ks widgets");
        QHBoxLayout* parentLayout = new QHBoxLayout;
        SelectableLabel* label = new SelectableLabel;
        label->setText("Just a test text\nBlablabla");
        parentLayout->addWidget(label);
        label = new SelectableLabel;
        label->setText("Just another text\nBlablabla");
        parentLayout->addWidget(label);
        label = new SelectableLabel;
        label->setText("Widget Three");
        parentLayout->addWidget(label);
        label = new SelectableLabel;
        label->setText("Widqura Quadro");
        parentLayout->addWidget(label);
        testDialog->setLayout(parentLayout);
        
        
    }
    testDialog->show();
}


