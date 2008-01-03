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
#include "ksconfigoption.h"
#include "xmlparser.h"
#include "ksplattformspec.h"

ksConfigOption::ksConfigOption()
{
    m_nValueType = TypeString;
    m_bChangeableByUser = FALSE;
    m_fMinimum = 0;
    m_fMaximum = 99;
}

ksConfigOption::~ksConfigOption()
{
}

ksConfigOption::ksConfigOption(const ksConfigOption& other)
{
    setValue(other.valueToString());
    m_nValueType = other.valueType();
    m_szName = other.name();
    m_bChangeableByUser = other.changeableByUser();
    setMinMax(other.minimum(), other.maximum());
    m_szDescription = other.description();
    m_szWhatsThis = other.whatsThis();
    m_szDefaultValue = other.defaultValue();
}

void ksConfigOption::operator=(const ksConfigOption& other)
{
    setValue(other.valueToString());
    m_nValueType = other.valueType();
    m_szName = other.name();
    m_bChangeableByUser = other.changeableByUser();
    setMinMax(other.minimum(), other.maximum());
    m_szDescription = other.description();
    m_szWhatsThis = other.whatsThis();
    m_szDefaultValue = other.defaultValue();
}



ksConfigOption::ksConfigOption(QString newName, bool    newValue)
{
    m_szName = newName;
    setValue(newValue);
    m_bChangeableByUser = FALSE;
    m_fMinimum = 0;
    m_fMaximum = 99;
    m_szDefaultValue = newValue ? "true" : "false";
}

ksConfigOption::ksConfigOption(QString newName, int     newValue)
{
    m_szName = newName;
    setValue(newValue);
    m_bChangeableByUser = FALSE;
    m_fMinimum = 0;
    m_fMaximum = 99;
    if(newValue > m_fMaximum)
    {// adjust min and max
        m_fMaximum = newValue;
    }
    if(newValue < m_fMinimum)
    {
        m_fMinimum = newValue;
    }
    m_szDefaultValue = QString::number(newValue);
}

ksConfigOption::ksConfigOption(QString newName, double  newValue)
{
    m_szName = newName;
    setValue(newValue);
    m_bChangeableByUser = FALSE;
    m_fMinimum = 0;
    m_fMaximum = 99;
    if(newValue > m_fMaximum)
    {// adjust min and max
        m_fMaximum = newValue;
    }
    if(newValue < m_fMinimum)
    {
        m_fMinimum = newValue;
    }
    m_szDefaultValue = QString::number(newValue);
}

ksConfigOption::ksConfigOption(QString newName, QString newValue)
{
    m_szName = newName;
    setValue(newValue);
    m_bChangeableByUser = FALSE;
    m_fMinimum = 0;
    m_fMaximum = 99;
    m_szDefaultValue = newValue;
}

bool ksConfigOption::operator==(const ksConfigOption& other) const
{
    switch(m_nValueType){
        // we need conversion, when type is int or double, because "10.0" == "10.000" would return FALSE, although 10.0 is equal to 10.000
        case TypeInt: return valueToInt() == other.valueToInt();
        case TypeDouble: return valueToDouble() == other.valueToDouble();
        
        default: return m_szValue == other.valueToString();
    }
}



QString ksConfigOption::name() const
{
    return m_szName;
}

void ksConfigOption::setName(QString newName)
{
    m_szName = newName;
}

void ksConfigOption::setValueType(ksConfigOption::OptionType newType)
{
    m_nValueType = newType;
}

void ksConfigOption::setValueType(QString    newType)
{
    m_nValueType = stringToOptionType(newType);
}

ksConfigOption::OptionType ksConfigOption::stringToOptionType(QString type)
{
    for(OptionType currentType = TypeBool; currentType <= TypeString; currentType = (OptionType)(((int)currentType) + 1))
    {
        if(type == valueTypeToString(currentType))
        {
            return currentType;
        }
    }
    return TypeString;
}

ksConfigOption::OptionType ksConfigOption::valueType() const
{
    return m_nValueType;
}

QString ksConfigOption::valueTypeToString(ksConfigOption::OptionType type)
{
    
    switch(type)
    {
        case TypeBool: return "bool";
        case TypeInt: return "int";
        case TypeDouble: return "double";
        default: return "string";
    }
}

QString ksConfigOption::valueTypeToString() const
{
    return valueTypeToString(m_nValueType);
}


void ksConfigOption::saveToXmlObject(xmlObject* target) const
{
    if(target == NULL)
    {
        return;
    }
    
    target->setName("option");
    target->nSetAttributeCounter(0); // delete all previous attributes
    target->nAddAttribute("type", ksPlattformSpec::qstringToSz(valueTypeToString()));
    target->nAddAttribute("name", ksPlattformSpec::qstringToSz(m_szName));
    target->nAddAttribute("value", ksPlattformSpec::qstringToSz(m_szValue));
    
}



void ksConfigOption::loadFromXmlObject(xmlObject* source)
{
    if(source == NULL)
    {
        return;
    }
    // 1. set name
    m_szName  = source->cGetAttributeByName("name") ? ksPlattformSpec::szToUmlauts(source->cGetAttributeByName("name")->value()) : "";
    // 2. set Value
    m_szValue = source->cGetAttributeByName("value") ? ksPlattformSpec::szToUmlauts(source->cGetAttributeByName("value")->value()) : "";
    // 3. set type
    setValueType(source->cGetAttributeByName("type") ? source->cGetAttributeByName("type")->value() : "string");
}
    

bool    ksConfigOption::valueToBool() const
{
    return valueToBool(m_szValue);
}


int     ksConfigOption::valueToInt() const
{
    return m_szValue.toInt();
}

double  ksConfigOption::valueToDouble() const
{
    return m_szValue.toDouble();
}

QString ksConfigOption::valueToString() const
{
    return m_szValue;
}


bool ksConfigOption::valueToBool(QString string)
{
    if(string == "true")
    {
        return TRUE;
    }
    if(string ==  "false")
    {
        return FALSE;
    }
    // if m_szValue is neither true nor false, then return false
    return FALSE;
}

int ksConfigOption::valueToInt(QString string)
{
    return string.toInt();
}

double ksConfigOption::valueToDouble(QString string)
{
    return string.toDouble();
}

QString ksConfigOption::valueToString(QString string)
{
    return string;
}


void ksConfigOption::setValue(bool    newValue)
{
    m_nValueType = TypeBool;
    if(newValue)
    {
        m_szValue = "true";
    }else
    {
        m_szValue = "false";
    }
}

void ksConfigOption::setValue(int     newValue)
{
    m_nValueType = TypeInt;
    m_szValue = QString::number(newValue);
}

void ksConfigOption::setValue(double  newValue)
{
    m_nValueType = TypeDouble;
    m_szValue = QString::number(newValue);
}

void ksConfigOption::setValue(QString newValue)
{
    m_nValueType = TypeString;
    m_szValue = newValue;
}

    
    
void ksConfigOption::setChangeableByUser(bool changeable)
{
    m_bChangeableByUser = changeable;
}

bool ksConfigOption::changeableByUser() const
{
    return m_bChangeableByUser;
}






