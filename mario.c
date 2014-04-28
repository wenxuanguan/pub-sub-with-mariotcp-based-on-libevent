/*
 * mario.c
 *
 *  Ãû³Æ£ºMario
 *
 *  Date: 2013-3-1
 *  Author: feng jianhua (johan fong)
 *  Mail: 56683216@qq.com
 *
 *  ÐÞ¸Ä¼ÇÂ¼£º
 */

#include <stdio.h>
#include <stdlib.h>
#include "mario/mario.h"
#include "function.h"
#include "config.h"
#include "protocol.h"
#include <pthread.h>

int main(void) {

	//run_daemon();

	if (init_conf() < 0)
		return -1;
	SERVER *server = init_server(conf->port, conf->workernum, conf->connnum, conf->timeout,
			conf->timeout);
	if (server == NULL)
		return -1;

	regist_akg_func(CMD_FUNCTION_SUBSCRIBE, mysub);
	regist_akg_func(CMD_FUNCTION_PUBLISH, mypub);
	regist_disconnected_func(logout);

	if (start_log_thread() != 0) {
		fprintf(stderr, "[error] start_log_thread, errno: %d %m\n", errno);
		return -1;
	}
	regist_stx(0x8eb69af7);

	start_server((void*) server);
	return EXIT_SUCCESS;
}
