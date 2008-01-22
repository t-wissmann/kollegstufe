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
#include <io/xmlparser.h>
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

/** start VALUE specific constructors */

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

ksConfigOption::ksConfigOption(QString newName, QColor newValue)
{
    m_szName = newName;
    setValue(newValue);
    m_bChangeableByUser = FALSE;
    m_fMinimum = 0;
    m_fMaximum = 99;
    m_szDefaultValue = m_szValue;
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

/** END VALUE specific constructors */

bool ksConfigOption::operator==(const ksConfigOption& other) const
{
    if(m_nValueType != other.valueType())
    {
        // if there are different types, they can't be equal
        return FALSE;
    }
    switch(m_nValueType){
        // we need conversion, when type is int or double, because "10.0" == "10.000" would return FALSE, although 10.0 is equal to 10.000
        case TypeInt: return valueToInt() == other.valueToInt();
        case TypeDouble: return valueToDouble() == other.valueToDouble();
        case TypeColor: return valueToColor() == other.valueToColor(); // red == #ff0000 !!
        
        default: return m_szValue == other.valueToString();
    }
}


void ksConfigOption::rangeValue() // only makes sense on numeric datatypes
{
    if(m_nValueType == TypeDouble)
    {
       double value = m_szValue.toDouble();
       if(value < m_fMinimum)
       {
           value = m_fMinimum;
       }
       if(value > m_fMaximum)
       {
           value = m_fMaximum;
       }
       m_szValue = QString::number(value);
       return;
    }
    if(m_nValueType == TypeInt)
    {
        int value = m_szValue.toInt();
        if(value < m_fMinimum)
        {
            value = (int) m_fMinimum;
        }
        if(value > m_fMaximum)
        {
            value = (int) m_fMaximum;
        }
        m_szValue = QString::number(value);
        return;
    }
    if(m_nValueType == TypeBool)
    {
        if(m_szValue != "true" && m_szValue != "false")
        {
            if(m_szValue == "FALSE")
            {
                m_szValue = "false";
            }
            else if(m_szValue == "TRUE")
            {
                m_szValue = "true";
            }
            else if(m_szValue == "1")
            {
                m_szValue = "true";
            }
            else if(m_szValue == "0")
            {
                m_szValue = "false";
            }
            else
            {// set to default
                m_szValue = m_szDefaultValue;
            }
        }
        return;
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


/** START: about value type */
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
        case TypeColor: return "color";
        default: return "string";
    }
}

QString ksConfigOption::valueTypeToString() const
{
    return valueTypeToString(m_nValueType);
}

/** END: about value type */

/** START: xmlObject Functions*/

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
    
/** END: xmlObject Functions*/
/** START: value to type Functions*/

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


QColor  ksConfigOption::valueToColor() const
{
    return QColor(m_szValue);
}

QString ksConfigOption::valueToString() const
{
    return m_szValue;
}

/** END: value to type Functions*/
/** START: static string to type Functions*/

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

QColor  ksConfigOption::valueToColor(QString string)
{
    return QColor(string);
}

QString ksConfigOption::valueToString(QString string)
{
    return string;
}


/** END: static string to type Functions*/
/** START: value setter */

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

void ksConfigOption::setValue(QColor newValue)
{
    m_nValueType = TypeColor;
    m_szValue = newValue.name();
}

void ksConfigOption::setValue(QString newValue)
{
    m_nValueType = TypeString;
    m_szValue = newValue;
}


/** END: value setter */
    
    
void ksConfigOption::setChangeableByUser(bool changeable)
{
    m_bChangeableByUser = changeable;
}

bool ksConfigOption::changeableByUser() const
{
    return m_bChangeableByUser;
}






