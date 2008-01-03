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
#include "ksconfigcontainer.h"
#include "xmlparser.h"


ksConfigContainer::ksConfigContainer() : QList<ksConfigOption>()
{
    m_bInGuiOptionPart = FALSE;
}


ksConfigContainer::~ksConfigContainer()
{
}


void ksConfigContainer::saveToXmlObject(xmlObject* target) const
{
    if(target == NULL)
    {
        return;
    }
    // 1. set name
    target->setName("plugin-config");
    
    // resize targets object counter to size of this
    xmlObject* currentObject = NULL;
    target->nSetObjectCounter(size());
    
    for(int i = 0; i < size(); i++)
    {
        currentObject = target->cGetObjectByIdentifier(i);
        (*this)[i].saveToXmlObject(currentObject);
    }
    
}

void ksConfigContainer::loadFromXmlObject(xmlObject* source)
{
    if(source == NULL)
    {
        return;
    }
    
    xmlObject* currentObject = NULL;
    ksConfigOption newOption; // this is the option, how it is in the xml file
    ksConfigOption* pOption; // this is the option that already is in this container, or that has just been created
    for(int i = 0; i < source->nGetObjectCounter(); i++)
    {
        currentObject = source->cGetObjectByIdentifier(i);
        if(!currentObject)
        {
            continue;
        }
        newOption.loadFromXmlObject(currentObject);
        pOption = getOption(newOption, TRUE); // get option, if it doesn't exist, then create it
        ksConfigOption::OptionType type = pOption->valueType();
        pOption->setValue(newOption.valueToString());
        pOption->setValueType(type);// set value type to former type
    }
}



ksConfigOption* ksConfigContainer::getOption(QString name)
{
    ksConfigOption* returnItem = NULL;
    for(int i = 0; i < size(); i++)
    {
        if(name == (*this)[i].name())
        {
            returnItem = &((*this)[i]);
            break;
        }
    }
    
    return returnItem;
}

ksConfigOption* ksConfigContainer::getOption(const ksConfigOption& option, bool createIfNotExists)
{
    ksConfigOption* item = getOption(option.name());
    if(item == NULL && createIfNotExists) // create if there is no item and if createIfNotExists == TRUE
    {
        ksConfigOption newoption = option;
        if(m_bInGuiOptionPart)
        {
            newoption.setChangeableByUser(TRUE);
        }
        append(newoption); // append it
        item = &(last()); // set return-item to just created item's address
    }
    
    return item;
}


ksConfigOption* ksConfigContainer::setOption(const ksConfigOption& option)
{
    ksConfigOption newoption = option;
    if(m_bInGuiOptionPart)
    {
        newoption.setChangeableByUser(TRUE);
    }
    for(int i = 0; i < size(); i++)
    {
        if((*this)[i].name() == newoption.name())
        {
            (*this)[i] = newoption;
            return &((*this)[i]);
        }
    }// if option couldn't be found, then add it and set m_bStandardChangeableByUser
    append(newoption);
    return &(last());
}


ksConfigOption* ksConfigContainer::createOption(const ksConfigOption& option)
{
    int i;
    ksConfigOption* founditem = NULL;
    for(i = 0; i < size(); i++)
    {
        if((*this)[i].name() == option.name())
        {
            founditem = &((*this)[i]);
            break;
        }
    }
    if(!founditem)
    {// if no item has been found, then create it
        append(ksConfigOption(option));
        founditem = &(this->last());
    }
    QString backupValue = founditem->valueToString(); // backup value and type
    ksConfigOption::OptionType backupType = founditem->valueType();
    // copy complete option
    (*founditem) = option;
    // backup actual value and type
    founditem->setValue(backupValue);
    founditem->setValueType(backupType);
    if(m_bInGuiOptionPart)
    {
        founditem->setChangeableByUser(TRUE);
    }
    return founditem;
}


int ksConfigContainer::sizeOfUserChangableOptions()
{
    int changableoptions = 0;
    for(int i = 0 ; i < size(); i++)
    {
        if((*this)[i].changeableByUser())
        {
            changableoptions++;
        }
    }
    return changableoptions;
}



