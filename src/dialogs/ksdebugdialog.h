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
#ifndef KSDEBUGDIALOG_H
#define KSDEBUGDIALOG_H

#include <QDialog>

class QHBoxLayout;
class QVBoxLayout;

class ksFilterWidget;
class QTextEdit;
class QPushButton;
class QDialogButtonBox;

class QEvent;
class QKeyEvent;

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class ksDebugDialog : public QDialog
{
Q_OBJECT
public:
    ksDebugDialog(QWidget *parent = 0);

    ~ksDebugDialog();
    
    void retranslateUi();
    void reloadIcons();
public slots:
    void putDebugMessage(QString msg);
    void find(QString phrase);
    void findNext();
protected:
    virtual void changeEvent(QEvent* event);
    virtual void keyPressEvent(QKeyEvent* event);
private:
    
    void allocateWidgets();
    void createLayouts();
    void connectSlots();
    
    // widgets
    QPushButton*    btnClose;
    QDialogButtonBox* bottomButtonBox;
    ksFilterWidget* wdgFind;
    QPushButton*    btnFindNext;
    QPushButton*    btnClear;
    QTextEdit*      txtLog;
    
    // layouts
    QHBoxLayout*    layoutToolBar;
    QVBoxLayout*    layoutParent;
    
    
    

};

#endif
