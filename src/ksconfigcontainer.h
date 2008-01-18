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
#ifndef KSCONFIGCONTAINER_H
#define KSCONFIGCONTAINER_H

#include "ksconfigoption.h"

#include <QList>

class xmlObject;

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/
class ksConfigContainer : public QList<ksConfigOption*>
{
    
public:
    ksConfigContainer();

    ~ksConfigContainer();
    
    int sizeOfUserChangableOptions();
    void saveToXmlObject(xmlObject* target) const;
    void loadFromXmlObject(xmlObject* source);
    
    bool inGuiOptionPart() const { return m_bInGuiOptionPart; };
    void setInGuiOptionPart(bool inGui) { m_bInGuiOptionPart = inGui; m_nCurrentGuiOptionCounter = 0; };
    
    ksConfigOption* getOption(QString name);
    ksConfigOption* getOption(const ksConfigOption& option, bool createIfNotExists = TRUE);
    ksConfigOption* setOption(const ksConfigOption& option);
    ksConfigOption* createOption(const ksConfigOption& option);
    
private:
    bool m_bInGuiOptionPart;
    unsigned int  m_nCurrentGuiOptionCounter;
    
};

#endif
