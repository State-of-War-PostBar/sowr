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

#ifndef SOWR_LIB_CONTAINER_HASH_MAP_H
#define SOWR_LIB_CONTAINER_HASH_MAP_H

#include <pch.h>

#include "string.h"
#include "vector.h"
#include "../data/hash.h"

typedef void (*sowr_HashMapWalkFunc)( void * );
typedef void (*sowr_HashMapFreeFunc)( void * );

typedef struct sowr_HashMapValue
{
    sowr_Hash index_hash;
    size_t data_size;
    sowr_HashMapFreeFunc free_func;
    void *data;
} sowr_HashMapValue;

typedef struct sowr_HashMap
{
    size_t buckets_count;
    size_t length;
    sowr_Vector buckets;
    sowr_HashMapFreeFunc free_func;
} sowr_HashMap;

///
/// \brief Create a hashmap
///
/// Create a hashmap, with default buckets, ready to use.
///
/// \param free_func Function to call when freeing an element
///
/// \return A hashmap
///
sowr_HashMap *
sowr_HashMap_Create( sowr_HashMapFreeFunc free_func );

///
/// \brief Create a hashmap
///
/// Create a hashmap by stack, with default buckets, ready to use.<BR />
/// <B>The created hashmap must be destroyed with \a sowr_HashMap_DestroyS().</B>
///
/// \param free_func Function to call when freeing an element
///
/// \return A hashmap
///
sowr_HashMap
sowr_HashMap_CreateS( sowr_HashMapFreeFunc free_func );

///
/// \brief Create a hashmap
///
/// Create a hashmap with custom number of buckets, ready to use.
///
/// \param buckets_count Suggested buckets count
/// \param free_func Function to call when freeing an element
///
/// \return A hashmap
///
sowr_HashMap *
sowr_HashMap_Create_SuggestBucketsCount( size_t buckets_count, sowr_HashMapFreeFunc free_func );

///
/// \brief Create a hashmap
///
/// Create a hashmap with custom number of buckets by stack, ready to use.<BR />
/// <B>The created hashmap must be destroyed with \a sowr_HashMap_DestroyS().</B>
///
/// \param buckets_count Suggested buckets count
/// \param free_func Function to call when freeing an element
///
/// \return A hashmap
///
sowr_HashMap
sowr_HashMap_Create_SuggestBucketsCountS( size_t buckets_count, sowr_HashMapFreeFunc free_func );

///
/// \brief Insert an element to hashmap
///
/// Use key and value to insert element to hashmap, override the old value if key is identical.
///
/// \param map Map to insert
/// \param index_length Length of index in byte
/// \param index Index
/// \param val_length Length of value in byte
/// \param value Value to insert
///
void
sowr_HashMap_Insert( sowr_HashMap *map, size_t index_length, const char *index, size_t val_length, const void *value );

///
/// \brief Insert an element to hashmap
///
/// Use key and value to insert element to hashmap, override the old value if key is identical.<BR />
/// The key and value are understood to be null-terminated strings.
///
/// \param map Map to insert
/// \param index Index
/// \param value Value to insert
///
void
sowr_HashMap_InsertCC( sowr_HashMap *map, const char *index, const char *value );

///
/// \brief Insert an element to hashmap
///
/// Use key and value to insert element to hashmap, override the old value if key is identical.<BR />
/// The key is understood to be null-terminated string.
///
/// \param map Map to insert
/// \param index Index
/// \param val_length Length of value in byte
/// \param value Value to insert
///
void
sowr_HashMap_InsertCV( sowr_HashMap *map, const char *index, size_t val_length, const void *value );

///
/// \brief Insert an element to hashmap
///
/// Use key and value to insert element to hashmap, override the old value if key is identical.<BR />
///
/// \param map Map to insert
/// \param index Index
/// \param value Value to insert
///
void
sowr_HashMap_InsertSS( sowr_HashMap *map, const sowr_String *index, const sowr_String *value );

///
/// \brief Insert an element to hashmap
///
/// Use key and value to insert element to hashmap, override the old value if key is identical.<BR />
/// The  value is understood to be null-terminated string.
///
/// \param map Map to insert
/// \param index Index
/// \param val_length Length of value in byte
/// \param value Value to insert
///
void
sowr_HashMap_InsertSV( sowr_HashMap *map, const sowr_String *index, size_t val_length, const void *value );

///
/// \brief Get a value from the map
///
/// Use key to get a value from the map.
///
/// \param map Map to get
/// \param index_length Length of index in byte
/// \param index Index
///
/// \return The value from key, NULL if not found
///
sowr_HashMapValue *
sowr_HashMap_Get( sowr_HashMap *map, size_t index_length, const char *index );

///
/// \brief Get a value from the map
///
/// Use key to get a value from the map.<BR />
/// The key is understood to be null-terminated string.
///
/// \param map Map to get
/// \param index Index
///
/// \return The value from key, NULL if not found
///
sowr_HashMapValue *
sowr_HashMap_GetC( sowr_HashMap *map, const char *index );


///
/// \brief Get a value from the map
///
/// Use string index to get a value from the map.
///
/// \param map Map to get
/// \param index Index
///
/// \return The value from index, NULL if not found
///
sowr_HashMapValue *
sowr_HashMap_GetS( sowr_HashMap *map, const sowr_String *index );

///
/// \brief Walk the map
///
/// Walk the map with given function.
///
/// \param map Map to walk
/// \param func Function to use
///
void
sowr_HashMap_Walk( sowr_HashMap *map, const sowr_HashMapWalkFunc func );

///
/// \brief Delete a value from the map
///
/// Delete a value by index from the map.
///
/// \param map Map to operate
/// \param index_length Length of index in byte
/// \param index Index
///
void
sowr_HashMap_Delete( sowr_HashMap *map, size_t index_length, const char *index );

///
/// \brief Delete a value from the map
///
/// Delete a value by index from the map.<BR />
/// The index is understood to be null-terminated string.
///
/// \param map Map to operate
/// \param index Index
///
void
sowr_HashMap_DeleteC( sowr_HashMap *map, const char *index );

///
/// \brief Delete a value from the map
///
/// Delete a value by index from the map using string as index.
///
/// \param map Map to operate
/// \param index Index
///
void
sowr_HashMap_DeleteS( sowr_HashMap *map, const sowr_String *index );

///
/// \brief Clear the hashmap
///
/// Clear the contents of the hashmap.
///
/// \param map Map to clear
///
void
sowr_HashMap_Clear( sowr_HashMap *map );

///
/// \brief Destroy the hashmap
///
/// Destroy the hashmap.
///
/// \param map Map to destroy
///
void
sowr_HashMap_Destroy( sowr_HashMap *map );

///
/// \brief Destroy the hashmap
///
/// Destroy the hashmap created by \a sowr_HashMap_CreateS() or \a sowr_HashMap_Create_SuggestBucketsCountS().
///
/// \param map Map to destroy
///
void
sowr_HashMap_DestroyS( sowr_HashMap *map );

#endif //!SOWR_LIB_CONTAINER_HASH_MAP_H
