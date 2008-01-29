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
#include "../dialogs/ksdebugdialog.h"

#include <io/ksiconcatcher.h>
#include <widgets/ksfilterwidget.h>

// widgets
#include <QPushButton>
#include <QTextEdit>
#include <QDialogButtonBox>


//layouts
#include <QHBoxLayout>
#include <QVBoxLayout>

// other qt classes
#include <QEvent>
#include <QTime>
#include <QKeyEvent>


ksDebugDialog::ksDebugDialog(QWidget *parent)
 : QDialog(parent)
{
    if(parent) // if has parent, then it can be closed automatically on main window close
    {
        setAttribute(Qt::WA_QuitOnClose, FALSE);
    }
    setModal(FALSE);
    
    allocateWidgets();
    createLayouts();
    connectSlots();
    
    retranslateUi();
    reloadIcons();
    
}


ksDebugDialog::~ksDebugDialog()
{
}


void ksDebugDialog::allocateWidgets()
{
    btnClear = new QPushButton;
    btnClose = new QPushButton;
    wdgFind  = new ksFilterWidget;
    btnFindNext = new QPushButton;
    btnFindNext->setDefault(TRUE);
    txtLog   = new QTextEdit;
    txtLog->setReadOnly(TRUE);
}

void ksDebugDialog::createLayouts()
{
    layoutToolBar = new QHBoxLayout;
    layoutToolBar->setMargin(0);
    layoutToolBar->addWidget(btnClear);
    layoutToolBar->addStretch(5);
    layoutToolBar->addWidget(wdgFind);
    layoutToolBar->addWidget(btnFindNext);
    
    bottomButtonBox = new QDialogButtonBox;
    bottomButtonBox->addButton(btnClose, QDialogButtonBox::RejectRole);
    
    layoutParent = new QVBoxLayout;
    layoutParent->addLayout(layoutToolBar);
    layoutParent->addWidget(txtLog);
    layoutParent->addWidget(bottomButtonBox);
    
    
    setLayout(layoutParent);
}

void ksDebugDialog::connectSlots()
{
    connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(wdgFind, SIGNAL(filterChanged(QString)), this, SLOT(find(QString)));
    connect(btnFindNext, SIGNAL(clicked()), this, SLOT(findNext()));
    connect(btnClear, SIGNAL(clicked()), txtLog, SLOT(clear()));
}


void ksDebugDialog::retranslateUi()
{
    btnClose->setText(tr("Close"));
    btnClear->setText(tr("Clear Log"));
    wdgFind->setCaption(tr("Find:"));
    btnFindNext->setText(tr("Next"));
    setWindowTitle(tr("Kollegstufe Debug-Window"));
}

void ksDebugDialog::reloadIcons()
{
    btnClose->setIcon(ksIconCatcher::getIcon("button_cancel", 16));
    btnClear->setIcon(ksIconCatcher::getIcon("editclear", 16));
    btnFindNext->setIcon(ksIconCatcher::getIcon("next", 16));
    setWindowIcon(ksIconCatcher::getIcon("kollegstufe", 16));
}



void ksDebugDialog::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
}

void ksDebugDialog::putDebugMessage(QString msg)
{
    bool isErrorMsg = msg.toLower().startsWith("error");
    bool isWarningMsg = msg.toLower().startsWith("warning");
    
    QString text = "<b>(";
    text += QTime::currentTime().toString();
    text += ")</b> ";
    if(isErrorMsg)
    {
        text += "<i><b>";
    }
    if(isWarningMsg)
    {
        text += "<i>";
    }
    text += msg;
    if(isWarningMsg)
    {
        text += "</i>";
    }
    if(isErrorMsg)
    {
        text += "</b></i>";
    }
    txtLog->append(text);
}

void ksDebugDialog::find(QString phrase)
{
    txtLog->moveCursor(QTextCursor::StartOfBlock);
    if(!txtLog->find(phrase))
    {// begin searching at start position
        txtLog->moveCursor(QTextCursor::Start);
        txtLog->find(phrase);
    }
}

void ksDebugDialog::findNext()
{
    if(!txtLog->find(wdgFind->filter()))
    {// begin searching at start position
        txtLog->moveCursor(QTextCursor::Start);
        txtLog->find(wdgFind->filter());
    }
}


void ksDebugDialog::keyPressEvent(QKeyEvent* event)
{
    if(!event)
    {
        return;
    }
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_F)
    {// CTRL + F is pressed
        wdgFind->focusTextInputField();
    }
    if(event->key() == Qt::Key_F3)
    {// if F3 is pressed, then find next
        findNext();
    }
}



