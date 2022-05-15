#include "../include/queue.h"

#include <string.h>

struct node {
  void *data;
  struct node *next;
};

struct queue {
  struct node *head;
  struct node *tail;

  size_t element_size;
};

struct queue *queue_new(size_t element_size) {
  struct queue *q = malloc(sizeof(struct queue));

  q->head = NULL;
  q->tail = NULL;
  q->element_size = element_size;

  return q;
}

void queue_copy(struct queue *dst, struct queue *src) {
  struct node *n = src->head;

  dst->head = NULL;
  dst->tail = NULL;
  dst->element_size = src->element_size;

  while (n != NULL) {
    queue_enqueue(dst, n->data);
    n = n->next;
  }
}

int queue_enqueue(struct queue *q, void *data) {
  struct node *n = malloc(sizeof(struct node));
  n->next = NULL;
  n->data = malloc(q->element_size);
  memcpy(n->data, data, q->element_size);

  if (q->tail == NULL) {
    q->head = n;
    q->tail = n;
  } else {
    q->tail->next = n;
    q->tail = n;
  }

  return 0;
}

int queue_dequeue(struct queue *q, void *data) {
  struct node *n = q->head;

  if (n == NULL) {
    return -1;
  }

  q->head = n->next;

  if (q->head == NULL) {
    q->tail = NULL;
  }

  if (data != NULL) {
    memcpy(data, n->data, q->element_size);
  }

  free(n->data);
  free(n);

  return 0;
}

int queue_peek(struct queue *q, void *data) {
  if (q->head == NULL) {
    return -1;
  }

  memcpy(data, q->head->data, q->element_size);

  return 0;
}

int queue_empty(struct queue *q) { return q->head == NULL; }

void queue_destroy(struct queue *q) {
  struct node *n = q->head;

  while (n != NULL) {
    struct node *next = n->next;
    free(n->data);
    free(n);
    n = next;
  }
  free(q);
}
