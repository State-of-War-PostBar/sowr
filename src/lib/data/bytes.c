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

#include "bytes.h"

bool
sowr_Endianness( void )
{
    const short num = 1;
    const signed char *const ptr = (const signed char *const) &num;
    return ptr[0] == 1;
}

inline
void
sowr_Swap( size_t sz, unsigned char *b_l, unsigned char *b_r )
{
    if (!sz)
        return;

    unsigned char b_lr[sz], b_rl[sz];
    memcpy(b_lr, b_l, sz);
    memcpy(b_rl, b_r, sz);
    memcpy(b_l, b_rl, sz);
    memcpy(b_r, b_lr, sz);
}

void
sowr_SwapEndian( size_t length, unsigned char *bytes )
{
    unsigned char byte1 = 0, byte2 = 0;
    for (size_t i = 0ULL, j = length - 1ULL; i < j; i++, j--)
    {
        byte1 = bytes[i];
        byte2 = bytes[j];
        bytes[i] = byte2;
        bytes[j] = byte1;
    }
}

inline
uint16_t
sowr_SwapEndian16( uint16_t val )
{
    return (val << 010U) | (val >> 010U);
}

inline
uint32_t
sowr_SwapEndian32( uint32_t val )
{
    return (val & 0xff000000U) >>  030U |
            (val & 0x00ff0000U) >> 010U |
            (val & 0x0000ff00U) << 010U |
            (val & 0x000000ffU) << 030U;
}

inline
uint64_t
sowr_SwapEndian64( uint64_t val )
{
    return (val & 0xff00000000000000ULL) >>  070U |
            (val & 0x00ff000000000000ULL) >> 050U |
            (val & 0x0000ff0000000000ULL) >> 030U |
            (val & 0x000000ff00000000ULL) >> 010U |
            (val & 0x00000000ff000000ULL) << 010U |
            (val & 0x0000000000ff0000ULL) << 030U |
            (val & 0x000000000000ff00ULL) << 050U |
            (val & 0x00000000000000ffULL) << 070U;
}
