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
#ifndef KSCATEGORYCHOOSERDIALOG_H
#define KSCATEGORYCHOOSERDIALOG_H

#include <QDialog>

class xmlObject;

// Qt-widgets
class QComboBox;
class QPushButton;
class QLabel;

// layouts
class QHBoxLayout;
class QVBoxLayout;
class QDialogButtonBox;

class QEvent;

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class ksCategoryChooserDialog : public QDialog
{
Q_OBJECT
public:
    ksCategoryChooserDialog(QWidget *parent = 0);

    ~ksCategoryChooserDialog();
    void retranslateUi();
    void reloadIcons();
    
    void setData(xmlObject* datapart, xmlObject* subject);
    QString categorySelected() const;
public slots:
    void applyChanges();
    void checkIfSubjectAlreadyExists();
protected:
    void changeEvent(QEvent* event);
    
private:
    void createGui();
    //widgets
    QLabel*      lblCategory;
    QLabel*      lblSubjectAlreadyExists;
    QComboBox*   cmbCategory;
    // bottom bar
    QPushButton* btnOk;
    QPushButton* btnCancel;
    QDialogButtonBox* boxBottomBox;
    // layouts
    QVBoxLayout* layoutParent;
    QHBoxLayout* layoutCategory;
    
    xmlObject* m_pDataPart;
    xmlObject* m_pCurrentSubject;
    xmlObject* m_pOldCategory;
    QString    m_szAlreadyExistsText;
    QString    m_szSubjectName;
    QString    m_szCategoryLabelText;
    
};

#endif
