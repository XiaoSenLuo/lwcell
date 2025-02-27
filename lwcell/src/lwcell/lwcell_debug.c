/**
 * \file            lwcell_debug.c
 * \brief           Debugging inside GSM stack
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
 * This file is part of LwCELL - Lightweight cellular modem AT library.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 * Version:         v0.1.1
 */
#include "lwcell/lwcell_debug.h"
#include "lwcell/lwcell_private.h"

#if LWCELL_CFG_DBG || __DOXYGEN__

const char*
lwcelli_dbg_msg_to_string(lwcell_cmd_t cmd) {
    static char tmp_arr[100];
    if (cmd) {
        sprintf(tmp_arr, "%d", (int)cmd);
        return tmp_arr;
    }
    return "";
}

#endif /* LWCELL_CFG_DBG || __DOXYGEN__ */
