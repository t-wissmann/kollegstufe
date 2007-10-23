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
#include "dateConverter.h"
#include <stdio.h>
#include <string.h>
#include <time.h>


int sgn(int value)
{
    if (value > 0)
        return 1;
    
    if (value == 0)
        return 0;
    
    if (value < 0)
        return -1;
    
    return 0;
}

cDateConverter::cDateConverter()
{
    nDay = 2;
    nMonth = 8;
    nYear = 1989;
    szDay[2] = '\0';
    szMonth[2] = '\0';
    szYear[4] = '\0';
    setToCurrentDate();
}

void  cDateConverter::setToCurrentDate()
{
    time_t TimeNow= time(NULL);
    tm TimeStruct=*localtime(&TimeNow);
    setDay( TimeStruct.tm_mday);
    setMonth( TimeStruct.tm_mon+1);
    setYear( TimeStruct.tm_year+1900);
}

int   cDateConverter::Day()
{
    return nDay;
}

int   cDateConverter::Month()
{
    return nMonth;
}

int   cDateConverter::Year()
{
    return nYear;
}
void cDateConverter::operator=( cDateConverter   newDate )
{
    setDay(newDate.Day());
    setMonth(newDate.Month());
    setYear(newDate.Year());
}

void   cDateConverter::setDay(int newDay)
{
    if((newDay >= 0) && (newDay <= 99))
    {
        nDay = newDay;
    }
    if(newDay == LAST_DAY_OF_MONTH)
        nDay = daysInMonth(nMonth, nYear);
    if(nDay < 10)
        snprintf(szDay, 3, "0%d", nDay);
    else
        snprintf(szDay, 3, "%2d", nDay);
    snprintf(szDate, 9,"%s%s%s", szYear, szMonth, szDay);
    snprintf(szHumanDate, 13,"%2d. %2d. %4d", nDay, nMonth, nYear);
}

void   cDateConverter::setMonth(int newMonth)
{
    if((newMonth >= 0) && (newMonth <= 99))
    {
        nMonth = newMonth;
        if(nMonth < 10)
            snprintf(szMonth, 3, "0%d", nMonth);
        else
            snprintf(szMonth, 3, "%2d", nMonth);
        snprintf(szDate, 9,"%s%s%s", szYear, szMonth, szDay);
        snprintf(szHumanDate, 13,"%2d. %2d. %4d", nDay, nMonth, nYear);
    }
}

void   cDateConverter::setYear(int newYear)
{
    if((newYear >= 0) && (newYear <= 9999))
    {
        nYear = newYear;
        
        if(nYear < 10)
            snprintf(szYear, 5, "000%d", nYear);
        else if(nYear < 100)
            snprintf(szYear, 5, "00%2d", nYear);
        else if(nYear < 1000)
            snprintf(szYear, 5, "0%3d", nYear);
        else
            snprintf(szYear, 5, "%4d", nYear);
        snprintf(szDate, 9,"%s%s%s", szYear, szMonth, szDay);
        snprintf(szHumanDate, 13,"%2d. %2d. %4d", nDay, nMonth, nYear);
    }
}

void   cDateConverter::setDateString(const char* szNewDate)
{
    strncpy(szDate, szNewDate, 9);
    szDate[8] = '\0';
    szYear[0] = szDate[0];
    szYear[1] = szDate[1];
    szYear[2] = szDate[2];
    szYear[3] = szDate[3];
    szYear[4] = '\0';
    sscanf(szYear, "%d", &nYear);
    szMonth[0] = szDate[4];
    szMonth[1] = szDate[5];
    szMonth[2] = '\0';
    sscanf(szMonth, "%d", &nMonth);
    szDay[0] = szDate[6];
    szDay[1] = szDate[7];
    szDay[2] = '\0';
    sscanf(szDay, "%d", &nDay);
    sprintf(szHumanDate,"%2d. %2d. %4d", nDay, nMonth, nYear);
}

