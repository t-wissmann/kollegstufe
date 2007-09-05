/***************************************************************************
 *   Copyright (C) 2007 by Thorsten Wissmann   *
 *   towi89@web.de http://towi16.piranho.de   *
 *   http://www.kde-look.org/usermanager/search.php?username=twi           *
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

#ifndef DATECONVERTER_H
#define DATECONVERTER_H


#define LAST_DAY_OF_MONTH -1

enum MONTHS
{
    MONTH_NULL,
    JANUARY,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNI,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
};

class cDateConverter
{
    public:
        cDateConverter();
        int   Day();
        int   Month();
        int   Year();
        void  setDay(int newDay);
        void  setMonth(int newMonth);
        void  setYear(int newYear);
        void  setToCurrentDate();
        void  setDateString(const char* szNewDate);
        char* getDateString();
        void  setHumanDate(const char* szNewHumanDate);
        char* humanDate(){ return szHumanDate; };
        char  szHumanDate[13];
        void  operator=( cDateConverter   newDate );
        void   addDays(int daysToAdd);
        void   addMonths(int monthsToAdd);
        void   addYears(int yearsToAdd);
        static int compareDates(cDateConverter date1, cDateConverter date2); //RETURNS -1 if date1 was earlies, 0 if both are equal and 1 if date2 was earlier)
        static bool isDateContainedIn(cDateConverter dateContained, cDateConverter dateStart, cDateConverter dateEnd);
        bool isContainedIn(cDateConverter dateStart, cDateConverter dateEnd);
        static bool isLeapYear(int  possibleLeapYear);
        static int  daysInMonth(int  monthNumber, int currentYear);
        static int  daysInMonth(int  monthNumber);
        static int  monthBefore(int  month)
        {
            if(month == JANUARY)
                return DECEMBER;
            return month - 1;
        };
        static int  monthAfter(int  month)
        {
            if(month == DECEMBER)
                return JANUARY;
            return month + 1;
        };
        static int  monthBefore(int  month, int* currentYear)
        {
            if(month == JANUARY)
            {
                (*currentYear)--;
                return DECEMBER;
            }
            return month - 1;
        };
        static int  monthAfter(int  month, int* currentYear)
        {
            if(month == DECEMBER)
            {
                (*currentYear)++;
                return JANUARY;
            }
            return month + 1;
        };
    private:
        int   nDay;
        int   nMonth;
        int   nYear;
        char  szDate[9];
        char szDay[3];
        char szMonth[3];
        char szYear[5];
};


int sgn(int value);


#endif
