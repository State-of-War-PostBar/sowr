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

#include "trie.h"

#include "../memory/heap_memory.h"

static
inline
sowr_TrieNode *
sowr_TrieNode_Gen( void )
{
    return sowr_HeapZeroAlloc(sizeof(sowr_TrieNode));
}

static
void
sowr_TrieNode_DeleteAfter( sowr_TrieNode *node, sowr_TrieFreeFunc free_func )
{
    if (!node)
        return;

    if (node->children)
        for (size_t i = 0ULL; i < CHAR_MAX; i++)
            if (node->characters[i])
            {
                sowr_TrieNode_DeleteAfter(node->characters[i], free_func);
                node->characters[i] = NULL;
                node->children--;
            }

    if (node->data)
    {
        if (free_func)
            free_func(node->data);
        sowr_HeapFree(node->data);
    }
    sowr_HeapFree(node);
}

sowr_Trie *
sowr_Trie_Create( sowr_TrieFreeFunc free_func )
{
    sowr_TrieNode head;
    head.data_size = 0ULL;
    head.data = NULL;
    head.children = 0ULL;
    for (size_t i = 0ULL; i < CHAR_MAX; i++)
        head.characters[i] = NULL;

    sowr_Trie *trie = sowr_HeapAlloc(sizeof(sowr_Trie));
    trie->free_func = free_func;
    trie->head = head;

    return trie;
}

sowr_Trie
sowr_Trie_CreateS( sowr_TrieFreeFunc free_func )
{
    sowr_TrieNode head;
    head.data_size = 0ULL;
    head.data = NULL;
    head.children = 0ULL;
    for (size_t i = 0ULL; i < CHAR_MAX; i++)
        head.characters[i] = NULL;

    sowr_Trie trie =
    {
        .free_func = free_func,
        .head = head
    };

    return trie;
}

void
sowr_Trie_Clear( sowr_Trie *trie )
{
    if (!trie)
        return;

    if (trie->head.children)
        for (size_t i = 0ULL; i < CHAR_MAX; i++)
            sowr_TrieNode_DeleteAfter(trie->head.characters[i], trie->free_func);
    trie->head.children = 0ULL;
}

void
sowr_Trie_Insert( sowr_Trie *trie, const char *index, size_t data_size, const void *data )
{
    if (!trie)
        return;

    sowr_TrieNode *iter = &(trie->head);
    size_t ch = 0ULL;
    while (*index)
    {
        ch = (size_t)(*index);
        if (!iter->characters[ch])
        {
            iter->characters[ch] = sowr_TrieNode_Gen();
            iter->children++;
        }

        iter = iter->characters[ch];
        index++;
    }

    if (!iter->data)
    {
        iter->data = sowr_HeapAlloc(data_size);
        memcpy(iter->data, data, data_size);
    }
    else
    {
        if (trie->free_func)
            trie->free_func(iter->data);
        memcpy(iter->data, data, data_size);
    }
}

inline
void
sowr_Trie_InsertS( sowr_Trie *trie, const sowr_String *index, size_t data_size, const void *data )
{
    sowr_Trie_Insert(trie, index->ptr, data_size, data);
}

sowr_TrieNode *
sowr_Trie_Get( sowr_Trie *trie, const char *index )
{
    if (!trie)
        return NULL;

    sowr_TrieNode *iter = &(trie->head);
    while (*index)
    {
        iter = iter->characters[(size_t)*index];
        if (!iter)
            return NULL;
        index++;
    }

    return iter;
}

inline
sowr_TrieNode *
sowr_Trie_GetS( sowr_Trie *trie, const sowr_String *index )
{
    return sowr_Trie_Get(trie, index->ptr);
}

bool
sowr_Trie_Delete( sowr_Trie *trie, const char *index )
{
    if (!trie)
        return false;

    sowr_TrieNode *iter = &(trie->head);
    while (*index)
    {
        iter = iter->characters[(size_t)(*index)];
        if (!iter)
            return false;
        index++;
    }

    if (iter->data)
    {
        if (trie->free_func)
            trie->free_func(iter->data);
        sowr_HeapFree(iter->data);
    }
    iter->data = NULL;

    return true;
}

inline
bool
sowr_Trie_DeleteS( sowr_Trie *trie, const sowr_String *index )
{
    return sowr_Trie_Delete(trie, index->ptr);
}

void
sowr_Trie_Destroy( sowr_Trie *trie )
{
    sowr_Trie_Clear(trie);
    sowr_HeapFree(trie);
}

void
sowr_Trie_DestroyS( sowr_Trie *trie )
{
    sowr_Trie_Clear(trie);
}
