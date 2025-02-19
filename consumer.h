/* SPDX-License-Identifier: BSD-3-Clause */

#ifndef __SO_CONSUMER_H__
#define __SO_CONSUMER_H__

#include "ring_buffer.h"
#include "packet.h"

typedef struct item {
	int isavl;
	pthread_cond_t cvar;
} item;


typedef struct buffer {
	pthread_mutex_t bfrMutex;
	 item items[256];
	 ssize_t nwp; //next written packet
	 pthread_cond_t nwp_cnd;
} buffer;

typedef struct so_consumer_ctx_t {
	struct so_ring_buffer_t *producer_rb;
	int out_fd;
    /* TODO: add synchronization primitives for timestamp ordering */
	buffer *bfr;
} so_consumer_ctx_t;

int create_consumers(pthread_t *tids,
					int num_consumers,
					so_ring_buffer_t *rb,
					const char *out_filename,
					int *out_fd_prll,
					buffer **bfr);

#endif /* __SO_CONSUMER_H__ */
