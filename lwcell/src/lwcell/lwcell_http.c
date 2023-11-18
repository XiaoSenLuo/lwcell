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

#define LWCELL_CFG_DBG_HTTP_CLIENT_TRACE         (LWCELL_CFG_DBG_HTTP_CLIENT | LWCELL_DBG_TYPE_TRACE)
#define LWCELL_CFG_DBG_HTTP_CLIENT_STATE         (LWCELL_CFG_DBG_HTTP_CLIENT | LWCELL_DBG_TYPE_STATE)
#define LWCELL_CFG_DBG_HTTP_CLIENT_TRACE_WARNING (LWCELL_CFG_DBG_HTTP_CLIENT | LWCELL_DBG_TYPE_TRACE | LWCELL_DBG_LVL_WARNING)
#define LWCELL_CFG_DBG_HTTP_CLIENT_TRACE_SEVERE  (LWCELL_CFG_DBG_HTTP_CLIENT | LWCELL_DBG_TYPE_TRACE | LWCELL_DBG_LVL_SEVERE)


typedef struct lwcell_http_client {
    lwcell_conn_p conn;

    char host[256];
    uint16_t port;

    lwcell_http_state_t conn_state;
    lwcell_http_conn_status_t connect_resp;

    lwcell_http_evt_t evt;
    lwcell_http_evt_fn evt_fn;

    lwcell_buff_t tx_buff;

    uint8_t is_sending;
    uint32_t sent_total;
    uint32_t written_total;

    lwcell_http_request_t requests[LWCELL_CFG_HTTP_MAX_REQUESTS];

    lwcell_sys_sem_t sync_sem;

    lwcell_sys_mutex_t mutex;
    uint8_t release_sem;

    lwcell_sys_mbox_t rcv_mbox;
    uint8_t* rx_buff;
    uint32_t rx_buff_len;

    uint32_t msg_rem_len;
    uint32_t msg_curr_pos;

    void* arg;
}lwcell_http_client_t;

static uint32_t network_counter = 0;

static uint8_t http_closed = 0xFF;

static void prv_release_sem(lwcell_http_client_p client){
    if(client->release_sem){
        client->release_sem = 0;
        lwcell_sys_sem_release(&client->sync_sem);
    }
}

static void prv_http_evt(lwcell_http_client_p client, lwcell_http_evt_t *evt){
    /// TODO: 处理 HTTP 事件
    switch(evt->type)
    {
        case LWCELL_HTTP_EVT_CONNECT:
        {
            break;
        }
        case LWCELL_HTTP_EVT_READ_BREAD:
        {
            break;
        }
        case LWCELL_HTTP_EVT_READ_COMPLETE:
        {
            break;
        }
        case LWCELL_HTTP_EVT_SEND_COMPLETE:
        {
            break;
        }
        case LWCELL_HTTP_EVT_KEEP_ALIVE:
        {
            break;
        }
        case LWCELL_HTTP_EVT_DISCONNECT:
        {
            break;
        }
        default: break;
    }
}

static void prv_http_evt_fn_default(lwcell_http_client_p client, lwcell_http_evt_t* evt){
    LWCELL_UNUSED(client);
    LWCELL_UNUSED(evt);
}


static lwcellr_t prv_http_conn_cb(lwcell_evt_t* evt){
    lwcell_conn_p conn;
    lwcell_http_client_p client;

    conn = lwcell_conn_get_from_evt(evt);

    if(conn != NULL){
        client = lwcell_conn_get_arg(conn);
        if(client == NULL){
            lwcell_conn_close(conn, 0);
            return lwcellERR;
        }
    }else if(evt->type != LWCELL_EVT_CONN_ERROR){
        return lwcellERR;
    }
    /// TODO: 处理 HTTP 接口事件
    switch (lwcell_evt_get_type(evt)){
        /*
         * Connection error. Connection to external
         * server was not successful
         */
        case LWCELL_EVT_CONN_ERROR: {
            lwcell_http_client_p client = NULL;
            if ((client = lwcell_evt_conn_error_get_arg(evt)) != NULL) {
                client->conn_state = LWCELL_HTTP_CONN_DISCONNECTED; /* Set back to disconnected state */
                /* Notify user upper layer */
                client->evt.type = LWCELL_HTTP_EVT_CONNECT;
                client->evt.evt.connect.status = LWCELL_HTTP_CONN_STATUS_TCP_FAILED; /* TCP connection failed */
                client->evt_fn(client, &client->evt); /* Notify upper layer about closed connection */
            }
            break;
        }
        case LWCELL_EVT_CONN_ACTIVE: {
            /// TODO: 处理 HTTP CLIENT 连接到服务器事件
            /// 对于 AIR724 HTTP 功能, 连接至服务器, 可以开始读数据或者写数据
//            prv_http_client_connected_cb(client);
            break;
        }
        case LWCELL_EVT_CONN_RECV: {
            /// TODO: 处理 HTTP CLIENT 接收到数据
            /// 对于 AIR724 HTTP Get 功能, 大文件下载时需要启用 Break
//            prv_http_client_data_recv_b(client,
//                                        lwcell_evt_conn_recv_get_buff(evt));
            break;
        }
            /* Data send event */
        case LWCELL_EVT_CONN_SEND: {

            break;
        }
            /* Periodic poll for connection */
        case LWCELL_EVT_CONN_POLL: {

            break;
        }
            /* Connection closed */
        case LWCELL_EVT_CONN_CLOSE: {

            break;
        }
        default: break;
    }
    return lwcellOK;
}

