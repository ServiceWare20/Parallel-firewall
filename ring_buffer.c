// SPDX-License-Identifier: BSD-3-Clause

#include "ring_buffer.h"
#include "utils.h"

int ring_buffer_init(so_ring_buffer_t *ring, size_t cap)
{
	/* TODO: implement ring_buffer_init */
	// cap = bytes
	if (cap == 0 || ring == NULL)
		return -1;

	ring->data = malloc(cap * sizeof(char));
	if (ring->data == NULL)
		return -1;

	ring->read_pos = 0;
	ring->write_pos = 0;
	ring->nextPacketID = 0;
	ring->len = 0;
	ring->cap = cap;
	ring->done = 0;

	if (pthread_mutex_init(&ring->mutex, NULL) != 0) {
		// it means it failed to init mutex
		free(ring->data);
		return -1;
	}

	if (pthread_cond_init(&ring->ring_empty, NULL) != 0) {
		pthread_mutex_destroy(&ring->mutex);
		free(ring->data);
		return -1;
	}

	if (pthread_cond_init(&ring->ring_full, NULL) != 0) {
		pthread_cond_destroy(&ring->ring_empty);
		pthread_mutex_destroy(&ring->mutex);
		free(ring->data);
		return -1;
	}

	return 0;
}

ssize_t ring_buffer_enqueue(so_ring_buffer_t *ring, void *data, size_t size)
{
	/* TODO: implement ring_buffer_enqueue */
	if (size > ring->cap)
		return -1;
	pthread_mutex_lock(&ring->mutex);

	while (size > ring->cap - ring->len)
		pthread_cond_wait(&ring->ring_full, &ring->mutex);

	char *sdata = data;

	size_t first_chunk = size < ring->cap - ring->write_pos ?
						size : ring->cap - ring->write_pos;

	memcpy(ring->data + ring->write_pos, sdata, first_chunk);
	memcpy(ring->data, sdata + first_chunk, size - first_chunk);

	// Updating the write pos
	ring->write_pos = (ring->write_pos + size) % ring->cap;
	ring->len += size;

	//printf("ENQUEUED\n");

	pthread_mutex_unlock(&ring->mutex);
	pthread_cond_signal(&ring->ring_empty);
	// 0 = success
	return size;
}

ssize_t ring_buffer_dequeue(so_ring_buffer_t *ring, void *data, size_t size)
{
	/* TODO: Implement ring_buffer_dequeue */
	pthread_mutex_lock(&ring->mutex);

	while (ring->len < size) {
		if (ring->done) {
			pthread_mutex_unlock(&ring->mutex);
			return -1;
		}
		pthread_cond_wait(&ring->ring_empty, &ring->mutex);
	}

	char *sdata = data;

	for (size_t i = 0; i < size; i++)
		sdata[i] = ring->data[(ring->read_pos + i) % ring->cap];

	// Updating the write pos
	ring->read_pos = (ring->read_pos + size) % ring->cap;
	ring->len -= size;
	ssize_t pid = ring->nextPacketID;

	ring->nextPacketID++;
	//printf("DEQUEUED\n");
	pthread_mutex_unlock(&ring->mutex);
	pthread_cond_signal(&ring->ring_full);

	return pid;
}

void ring_buffer_destroy(so_ring_buffer_t *ring)
{
	/* TODO: Implement ring_buffer_destroy */
	free(ring->data);
	pthread_mutex_destroy(&ring->mutex);
	pthread_cond_destroy(&ring->ring_empty);
	pthread_cond_destroy(&ring->ring_full);
}

void ring_buffer_stop(so_ring_buffer_t *ring)
{
	/* TODO: Implement ring_buffer_stop */
	if (ring == NULL)
		return;

	pthread_mutex_lock(&ring->mutex);
	ring->done = 1;
	pthread_cond_broadcast(&ring->ring_empty);
	pthread_cond_broadcast(&ring->ring_full);

	pthread_mutex_unlock(&ring->mutex);
}
