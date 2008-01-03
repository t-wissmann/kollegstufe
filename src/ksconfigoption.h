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
#ifndef KSCONFIGOPTION_H
#define KSCONFIGOPTION_H

/**
	@author Thorsten Wissmann <kollegstufe@thorsten-wissmann.de>
*/

#include <QString>
class xmlObject;


class ksConfigOption{
public:
    ksConfigOption();

    ksConfigOption(const ksConfigOption& other);
    ksConfigOption(QString newName, bool    newValue);
    ksConfigOption(QString newName, int     newValue);
    ksConfigOption(QString newName, double  newValue);
    ksConfigOption(QString newName, QString newValue);

    ~ksConfigOption();
    enum OptionType
    {
        TypeBool, // when type is bool, then m_szValue is "true" or "false", not "TRUE" or "FALSE" !
        TypeInt,
        TypeDouble,
        TypeString
    };
    
    QString name() const;
    void setName(QString newName);
    
    OptionType valueType() const;
    void setValueType(OptionType newType);
    void setValueType(QString    newType);
    QString valueTypeToString() const;
    static OptionType stringToOptionType(QString type);
    static QString valueTypeToString(OptionType type);
    
    bool    valueToBool() const;
    int     valueToInt() const;
    double  valueToDouble() const;
    QString valueToString() const;
    
    static bool    valueToBool(QString string);
    static int     valueToInt(QString string);
    static double  valueToDouble(QString string);
    static QString valueToString(QString string);
    
    void saveToXmlObject(xmlObject* target) const;
    void loadFromXmlObject(xmlObject* source);
    
    void setValue(bool    newValue);
    void setValue(int     newValue);
    void setValue(double  newValue);
    void setValue(QString newValue);
    
    void setMinimum(double min){ m_fMinimum = min; };
    double minimum() const { return m_fMinimum; };
    
    void setMaximum(double max){ m_fMaximum = max; };
    double maximum() const { return m_fMaximum; };
    
    void setMinMax(double min, double max) { setMinimum(min); setMaximum(max); };
    
    void setChangeableByUser(bool changeable);
    bool changeableByUser() const;
    
    void setDescription(QString newDescription){ m_szDescription = newDescription; };
    QString description() const { return m_szDescription; };
    
    void setWhatsThis(QString newWhatsThis){ m_szWhatsThis = newWhatsThis; };
    QString whatsThis() const { return m_szWhatsThis; };
    
    void setDefaultValue(QString defaultvalue) { m_szDefaultValue = defaultvalue; };
    QString defaultValue() const { return m_szDefaultValue; };
    
    void operator= (const ksConfigOption& other);
    bool operator==(const ksConfigOption& other) const;
    
private:
    QString m_szName;
    QString m_szValue;
    QString m_szDefaultValue;
    OptionType m_nValueType;
    //members, only need for config gui
    QString m_szDescription;
    QString m_szWhatsThis;
    bool    m_bChangeableByUser;
    double  m_fMinimum;
    double  m_fMaximum;
    
    
};

#endif
