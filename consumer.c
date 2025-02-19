// SPDX-License-Identifier: BSD-3-Clause

#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

#include "consumer.h"
#include "ring_buffer.h"
#include "packet.h"
#include "utils.h"


void consumer_thread(so_consumer_ctx_t *ctx)
{
	//printf("SH\n");
	(void) ctx;
	/* TODO: implement consumer thread */
	char data[PKT_SZ], out_buf[PKT_SZ];
	int len;
	ssize_t pid;
	item *it;

	while (/*am date*/ true) {
		pid = ring_buffer_dequeue(ctx->producer_rb, data, PKT_SZ);
		if (pid < 0)
			break;

		struct so_packet_t *pkt = (struct so_packet_t *) data;

		int action = process_packet(pkt);
		unsigned long hash = packet_hash(pkt);
		unsigned long timestamp = pkt->hdr.timestamp;

		len = snprintf(out_buf, PKT_SZ, "%s %016lx %lu\n",
						RES_TO_STR(action), hash, timestamp);

		// sorting...
		it = &ctx->bfr->items[pid%256];
		pthread_mutex_lock(&ctx->bfr->bfrMutex);
		while (!it->isavl)
			pthread_cond_wait(&it->cvar, &ctx->bfr->bfrMutex);// 2

		it->isavl = 0;

		while (ctx->bfr->nwp != pid)
			pthread_cond_wait(&ctx->bfr->nwp_cnd, &ctx->bfr->bfrMutex);// 1

		write(ctx->out_fd, out_buf, len);
		ctx->bfr->nwp++;
		pthread_cond_broadcast(&ctx->bfr->nwp_cnd);// 1

		it->isavl = 1;
		pthread_cond_signal(&it->cvar);// 2

		pthread_mutex_unlock(&ctx->bfr->bfrMutex);
	}

	free(ctx); // free the malloc that was allocated in create_consumers
}

int create_consumers(pthread_t *tids,
					 int num_consumers,
					 struct so_ring_buffer_t *rb,
					 const char *out_filename,
					 int *out_fd_prll,
					 buffer **bfr)
{
	*out_fd_prll = open(out_filename, O_RDWR|O_CREAT|O_TRUNC, 0666);

	// Create and init bfr for sort
	*bfr = malloc(sizeof(buffer));
	buffer *ptr = *bfr;

	pthread_mutex_init(&ptr->bfrMutex, NULL);

	for (int i = 0; i < 256; i++) {
		pthread_cond_init(&ptr->items[i].cvar, NULL);
		ptr->items[i].isavl = 1;
	}
	ptr->nwp = 0;
	pthread_cond_init(&ptr->nwp_cnd, NULL);
	//--------------------------------------------------
	if (out_fd_prll == NULL) {
		fprintf(stderr, "file can not be open");
		return -1;
	}

	//printf("NC:%d\n",num_consumers);
	for (int i = 0; i < num_consumers; i++) {
		so_consumer_ctx_t *context = malloc(sizeof(so_consumer_ctx_t));

		context->bfr = ptr;
		context->producer_rb = rb;
		context->out_fd = *out_fd_prll;
		pthread_create(tids + i, NULL, (void *)consumer_thread, context);
	}
	return num_consumers;
}
