/**
 * \file            lwcell_mem_lwmem.c
 * \brief           Dynamic memory manager implemented with FreeRTOS Heap4
 */

/*
 * Copyright (c) 2023 Tilen MAJERLE
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of LwCELL - Lightweight GSM-AT parser library.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 * Version:         v0.1.1
 */
#include "lwcell/lwcell_types.h"
#include "FreeRTOS.h"

#if !__DOXYGEN__

void*
lwcell_mem_malloc(size_t size) {
    void* pointer;
    pointer = pvPortMalloc(size);
    return pointer;
}

void*
lwcell_mem_realloc(void* ptr, size_t size) {
    /* No need to support it */
    void *new_ptr;
    size_t old_size;

    if(ptr == NULL){
        return pvPortMalloc(size);
    }
    old_size = *(size_t *)((ptr - (( 8 + ( ( size_t ) ( portBYTE_ALIGNMENT - 1 ) ) ) & ~( ( size_t ) portBYTE_ALIGNMENT_MASK ))) + 4);

    new_ptr = pvPortMalloc(size);
    if(new_ptr){
        memcpy(new_ptr, ptr, configMIN(size, old_size));
        vPortFree(ptr);
    }

    return NULL;
}

void*
lwcell_mem_calloc(size_t num, size_t size) {
    size_t total = num * size;
    void* pointer = pvPortMalloc(total);
    memset(pointer, 0, total);
    return pointer;
}

void
lwcell_mem_free(void* ptr) {
    (void) vPortFree(ptr);
}

#endif /* !__DOXYGEN__ */
