/*************************************************************************************************
*                                                                                                *
*                                  [ State of War: Remastered ]                                  *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                  A free, open-source software project recreating an old game.                  *
*               (c) 2017 - 2020 State of War Baidu Postbar, some rights reserved.                *
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

#include "string.h"

#include "../memory/heap_memory.h"

sowr_String *
sowr_String_Create( void )
{
    return sowr_HeapZeroAlloc(sizeof(sowr_String));
}

sowr_String
sowr_String_CreateS( void )
{
    sowr_String str =
    {
        .capacity = 0ULL,
        .length = 0ULL,
        .ptr = NULL
    };
    return str;
}

sowr_String *
sowr_String_From( const char *original )
{
    sowr_String *str = sowr_HeapAlloc(sizeof(sowr_String));
    str->length = str->capacity = strlen(original);
    str->ptr = sowr_HeapAlloc(str->length + 1ULL);
    memcpy(str->ptr, original, str->length + 1ULL);
    return str;
}

sowr_String
sowr_String_FromS( const char *original )
{
    sowr_String str;
    str.length = str.capacity = strlen(original);
    str.ptr = sowr_HeapAlloc(str.length + 1ULL);
    memcpy(str.ptr, original, str.length + 1ULL);
    return str;
}

void
sowr_String_Expand( sowr_String *str )
{
    if (!str->capacity)
    {
        str->capacity = 2ULL;
        str->ptr = sowr_HeapAlloc(sizeof(char) * 2ULL);
    }
    else
    {
        str->capacity *= 2ULL;
        str->ptr = sowr_ReAlloc(str->capacity, str->ptr);
    }
}

inline
void
sowr_String_ExpandUntil( sowr_String *str, size_t new_size )
{
    while (str->capacity < new_size)
        sowr_String_Expand(str);
}

void
sowr_String_ShrinkToFit( sowr_String *str )
{
    if (str->capacity > str->length && str->length)
    {
        str->ptr = sowr_ReAlloc(sizeof(char) * (str->length + 1ULL), str->ptr);
        str->capacity = str->length + 1ULL;
    }
    else if (str->capacity && !str->length)
    {
        sowr_HeapFree(str->ptr);
        str->capacity = 0ULL;
    }
}

inline
char *
sowr_String_First( const sowr_String *str )
{
    return str->ptr;
}

inline
char *
sowr_String_Last( const sowr_String *str )
{
    if (!str->length)
        return NULL;

    return str->ptr + str->length - 1ULL;
}

void
sowr_String_PushC( sowr_String *str, char data )
{
    sowr_String_ExpandUntil(str, sizeof(char) * (str->length + 2ULL));
    str->ptr[str->length] = data;
    str->ptr[str->length + 1ULL] = '\0';
    str->length++;
}

void
sowr_String_PushS( sowr_String *str, const char *data )
{
    size_t target_len = strlen(data);
    sowr_String_ExpandUntil(str, sizeof(char) * (str->length + target_len + 1ULL));
    memcpy(str->length ? str->ptr + str->length : str->ptr, data, sizeof(char) * (target_len + 1ULL));
    str->length += target_len;
}

inline
void
sowr_String_Clear( sowr_String *str )
{
    str->length = 0ULL;
}

void
sowr_String_Destroy( sowr_String *str )
{
    sowr_String_Clear(str);
    if (str->capacity)
        sowr_HeapFree(str->ptr);
    sowr_HeapFree(str);
}

void
sowr_String_DestroyS( sowr_String *str )
{
    sowr_String_Clear(str);
    if (str->capacity)
        sowr_HeapFree(str->ptr);
}