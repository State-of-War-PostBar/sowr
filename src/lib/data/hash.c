/*************************************************************************************************
*                                                                                                *
*                                  [ State of War: Remastered ]                                  *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                  A free, open-source software project recreating an old game.                  *
*               (c) 2017 - 2021 State of War Baidu Postbar, some rights reserved.                *
*                                                                                                *
*    State of War: Remastered is a free software. You can freely do whatever you want with it    *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                                   published by mhtvsSFrpHdE.                                   *
*                                                                                                *
*  By the time this line is written, the version of the license document is 1, but you may use   *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
*     State of War: Remastered is created, intended to be useful, but without any warranty.      *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*       You should have received a copy of the license along with the source code of this        *
*  program. If not, please see https://github.com/mhtvsSFrpHdE/ipcui/blob/master/LICENSE_JDBM.   *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                         https://github.com/State-of-War-PostBar/sowr.                          *
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#include "hash.h"

sowr_Hash
sowr_GetHash( size_t length, const unsigned char *bytes )
{
    sowr_Hash res = 1ULL;

    size_t i = 0ULL;
    for (i = 0ULL; i < length / 4ULL; i++)
        res += (bytes[i] * bytes[i] >> 5) * 0x1070601686fULL;
    for (i = length / 4ULL; i < length / 2ULL; i++)
        res += (bytes[i] * bytes[i] << 2) * 0x405236496fULL;
    for (i = length / 2ULL; i < length * 3ULL / 4ULL; i++)
        res += (bytes[i] * bytes[i] >> 1) * 0x1966822847fULL;
    for (i = length * 3ULL / 4ULL; i < length; i++)
        res += (bytes[i] * bytes[i] << 4) * 0x732464301fULL;

    return res;
}

inline
sowr_Hash
sowr_GetHashI( const unsigned char *str )
{
    return sowr_GetHash(strlen((char *)str), str);
}

inline
sowr_Hash
sowr_GetHashS( const sowr_String *str )
{
    return sowr_GetHash(str->length, (unsigned char *)(str->ptr));
}
