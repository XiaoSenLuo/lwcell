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

#include "lwcell/lwcell_includes.h"

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

struct lwcell_http_client;

typedef struct lwcell_http_client * lwcell_http_client_p;

typedef enum {
    LWCELL_HTTP_CONN_DISCONNECTED = 0x00,
    LWCELL_HTTP_CONN_CONNECTING,
    LWCELL_HTTP_CONN_DISCONNECTING,
    LWCELL_HTTP_CONNECTING,
    LWCELL_HTTP_CONNECTED,
}lwcell_http_state_t;


typedef struct {
    uint8_t method;
    uint8_t type;
    uint16_t status;

    void *arg;

    union {
        uint32_t post_sent_len;
        uint32_t get_recv_len;
        uint32_t head_recv_len;
    };

    uint32_t timeout_start_time;
}lwcell_http_request_t;


typedef enum {
    LWCELL_HTTP_EVT_CONNECT,
    LWCELL_HTTP_EVT_SEND_COMPLETE,
    LWCELL_HTTP_EVT_READ_BREAD,
    LWCELL_HTTP_EVT_READ_COMPLETE,
    LWCELL_HTTP_EVT_DISCONNECT,
    LWCELL_HTTP_EVT_KEEP_ALIVE,
}lwcell_http_evt_type_t;


typedef enum {
    LWCELL_HTTP_CONN_STATUS_ACCEPTED = 0x00,

    LWCELL_HTTP_CONN_STATUS_TCP_FAILED = 0x100,
}lwcell_http_conn_status_t;


typedef struct {
    lwcell_http_evt_type_t type;

    union {
        struct {
            lwcell_http_conn_status_t status;
        }connect;
        struct {
            uint8_t is_accepted;
        }disconnect;
        struct {
            void *arg;
            lwcellr_t res;
        }post;
        struct {
            void *arg;
            lwcellr_t res;
        }get;
        struct {
            void *arg;
            lwcellr_t res;
        }head;

        struct {
            uint8_t is_break;
            uint32_t body_len;
            uint32_t recv_len;
            const void *data;
        }recv;
    }evt;
}lwcell_http_evt_t;

typedef void (*lwcell_http_evt_fn)(lwcell_http_client_p client, lwcell_http_evt_t* evt);


typedef struct lwcell_netconn* lwcell_httpconn_p;


#define LWCELL_HTTP_RECEIVE_NO_WAIT 0xFFFFFFFF

#define LWCELL_NETCONN_FLAG_FLUSH      ((uint16_t)0x0001) /*!< Immediate flush after netconn write */



typedef enum {
    LWCELL_HTTPCONN_TYPE_HTTP = LWCELL_CONN_TYPE_HTTP,
    LWCELL_HTTPCONN_TYPE_HTTPS = LWCELL_CONN_TYPE_HTTPS,
}lwcell_httpconn_type_t;


lwcellr_t lwcell_http_request_attach(void);
lwcellr_t lwcell_http_request_detach(void);


lwcell_http_client_p lwcell_http_client_new (lwcell_httpconn_type_t type, size_t tx_buff_len, size_t rx_buff_len);
lwcellr_t lwcell_http_client_delete(lwcell_http_client_p client);
lwcell_http_conn_status_t lwcell_http_client_connect(lwcell_http_client_p client, const char* host, lwcell_port_t port);
uint8_t lwcell_http_client_is_connected(lwcell_http_client_p client);
lwcellr_t lwcell_http_client_receive (lwcell_http_client_p client, lwcell_pbuf_p *pbuf, uint32_t timeout);
lwcellr_t lwcell_http_client_close(lwcell_http_client_p client);

lwcellr_t lwcell_http_client_write(lwcell_http_client_p client, const void* data, size_t btw);


/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LWCELL_HTTP_HDR_H */
