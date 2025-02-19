/* SPDX-License-Identifier: BSD-3-Clause */

#ifndef __SO_RINGBUFFER_H__
#define __SO_RINGBUFFER_H__

#include <sys/types.h>
#include <string.h>
#include <pthread.h>

typedef struct so_ring_buffer_t {
	char *data; //the buffer

	size_t read_pos; //tail
	size_t write_pos; //head

	/* TODO: Add syncronization primitives */
	// primitive de sincronizares
	pthread_cond_t  ring_empty;
	pthread_cond_t ring_full;

	pthread_mutex_t mutex;

	ssize_t nextPacketID;

	size_t len;
	size_t cap;
	int done;
} so_ring_buffer_t;

int     ring_buffer_init(so_ring_buffer_t *rb, size_t cap);
ssize_t ring_buffer_enqueue(so_ring_buffer_t *rb, void *data, size_t size);
ssize_t ring_buffer_dequeue(so_ring_buffer_t *rb, void *data, size_t size);
void    ring_buffer_destroy(so_ring_buffer_t *rb);
void    ring_buffer_stop(so_ring_buffer_t *rb);

#endif /* __SO_RINGBUFFER_H__ */