static lwcellr_t prv_http_client_connect(lwcell_http_client_p client, const char *host, lwcell_port_t port, lwcell_http_evt_fn evt_fn){
    lwcellr_t res = lwcellERR;

    LWCELL_ASSERT(client != NULL); /* t input parameters */
    LWCELL_ASSERT(host != NULL);
    LWCELL_ASSERT(port > 0);

    lwcell_core_lock();
    if (lwcell_network_is_attached(LWCELL_PDP_APP_PROTOCOL) && client->conn_state == LWCELL_HTTP_CONN_DISCONNECTED) {
        client->evt_fn = evt_fn != NULL ? evt_fn : prv_http_evt_fn_default;

        /* Start a new connection in non-blocking mode */
        if ((res = lwcell_conn_start(&client->conn, LWCELL_CONN_TYPE_HTTP, host, port, client, prv_http_conn_cb, 0))
            == lwcellOK) {
            client->conn_state = LWCELL_HTTP_CONN_CONNECTING;
        }
    }
    lwcell_core_unlock();
    return res;
}

static lwcellr_t prv_http_client_close(lwcell_http_client_p client){
    lwcellr_t res = lwcellERR;
    if (client->conn_state != LWCELL_HTTP_CONN_DISCONNECTED && client->conn_state != LWCELL_HTTP_CONN_DISCONNECTING) {

        res = lwcell_conn_close(client->conn, 0); /* Close the connection in non-blocking mode */
        if (res == lwcellOK) {
            client->conn_state = LWCELL_HTTP_CONN_DISCONNECTING;
        }
    }
    return res;
}

static lwcellr_t prv_http_client_disconnect(lwcell_http_client_p client){
    lwcellr_t res = lwcellERR;

    lwcell_core_lock();
    if (client->conn_state != LWCELL_HTTP_CONN_DISCONNECTED && client->conn_state != LWCELL_HTTP_CONN_DISCONNECTING) {
        res = prv_http_client_close(client); /* Close client connection */
    }
    lwcell_core_unlock();
    return res;
}


static uint8_t prv_http_client_is_connected(lwcell_http_client_p client){
    uint8_t res;
    lwcell_core_lock();
    res = LWCELL_U8(client->conn_state == LWCELL_HTTP_CONNECTED);
    lwcell_core_unlock();
    return res;
}

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


lwcell_http_client_p lwcell_http_client_new (lwcell_httpconn_type_t type, size_t tx_buff_len, size_t rx_buff_len) {
    lwcell_http_client_p client;

    if((client = lwcell_mem_calloc(1, sizeof(*client))) != NULL){
        client->conn_state = LWCELL_HTTP_CONN_DISCONNECTED;
        if(!lwcell_buff_init(&client->tx_buff, tx_buff_len)){
            goto free_client_section;
        }
        client->rx_buff_len = rx_buff_len;
        if((client->rx_buff = lwcell_mem_malloc(rx_buff_len)) == NULL){
            goto free_tx_buff_section;
        }
        if(!lwcell_sys_mbox_create(&client->rcv_mbox, LWCELL_CFG_HTTP_MBOX_SIZE)){
            goto free_rx_buff_section;
        }
        if(!lwcell_sys_sem_create(&client->sync_sem, 1)){
            goto free_rcv_sem_section;
        }
        if(!lwcell_sys_mutex_create(&client->mutex)){
            goto free_sync_sem_section;
        }

        return client;
    }else{
        return NULL;
    }

    free_sync_sem_section:
    lwcell_sys_sem_delete(&client->sync_sem);
    free_rcv_sem_section:
    lwcell_sys_sem_delete(&client->sync_sem);
    free_rx_buff_section:
    lwcell_mem_free(client->rx_buff);
    free_tx_buff_section:
    lwcell_buff_free(&client->tx_buff);
    free_client_section:
    lwcell_mem_free_s((void **)&client);
    return NULL;
}

lwcellr_t lwcell_http_client_delete(lwcell_http_client_p client){
    if (client == NULL) {
        return lwcellOK;
    }
    if (lwcell_sys_sem_isvalid(&client->sync_sem)) {
        lwcell_sys_sem_delete(&client->sync_sem);
        lwcell_sys_sem_invalid(&client->sync_sem);
    }
    if (lwcell_sys_mutex_isvalid(&client->mutex)) {
        lwcell_sys_mutex_delete(&client->mutex);
        lwcell_sys_mutex_invalid(&client->mutex);
    }
    if (lwcell_sys_mbox_isvalid(&client->rcv_mbox)) {
        void* d;
        while (lwcell_sys_mbox_getnow(&client->rcv_mbox, &d)) {
            if ((uint8_t*)d != (uint8_t*)&http_closed) {
                lwcell_mem_free_s((void **)&d);
            }
        }
        lwcell_sys_mbox_delete(&client->rcv_mbox);
        lwcell_sys_mbox_invalid(&client->rcv_mbox);
    }

    lwcell_mem_free_s((void**)&client);
    return lwcellOK;
}


