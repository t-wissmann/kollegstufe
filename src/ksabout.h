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
#ifndef KSABOUT_H
#define KSABOUT_H

class QEvent;
class QPushButton;
class QLabel;
class QFrame;
class QTextEdit;
class QGroupBox;

// layouts
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;



#include <QDialog>

/**
	@author Thorsten Wissmann <towi89@web.de>
*/
class ksAbout : public QDialog
{
Q_OBJECT
public:
    ksAbout(QWidget *parent = 0);

    ~ksAbout();
    
    void retranslateUi();
    
protected:
    virtual void changeEvent(QEvent* event);
private:
    void createGui();
    void initGui();
    //widgets
    QLabel*         lblHeader;
    QLabel*         lblVersion;
    QLabel*         lblIcon;
    QPushButton*    btnClose;
    QGroupBox*      frmContainer;
    QTextEdit*      txtInfoText;
    
    //layouts
    QHBoxLayout*    layoutTop;
    QVBoxLayout*    layoutParent;
    QHBoxLayout*    layoutBottom;
    QHBoxLayout*    layoutFrame;
    

};

#endif
