/**
 * \file            lwcell_opts_template.h
 * \brief           Template config file
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
#ifndef LWCELL_OPTS_HDR_H
#define LWCELL_OPTS_HDR_H

/* Rename this file to "lwcell_opts.h" for your application */

/*
 * Open "include/lwcell/lwcell_opt.h" and
 * copy & replace here settings you want to change values
 */

#if !__DOXYGEN__
#define LWCELL_CFG_DBG               LWCELL_DBG_ON
#define LWCELL_CFG_DBG_TYPES_ON      (LWCELL_DBG_TYPE_TRACE | LWCELL_DBG_TYPE_STATE)
#define LWCELL_CFG_DBG_MQTT          LWCELL_DBG_ON
#define LWCELL_CFG_DBG_INIT          LWCELL_DBG_ON
#define LWCELL_CFG_DBG_MEM           LWCELL_DBG_ON
#define LWCELL_CFG_DBG_INPUT         LWCELL_DBG_ON
#define LWCELL_CFG_DBG_THREAD        LWCELL_DBG_ON
#define LWCELL_CFG_DBG_ASSERT        LWCELL_DBG_ON
#define LWCELL_CFG_DBG_IPD           LWCELL_DBG_ON
#define LWCELL_CFG_DBG_PBUF          LWCELL_DBG_ON
#define LWCELL_CFG_DBG_CONN          LWCELL_DBG_ON
#define LWCELL_CFG_DBG_VAR           LWCELL_DBG_ON
#define LWCELL_CFG_DBG_NETCONN       LWCELL_DBG_ON
#define LWCELL_CFG_DBG_HTTP_CLIENT   LWCELL_DBG_ON

//#define LWCELL_CFG_CONN_MAX_DATA_LEN 1460
#define LWCELL_CFG_INPUT_USE_PROCESS 1
#define LWCELL_CFG_AT_ECHO           0

#define LWCELL_CFG_NETWORK           1
#define LWCELL_CFG_NETCONN           1
#define LWCELL_CFG_CONN              1

/// TODO: 应用层协议配置
#define LWCELL_CFG_PROTOCOL          1
#define LWCELL_CFG_HTTP              1
#define LWCELL_CFG_MQTT              0
#define LWCELL_CFG_FTP               0


#define LWCELL_CFG_MAX_CONNS         8


#define LWCELL_CFG_HTTP_CONN_OFFSET  (LWCELL_CFG_MAX_CONNS)
#define LWCELL_CFG_MAX_HTTP_CONNS    1
#define LWCELL_CFG_HTTP_MAX_REQUESTS 1
#define LWCELL_CFG_HTTP_MBOX_SIZE    8

#define LWCELL_CFG_MQTT_CONN_OFFSET  (LWCELL_CFG_HTTP_CONN_OFFSET + LWCELL_CFG_MAX_HTTP_CONNS)
#define LWCELL_CFG_MAX_MQTT_CONNS    1



#define LWCELL_CFG_SMS               0
#define LWCELL_CFG_CALL              0
#define LWCELL_CFG_PHONEBOOK         0
#define LWCELL_CFG_USSD              0

//#define LWCELL_CFG_USE_API_FUNC_EVT  1

#define LWCELL_CFG_RESET_ON_INIT     1


#define LWCELL_CFG_MEM_CUSTOM        1

#include "printf.h"

#endif /* !__DOXYGEN__ */


#endif /* LWCELL_OPTS_HDR_H */
