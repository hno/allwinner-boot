/*
* (C) Copyright 2007-2013
* Allwinner Technology Co., Ltd. <www.allwinnertech.com>
* Martin zheng <zhengjiewen@allwinnertech.com>
*
* See file CREDITS for list of people who contributed to this
* project.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston,
* MA 02111-1307 USA
*/

#include "fat_priv.h"

/*
**********************************************************************************************************************
*             Global functions
**********************************************************************************************************************
*/

/*
**********************************************************************************************************************
*             fsys_fat_fseek
*
  Description:
  FS internal function. Read data from a file.

  Parameters:
  pData       - Pointer to a data buffer for storing data transferred
                from file. 
  Size        - Size of an element to be transferred from file to data
                buffer
  N           - Number of elements to be transferred from the file.
  pFile       - Pointer to a FS_FILE data structure.
  
  Return value:
  Number of elements read.
**********************************************************************************************************************
*/

__s32 fsys_fat_fseek(__fsys_file_t *  pFile, __s32 Offset, __s32 Whence) 
{

    pFile->error     = FSYS_ERR_OK;                 /* Clear any previous error                                     */
    pFile->CurClust  = 0;                           /* Invalidate current cluster                                   */
    
    switch(Whence)
    {
    	case FSYS_FAT_SEEK_SET:
    	    if (Offset <= pFile->size) 
        	{
            	pFile->filepos = Offset;
        	}
        	else 
        	{
            	pFile->error = FSYS_ERR_INVALIDPAR; /* New position would be behind EOF                             */
            	return EPDK_FAIL;
        	}
        	break;
    
    	case FSYS_FAT_SEEK_CUR:
        	if ((pFile->filepos + Offset) <= pFile->size) 
        	{
            	pFile->filepos += Offset;
        	}
        	else 
        	{
            	pFile->error = FSYS_ERR_INVALIDPAR; /* New position would be behind EOF                             */
            	return EPDK_FAIL;
        	}
        	break;
    
    	case FSYS_FAT_SEEK_END:
	        if((Offset > 0) || ((Offset + pFile->size) < 0))
    	    {
        		pFile->error = FSYS_ERR_INVALIDPAR; /* The file system does not support this                        */
        		return EPDK_FAIL;
        	}
        	pFile->filepos = Offset + pFile->size;
        	break;
    	default:
    		pFile->error = FSYS_ERR_INVALIDPAR;     /* Parameter 'Whence' is invalid                                */
        	return EPDK_FAIL;
    }
    return EPDK_OK;
}

