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
#include "../dialogs/kscategorychooserdialog.h"

#include <io/xmlparser.h>
#include <core/ksplattformspec.h>
#include <io/ksiconcatcher.h>

// Qt- widgets
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

// layouts
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>

// other qt-classes
#include <QEvent>

ksCategoryChooserDialog::ksCategoryChooserDialog(QWidget *parent)
 : QDialog(parent)
{
    m_pCurrentSubject = NULL;
    m_pDataPart       = NULL;
    m_pOldCategory = NULL;
    createGui();
    retranslateUi();
    reloadIcons();
}


ksCategoryChooserDialog::~ksCategoryChooserDialog()
{
}


void ksCategoryChooserDialog::createGui()
{
    // widgets
    lblCategory = new QLabel;
    cmbCategory = new QComboBox;
    lblSubjectAlreadyExists = new QLabel;
    
    boxBottomBox = new QDialogButtonBox;
    btnOk        = new QPushButton;
    btnCancel    = new QPushButton;
    boxBottomBox->addButton(btnOk, QDialogButtonBox::AcceptRole);
    boxBottomBox->addButton(btnCancel, QDialogButtonBox::RejectRole);
    
    // connect slots
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(this, SIGNAL(accepted()), this, SLOT(applyChanges()));
    connect(cmbCategory, SIGNAL(currentIndexChanged(int)), this, SLOT(checkIfSubjectAlreadyExists()));
    
    // layouts
    layoutCategory = new QHBoxLayout;
    layoutCategory->addWidget(lblCategory);
    layoutCategory->addWidget(cmbCategory);
    
    layoutParent = new QVBoxLayout;
    layoutParent->addLayout(layoutCategory);
    layoutParent->addWidget(lblSubjectAlreadyExists);
    lblSubjectAlreadyExists->setVisible(FALSE);
    layoutParent->addWidget(boxBottomBox);
    
    setLayout(layoutParent);
}

void ksCategoryChooserDialog::retranslateUi()
{
    setWindowTitle(tr("Move Subject to a new Category"));
    btnOk->setText(tr("OK"));
    btnCancel->setText(tr("Cancel"));
    m_szAlreadyExistsText = tr("The Subject %subjectname already exists in this category!");
    m_szCategoryLabelText = tr("Select a category for the subject %subjectname:");
    lblSubjectAlreadyExists->setText(m_szAlreadyExistsText);
    lblCategory->setText(QString(m_szCategoryLabelText).replace("%subjectname", m_szSubjectName));
}


void ksCategoryChooserDialog::reloadIcons()
{
    btnOk->setIcon(ksIconCatcher::getIcon("button_ok", 16));
    btnCancel->setIcon(ksIconCatcher::getIcon("button_cancel", 16));
}

void ksCategoryChooserDialog::changeEvent(QEvent* event)
{
    
    QWidget::changeEvent(event);
    if(event && (event->type() == QEvent::LanguageChange))
    {
        retranslateUi();
    }
}


void ksCategoryChooserDialog::setData(xmlObject* datapart, xmlObject* subject)
{
    cmbCategory->clear();
    m_pCurrentSubject = subject;
    m_pDataPart = datapart;
    m_pOldCategory = NULL;
    if(!subject || !datapart)
    {
        return;
    }
    ksPlattformSpec::addMissingSubjectAttributes(subject);
    xmlObject* currentCategory = NULL;
    int catIndexOfSubject = -1;
    for(int i = 0; i < datapart->nGetObjectCounter(); i++)
    {
        currentCategory = datapart->cGetObjectByIdentifier(i);
        if(!currentCategory)
        {
            continue;
        }
        ksPlattformSpec::addMissingCathegoryAttributes(currentCategory);
        QString newItem = currentCategory->cGetAttributeByName("name")->value();
        if(catIndexOfSubject == -1 ) // if subject wasn't found yet
        {
            // then try this category
            int subjectindex = currentCategory->nGetIdentifierOf(subject);
            if(subjectindex != -1)
            { // if current category contains the  subject
                catIndexOfSubject = i;
                m_pOldCategory = currentCategory;
            }
        }
        cmbCategory->addItem(newItem);
    }
    if(catIndexOfSubject > -1)
    {
        cmbCategory->setCurrentIndex(catIndexOfSubject);
    }
    // refres error label:
    m_szSubjectName = ksPlattformSpec::szToUmlauts(subject->cGetAttributeByName("name")->value());
    lblSubjectAlreadyExists->setText(QString(m_szAlreadyExistsText).replace("%subjectname", m_szSubjectName));
    lblCategory->setText(QString(m_szCategoryLabelText).replace("%subjectname", m_szSubjectName));
}

void ksCategoryChooserDialog::applyChanges() // i.e. move subject to new category
{
    if(!m_pDataPart || !m_pCurrentSubject || !m_pOldCategory)
    {
        qDebug("ksCategoryChooserDialog::applyChanges() Internal Pointer Error");
        return;
    }
    xmlObject* newCategory = m_pDataPart->cGetObjectByAttributeValue("name", ksPlattformSpec::qstringToSz(cmbCategory->currentText()));
    if(newCategory == m_pOldCategory)
    {
        // there is nothing to do
        return;
    }
    // remove subject from old category
    m_pOldCategory->cTakeObjectFromList(m_pCurrentSubject);
    // and add it to new category
    newCategory->cAddExistingObject(m_pCurrentSubject);
    
}


void ksCategoryChooserDialog::checkIfSubjectAlreadyExists()
{
    if(!m_pDataPart || !m_pCurrentSubject)
    {
        return;
    }
    xmlObject* currentCategory = m_pDataPart->cGetObjectByAttributeValue("name",
            ksPlattformSpec::qstringToSz(cmbCategory->currentText()));
    if(!currentCategory)
    {
        return;
    }
    // is there a other other subject with the same name in the new category ?
    xmlObject* foundSubject = currentCategory->cGetObjectByAttributeValue("name", m_pCurrentSubject->cGetAttributeByName("name")->value());
    bool alreadyExists = (foundSubject && m_pCurrentSubject != foundSubject);
    //qDebug("alreadyExists = %d", alreadyExists);
    lblSubjectAlreadyExists->setVisible(alreadyExists);
    btnOk->setEnabled(!alreadyExists);
}

QString ksCategoryChooserDialog::categorySelected() const
{
    return cmbCategory->currentText();
}

