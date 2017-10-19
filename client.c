/*------------------------------------------------------------------------------
 *
 * Copyright (c) 2016-Present Pivotal Software, Inc
 *
 *------------------------------------------------------------------------------
 */
#include <stdlib.h>

#include "common/comm_channel.h"
#include "common/comm_utils.h"
#include "common/comm_connectivity.h"
#include "common/comm_server.h"
#include "rcall.h"

int main(int argc UNUSED, char **argv UNUSED) {
    int      sock;
    plcConn* conn;
    int      status;

	set_signal_handlers();

    // Bind the socket and start listening the port
    sock = start_listener();

    // Initialize R
    status = r_init();

#ifdef _DEBUG_CLIENT
    // In debug mode we have a cycle of connections with infinite wait time
    while (true) {
        conn = connection_init(sock);
		if (status == 0){
			receive_loop(handle_call, conn);
		} else {
			plc_raise_delayed_error(conn);
		}
    }
#else
    // In release mode we wait for incoming connection for limited time
    // and the client works for a single connection only
    connection_wait(sock);
    conn = connection_init(sock);
    if (status == 0){
        receive_loop(handle_call, conn);
    } else {
        plc_raise_delayed_error(conn);
    }

#endif

    lprintf(NOTICE, "Client has finished execution");
    return 0;
}



