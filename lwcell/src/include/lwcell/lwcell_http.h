/**
 * \file            lwcell_http.h
 * \brief           HTTP API
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
#ifndef LWCELL_HTTP_HDR_H
#define LWCELL_HTTP_HDR_H

#include "lwcell/lwcell_types.h"
#include "lwcell/lwcell_netconn.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \ingroup         LWCELL
 * \defgroup        LWCELL_HTTP Hyper Text Transfer Protocol
 * \brief           Hyper Text Transfer Protocol (HTTP) manager
 * \{
 *
 * Currently it is under development
 *
 */


typedef struct lwcell_netconn* lwcell_httpconn_p;


#define LWCELL_HTTP_RECEIVE_NO_WAIT 0xFFFFFFFF

#define LWCELL_NETCONN_FLAG_FLUSH      ((uint16_t)0x0001) /*!< Immediate flush after netconn write */



typedef enum {
    LWCELL_HTTPCONN_TYPE_HTTP = LWCELL_CONN_TYPE_HTTP,
    LWCELL_HTTPCONN_TYPE_HTTPS = LWCELL_CONN_TYPE_HTTPS,
}lwcell_httpconn_type_t;


lwcellr_t lwcell_http_request_attach(void);
lwcellr_t lwcell_http_request_detach(void);


lwcell_httpconn_p lwcell_http_new(lwcell_httpconn_type_t type);
lwcellr_t lwcell_http_delete(lwcell_httpconn_p nc);
lwcellr_t lwcell_http_connect(lwcell_httpconn_p nc, const char* host, lwcell_port_t port);
lwcellr_t lwcell_http_receive(lwcell_httpconn_p nc, lwcell_pbuf_p* pbuf);
lwcellr_t lwcell_http_close(lwcell_httpconn_p nc);
int8_t lwcell_http_getconnnum(lwcell_httpconn_p nc);
void lwcell_http_set_receive_timeout(lwcell_httpconn_p nc, uint32_t timeout);
uint32_t lwcell_http_get_receive_timeout(lwcell_httpconn_p nc);


lwcellr_t lwcell_http_write(lwcell_httpconn_p nc, const void* data, size_t btw);
lwcellr_t lwcell_http_write_ex(lwcell_httpconn_p nc, const void* data, size_t btw, uint16_t flags);
lwcellr_t lwcell_http_flush(lwcell_httpconn_p nc);



/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LWCELL_HTTP_HDR_H */