lwcell_http_conn_status_t lwcell_http_client_connect(lwcell_http_client_p client, const char* host, lwcell_port_t port){
    if (client == NULL || host == NULL || !port) {
        LWCELL_DEBUGF(LWCELL_CFG_DBG_HTTP_CLIENT_TRACE_WARNING, "[HTTP CLIENT] Invalid parameters in function\r\n");
        return LWCELL_HTTP_CONN_STATUS_TCP_FAILED;
    }
    lwcell_sys_mutex_lock(&client->mutex);
    client->connect_resp = LWCELL_HTTP_CONN_STATUS_TCP_FAILED;
    lwcell_sys_sem_wait(&client->sync_sem, 0);
    client->release_sem = 1;

    if (prv_http_client_connect(client, host, port, prv_http_evt) == lwcellOK) {
        lwcell_sys_sem_wait(&client->sync_sem, 0);
    } else {
        LWCELL_DEBUGF(LWCELL_CFG_DBG_HTTP_CLIENT_TRACE_WARNING, "[HTTP CLIENT] Cannot connect to %s\r\n", host);
    }

    client->release_sem = 0;
    lwcell_sys_sem_release(&client->sync_sem);
    lwcell_sys_mutex_unlock(&client->mutex);
}

uint8_t lwcell_http_client_is_connected(lwcell_http_client_p client){
    uint8_t ret;

    if (client == NULL) {
        return 0;
    }

    lwcell_sys_mutex_lock(&client->mutex);
    ret = prv_http_client_is_connected(client);
    lwcell_sys_mutex_unlock(&client->mutex);
    return ret;
}

lwcellr_t lwcell_http_client_receive(lwcell_http_client_p client, lwcell_pbuf_p *pbuf, uint32_t timeout) {
    LWCELL_ASSERT(client != NULL);
    LWCELL_ASSERT(pbuf != NULL);

    *pbuf = NULL;

    /* Get new entry from mbox */
    if (timeout == 0) {
        if (!lwcell_sys_mbox_getnow(&client->rcv_mbox, (void**)pbuf)) {
            return lwcellTIMEOUT;
        }
    } else if (lwcell_sys_mbox_get(&client->rcv_mbox, (void**)pbuf, timeout) == LWCELL_SYS_TIMEOUT) {
        return lwcellTIMEOUT;
    }

    /* Check for MQTT closed event */
    if ((uint8_t*)(*pbuf) == (uint8_t*)&http_closed) {
        LWCELL_DEBUGF(LWCELL_CFG_DBG_HTTP_CLIENT_TRACE, "[HTTP CLIENT] Closed event received from queue\r\n");

        *pbuf = NULL;
        return lwcellCLOSED;
    }
    return lwcellOK;
}

lwcellr_t lwcell_http_client_close(lwcell_http_client_p client){
    lwcellr_t res = lwcellERR;

    LWCELL_ASSERT(client != NULL);

    lwcell_sys_mutex_lock(&client->mutex);
    lwcell_sys_sem_wait(&client->sync_sem, 0);
    client->release_sem = 1;
    if (prv_http_client_disconnect(client) == lwcellOK) {
        res = lwcellOK;
        lwcell_sys_sem_wait(&client->sync_sem, 0);
    } else {
        LWCELL_DEBUGF(LWCELL_CFG_DBG_HTTP_CLIENT_TRACE_WARNING, "[MQTT API] Cannot close API connection\r\n");
    }
    client->release_sem = 0;
    lwcell_sys_sem_release(&client->sync_sem);
    lwcell_sys_mutex_unlock(&client->mutex);
    return res;
}



lwcellr_t lwcell_http_client_write(lwcell_http_client_p client, const void* data, size_t btw){
    lwcellr_t res = lwcellERR;

    LWCELL_ASSERT(client != NULL);

    lwcell_sys_mutex_lock(&client->mutex);
    lwcell_sys_sem_wait(&client->sync_sem, 0);
    client->release_sem = 1;

    /// TODO: HTTP 发送数据
//    if(lwcell_mqtt_client_publish(client->mc, topic, data, LWCELL_U16(btw), qos, retain, NULL) == lwcellOK) {
//        lwcell_sys_sem_wait(&client->sync_sem, 0);
//        res = client->sub_pub_resp;
//    } else {
//        LWCELL_DEBUGF(LWCELL_CFG_DBG_MQTT_API_TRACE_WARNING, "[MQTT API] Cannot publish new packet\r\n");
//    }

    client->release_sem = 0;
    lwcell_sys_sem_release(&client->sync_sem);
    lwcell_sys_mutex_unlock(&client->mutex);

    return res;
}




#endif /* LWCELL_CFG_HTTP || __DOXYGEN__ */
