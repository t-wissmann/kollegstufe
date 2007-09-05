/***************************************************************************
 *   Copyright (C) 2007 by Thorsten Wissmann   *
 *   towi89@web.de  http://towi16.piranho.de *
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



#include "error.h" 
#include <stdlib.h>
#include <stdio.h>


char**  ErrorCodeText;

void ErrorOutput(long ErrorCode)
{
    printf("ERROR: %s\n", ErrorCodeText[ErrorCode+500]);
}

void InitErrorCodes()
{
    int i;
    ErrorCodeText = (char**) malloc( (500 + ErrorCodeNumber) * sizeof(char*));
    for(i=0;i< (500 + ErrorCodeNumber); i++)
    {
        ErrorCodeText[i] = (char*) malloc( 80 * sizeof(char));
    }
    sprintf(ErrorCodeText[ErrorGeneral +500], "General Error");
    sprintf(ErrorCodeText[ErrorNoMemory +500], "No Memory");
    sprintf(ErrorCodeText[ErrorFileNotFound +500], "File Not Found");
    sprintf(ErrorCodeText[ErrorInvalidCommand +500], "Invalid Command");
    sprintf(ErrorCodeText[ErrorInvalidParameters +500], "Invalid Parameters");
    sprintf(ErrorCodeText[ErrorInvalidDataFormat +500], "Invalid Data Format");
    sprintf(ErrorCodeText[ErrorEndOfFile +500], "Reached End of File");
    
}

void FreeErrorCodes()
{
    int i;
    for(i=0;i< (500 + ErrorCodeNumber); i++)
        free(ErrorCodeText[i]);
    free(ErrorCodeText);
}


