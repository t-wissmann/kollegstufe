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
#ifndef KSDATABASESELECTION_H
#define KSDATABASESELECTION_H

#include <QDialog>
#include <QString>
#include <QStringList>

class QListWidget;
class QLabel;
class QPushButton;


//layouts

class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
/**
	@author Thorsten Wissmann <towi89@web.de>
*/
class ksDatabaseSelection : public QDialog
{
Q_OBJECT
public:
    ksDatabaseSelection(QWidget *parent = 0);

    ~ksDatabaseSelection();
    QString getCurrentFile();
    void    setCurrentFile(QString szNewFile);
    void    catchStringList();
public slots:
    void    refreshFileList();
    void    selectedFileChanged();
private:
    
    void    allocateWidgets();
    void    createLayouts();
    void    connectSlots();
    
    // widgets
    QPushButton*    btnOk;
    QPushButton*    btnCancel;
    QListWidget*    lstFileList;
    QLabel*         lblInfo;
    
    //layouts:
    QHBoxLayout*    layoutBottom;
    QGridLayout*    layoutParent;
    
    //members
    QStringList szFileList;
    QString     szCurrentFile;
};

#endif
