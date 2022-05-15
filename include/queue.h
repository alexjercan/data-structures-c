#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

struct queue;

struct queue *queue_new(size_t element_size);
void queue_copy(struct queue *dst, struct queue *src);

int queue_enqueue(struct queue *q, void *data);
int queue_dequeue(struct queue *q, void *data);
int queue_peek(struct queue *q, void *data);
int queue_empty(struct queue *q);
void queue_destroy(struct queue *q);

#endif // QUEUE_H
