#pragma once
#include "tree_struct.h"
/// Добавление элемента в очередь
void enqueue(queue& queue, node* tree_node);
/// Извлечение элемента из очереди
bool dequeue(queue& queue, node*& tree_node);
/// Очистка очереди
void clear(queue& queue);
