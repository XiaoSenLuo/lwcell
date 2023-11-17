/**
 * \file            lwcell_http.c
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
#include "lwcell/lwcell_http.h"
#include "lwcell/lwcell_conn.h"
#include "lwcell/lwcell_mem.h"
#include "lwcell/lwcell_private.h"

#if LWCELL_CFG_HTTP || __DOXYGEN__

#if !LWCELL_CFG_PROTOCOL
#error "LWCELL_CFG_PROTOCOL must be enabled for HTTP!"
#endif /* !LWCELL_CFG_CONN */

#if !LWCELL_CFG_CONN
#error "LWCELL_CFG_CONN must be enabled for HTTP!"
#endif /* !LWCELL_CFG_CONN */

#if LWCELL_CFG_NETCONN_RECEIVE_QUEUE_LEN < 2
#error "LWCELL_CFG_NETCONN_RECEIVE_QUEUE_LEN must be greater or equal to 2"
#endif /* LWCELL_CFG_NETCONN_RECEIVE_QUEUE_LEN < 2 */


static uint32_t network_counter = 0;

lwcellr_t lwcell_http_request_attach(void){
    lwcellr_t res = lwcellOK;
    uint8_t do_conn = 0;

    /* Check if we need to connect */
    lwcell_core_lock();
    if (network_counter == 0) {
        if (!lwcell_network_is_attached(LWCELL_PDP_APP_PROTOCOL)) {
            do_conn = 1;
        }
    }
    if (!do_conn) {
        ++network_counter;
    }
    lwcell_core_unlock();

    /* Connect to network */
    if (do_conn) {
        res = lwcell_network_attach(LWCELL_PDP_APP_PROTOCOL, NULL, NULL, NULL, NULL, NULL, 1);
        if (res == lwcellOK) {
            lwcell_core_lock();
            ++network_counter;
            lwcell_core_unlock();
        }
    }
    return res;
}

lwcellr_t lwcell_http_request_detach(void){
    lwcellr_t res = lwcellOK;
    uint8_t do_disconn = 0;

    /* Check if we need to disconnect */
    lwcell_core_lock();
    if (network_counter > 0) {
        if (network_counter == 1) {
            do_disconn = 1;
        } else {
            --network_counter;
        }
    }
    lwcell_core_unlock();

    /* Connect to network */
    if (do_disconn) {
        res = lwcell_network_detach(LWCELL_PDP_APP_PROTOCOL, NULL, NULL, 1);
        if (res == lwcellOK) {
            lwcell_core_lock();
            --network_counter;
            lwcell_core_unlock();
        }
    }
    return res;
}


lwcell_httpconn_p lwcell_http_new(lwcell_httpconn_type_t type){
    return lwcell_netconn_new((lwcell_netconn_type_t)type);
}

lwcellr_t lwcell_http_delete(lwcell_httpconn_p nc){
    return lwcell_netconn_delete(nc);
}

lwcellr_t lwcell_http_connect(lwcell_httpconn_p nc, const char* host, lwcell_port_t port){
    return lwcell_netconn_connect(nc, host, port);
}

lwcellr_t lwcell_http_receive(lwcell_httpconn_p nc, lwcell_pbuf_p* pbuf){

    return lwcell_netconn_receive(nc, pbuf);
}

lwcellr_t lwcell_http_close(lwcell_httpconn_p nc){
    return lwcell_netconn_close(nc);
}

int8_t lwcell_http_getconnnum(lwcell_httpconn_p nc){
    return lwcell_netconn_getconnnum(nc);
}

void lwcell_http_set_receive_timeout(lwcell_httpconn_p nc, uint32_t timeout){
    return lwcell_netconn_set_receive_timeout(nc, timeout);
}

uint32_t lwcell_http_get_receive_timeout(lwcell_httpconn_p nc){
    return lwcell_netconn_get_receive_timeout(nc);
}


lwcellr_t lwcell_http_write(lwcell_httpconn_p nc, const void* data, size_t btw){
    return lwcell_netconn_write(nc, data, btw);
}

lwcellr_t lwcell_http_write_ex(lwcell_httpconn_p nc, const void* data, size_t btw, uint16_t flags){
    return lwcell_netconn_write_ex(nc, data, btw, flags);
}

lwcellr_t lwcell_http_flush(lwcell_httpconn_p nc){
    return lwcell_netconn_flush(nc);
}



#endif /* LWCELL_CFG_HTTP || __DOXYGEN__ */