int cDateConverter::compareDates(cDateConverter date1, cDateConverter date2) //RETURNS -1 if date1 was earlier, 0 if both are equal and 1 if date2 was earlier
{
    //YEAR
    if(date1.Year() < date2.Year())
        return -1;
    if(date1.Year() > date2.Year())
        return 1;
    //MONTH
    if(date1.Month() < date2.Month())
        return -1;
    if(date1.Month() > date2.Month())
        return 1;
    //DAY
    if(date1.Day() < date2.Day())
        return -1;
    if(date1.Day() > date2.Day())
        return 1;
    //if there aren't any differences, the dates must be equal
    return 0;
}

void   cDateConverter::addDays(int daysToAdd)
{
    if( daysToAdd == 0)
        return;
    
    nDay += daysToAdd;
    
    while(nDay > daysInMonth(nMonth, nYear) || nDay < 0)
    {
        addMonths(sgn(nDay));
        if(nDay > 0)
            nDay -= daysInMonth(monthBefore(nMonth), nYear);
        if(nDay < 0)
            nDay += daysInMonth(nMonth, nYear);
    }
    setDay(nDay);
    
}

void   cDateConverter::addMonths(int monthsToAdd)
{
    addYears((int) (monthsToAdd / 12));
    monthsToAdd = monthsToAdd % 12;
    
    while(monthsToAdd != 0)
    {
        if(sgn(monthsToAdd) == 1)
            nMonth = monthAfter( nMonth, &nYear);
        if(sgn(monthsToAdd) == -1)
            nMonth = monthBefore( nMonth, &nYear);
        monthsToAdd -= sgn(monthsToAdd);
    }
    setMonth( nMonth ); // REFREHS ALL
}

void   cDateConverter::addYears(int yearsToAdd)
{
    setYear(nYear +  yearsToAdd);
}

void  cDateConverter::setHumanDate(const char* szNewHumanDate)
{
    strncpy(szHumanDate, szNewHumanDate, 12);
    szHumanDate[12] = '\0';
    sscanf(szHumanDate, "%2d. %2d. %4d", &nDay, &nMonth, &nYear);
    //REFRESH the other variables:
    setDay(nDay);
}
int  cDateConverter::daysInMonth(int  monthNumber, int currentYear)
{
    switch(monthNumber)
    {
        case 1: return 31;
        case 2: return (28+isLeapYear( currentYear));
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
    }
    return 40;
}

int  cDateConverter::daysInMonth(int  monthNumber)
{
    switch(monthNumber)
    {
        case 1: return 31;
        case 2: return 28;
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
    }
    return 40;
}

bool cDateConverter::isLeapYear(int  possibleLeapYear)
{
    //SEE: http://de.wikipedia.org/wiki/Schaltjahr#Die_Schaltregeln (GERMAN)
    //     http://en.wikipedia.org/wiki/Leap_year#Leap_year_rules   (ENGLISH)
    if(possibleLeapYear % 400 == 0)
        return 1;
    if(possibleLeapYear % 100 == 0)
        return 0;
    if(possibleLeapYear % 4 == 0)
        return 1;
    return 0;
}
bool cDateConverter::isDateContainedIn(cDateConverter dateContained, cDateConverter dateStart, cDateConverter dateEnd)
{
    if(cDateConverter::compareDates(dateContained, dateStart) == 0)
        return 1;
    if(cDateConverter::compareDates(dateContained, dateEnd) == 0)
        return 1;
    if(cDateConverter::compareDates(dateContained, dateStart) == (-1)*(cDateConverter::compareDates(dateContained, dateEnd)))
        return 1;
    return 0;
}
bool cDateConverter::isContainedIn(cDateConverter dateStart, cDateConverter dateEnd)
{
    return cDateConverter::isDateContainedIn(*this, dateStart, dateEnd);
}

char* cDateConverter::getDateString()
{
    return szDate;
}

