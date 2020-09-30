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

#include "fio.h"

#include "../container/string.h"
#include "../data/unicode.h"
#include "../memory/heap_memory.h"

sowr_FileDescriptor
sowr_File_OpenR( const char *path )
{
#ifdef SOWR_TARGET_WINDOWS
    sowr_Vector utf16 = sowr_Vector_CreateS(sizeof(unsigned char), NULL);
    sowr_Unicode_UTF8ToUTF16((unsigned char *)path, &utf16);
    sowr_FileDescriptor hdl = CreateFileW((wchar_t *)(utf16.ptr), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    sowr_Vector_DestroyS(&utf16);
    return hdl;
#else
    return open(path, O_RDONLY);
#endif
}

sowr_FileDescriptor
sowr_File_OpenW( const char *path, sowr_FileWriteMode mode )
{
#ifdef SOWR_TARGET_WINDOWS
    sowr_Vector utf16 = sowr_Vector_CreateS(sizeof(unsigned char), NULL);
    sowr_Unicode_UTF8ToUTF16((unsigned char *)path, &utf16);
    sowr_FileDescriptor hdl = CreateFileW((wchar_t *)(utf16.ptr),
                        GENERIC_READ | (mode == SOWR_FIO_WRITE_APPEND ? FILE_APPEND_DATA : GENERIC_WRITE),
                        0, NULL,
                        mode == SOWR_FIO_WRITE_TRUNCATE ? TRUNCATE_EXISTING : OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL, NULL);
    sowr_Vector_DestroyS(&utf16);
    return hdl;
#else
    return open(path, O_RDWR | (mode == SOWR_FIO_WRITE_APPEND ? O_APPEND :
                              (mode == SOWR_FIO_WRITE_TRUNCATE ? O_TRUNC : O_RDWR)));
#endif
}

sowr_FileDescriptor
sowr_File_OpenOrCreate( const char *path, sowr_FileWriteMode mode )
{
    sowr_FileDescriptor file = sowr_File_OpenW(path, mode);
#ifdef SOWR_TARGET_WINDOWS
    sowr_Vector utf16 = sowr_Vector_CreateS(sizeof(unsigned char), NULL);
#endif
    if (file != SOWR_INVALID_FILE_DESCRIPTOR)
        return file;

    char *last_dir = strrchr(path, '/');
    if (!last_dir)
#ifdef SOWR_TARGET_WINDOWS
        file = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
#else
        file = open(path, O_RDWR | O_CREAT, S_IRWXU);
#endif
    else
    {
        sowr_String str = sowr_String_CreateS();
        const char *path_r = path;
        while (*path_r)
        {
            while (*path_r != '/')
                sowr_String_PushC(&str, *path_r++);
#ifdef SOWR_TARGET_WINDOWS
            sowr_Unicode_UTF8ToUTF16((unsigned char *)str.ptr, &utf16);
            CreateDirectoryW((wchar_t *)(utf16.ptr), NULL);
#else
            mkdir(str.ptr, S_IRWXU);
#endif
            if (path_r == last_dir)
                break;
            sowr_String_PushC(&str, '/');
            path_r++;
        }
#ifdef SOWR_TARGET_WINDOWS
        sowr_Vector_Clear(&utf16);
        sowr_Unicode_UTF8ToUTF16((unsigned char *)path, &utf16);
        file = CreateFileW((wchar_t *)(utf16.ptr), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
#else
        file = open(path, O_RDWR | O_CREAT, S_IRWXU);
#endif
        sowr_String_DestroyS(&str);
    }
#ifdef SOWR_TARGET_WINDOWS
    sowr_Vector_DestroyS(&utf16);
#endif
    return file;
}

void
sowr_File_Close( sowr_FileDescriptor file )
{
    if (file != SOWR_INVALID_FILE_DESCRIPTOR)
#ifdef SOWR_TARGET_WINDOWS
        CloseHandle(file);
#else
        close(file);
#endif
}

void
sowr_File_Mkdir( const char *path )
{
    sowr_String str = sowr_String_CreateS();
#ifdef SOWR_TARGET_WINDOWS
    sowr_Vector utf16 = sowr_Vector_CreateS(sizeof(unsigned char), NULL);
#endif
    while (*path)
    {
        while (*path != '/')
        {
            sowr_String_PushC(&str, *path);
            path++;
        }
#ifdef SOWR_TARGET_WINDOWS
        sowr_Vector_Clear(&utf16);
        sowr_Unicode_UTF8ToUTF16((unsigned char *)str.ptr, &utf16);
        CreateDirectoryW((wchar_t *)(utf16.ptr), NULL);
#else
        mkdir(str.ptr, S_IRWXU);
#endif
        sowr_String_PushC(&str, '/');
        path++;
    }
#ifdef SOWR_TARGET_WINDOWS
    sowr_Vector_DestroyS(&utf16);
#endif
    sowr_String_DestroyS(&str);
}

void
sowr_File_WalkDir( const char *path, sowr_DirWalkFunc func )
{
    sowr_String str = sowr_String_FromS(path);
#ifdef SOWR_TARGET_WINDOWS
    sowr_Vector utf8 = sowr_Vector_CreateS(sizeof(unsigned char), NULL);
    sowr_Vector utf16 = sowr_Vector_CreateS(sizeof(unsigned char), NULL);
    sowr_Unicode_UTF8ToUTF16((unsigned char *)path, &utf16);
    sowr_String_PushS(&str, "/*");
    WIN32_FIND_DATAW find_data;
    HANDLE f_entry = FindFirstFileW((wchar_t *)(utf16.ptr), &find_data);
    if (f_entry == INVALID_HANDLE_VALUE)
    {
        sowr_Vector_DestroyS(&utf16);
        sowr_Vector_DestroyS(&utf8);
        sowr_String_DestroyS(&str);
        return;
    }

    do
    {
        sowr_Unicode_UTF16ToUTF8((unsigned char *)find_data.cFileName, &utf8);
        if (!strcmp(utf8.ptr, ".") || !strcmp(utf8.ptr, ".."))
            continue;

        sowr_String_Clear(&str);
        sowr_String_PushS(&str, path);
        sowr_String_PushC(&str, '/');
        sowr_String_PushS(&str, utf8.ptr);
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            sowr_File_WalkDir(str.ptr, func);
        else
            func(str.ptr);
    } while (FindNextFileW(f_entry, &find_data));

    FindClose(f_entry);
#else
    DIR *dir = opendir(str.ptr);
    struct dirent *f_entry = readdir(dir);
    struct stat64 f_stat;

    if (!dir || !f_entry)
    {
        sowr_String_DestroyS(&str);
        return;
    }

    do
    {
        if (!strcmp(f_entry->d_name, ".") || !strcmp(f_entry->d_name, ".."))
            continue;

        sowr_String_Clear(&str);
        sowr_String_PushS(&str, path);
        sowr_String_PushC(&str, '/');
        sowr_String_PushS(&str, f_entry->d_name);
        if (!stat64(str.ptr, &f_stat) && S_ISDIR(f_stat.st_mode))
            sowr_File_WalkDir(str.ptr, func);
        else
            func(str.ptr);
    } while ((f_entry = readdir(dir)));

    closedir(dir);
#endif
#ifdef SOWR_TARGET_WINDOWS
    sowr_Vector_DestroyS(&utf16);
    sowr_Vector_DestroyS(&utf8);
#endif
    sowr_String_DestroyS(&str);
}

size_t
sowr_File_GetSize( sowr_FileDescriptor file )
{
#ifdef SOWR_TARGET_WINDOWS
    LARGE_INTEGER sz;
    GetFileSizeEx(file, &sz);
    return sz.QuadPart;
#else
    struct stat64 f_stat;
    fstat64(file, &f_stat);
    return f_stat.st_size;
#endif
}

bool
sowr_File_ReadContent( sowr_FileDescriptor file, void *buffer, size_t sz )
{
#ifdef SOWR_TARGET_WINDOWS
    return ReadFile(file, buffer, sz, NULL, NULL);
#else
    return read(file, buffer, sz);
#endif
}

bool
sowr_File_WriteContent( sowr_FileDescriptor file, const void *buffer, size_t sz )
{
#ifdef SOWR_TARGET_WINDOWS
    return WriteFile(file, buffer, sz, NULL, NULL);
#else
    return write(file, buffer, sz);
#endif
}

char *
sowr_FileEx_ReadContent( const char *path )
{
    sowr_FileDescriptor file = sowr_File_OpenR(path);
    if (file == SOWR_INVALID_FILE_DESCRIPTOR)
        return NULL;

    size_t sz = sowr_File_GetSize(file);
    char *content = sowr_HeapAlloc(sz);
    sowr_File_ReadContent(file, content, sz);

    sowr_File_Close(file);
    return content;
}
