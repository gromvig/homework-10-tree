#include "queue.h"
void enqueue(queue& queue, node* tree_node) {
	// Создаем новый элемент для размещения в очереди
	elem* newel = new elem;
	newel->node = tree_node;
	if (!queue.tail) {
		// Если очереди еще не было, новый элемент становится едиственным в ней
		queue.head = queue.tail = newel;
	}
	else {
		// Если очередь уже была, новый элемент помещается в конец:
		queue.tail->next = newel;
		queue.tail = newel;
	}
	queue.size++;
}
bool dequeue(queue& queue, node*& tree_node) {
	if (!queue.head) return false; // Очереди нет. Вернуть значение невозможно
	// Сохраняем возвращаемое значение
	tree_node = queue.head->node;
	// Сохраняем указатель на начало очереди
	elem* rem = queue.head;
	// Изменяем адрес головного элемента
	queue.head = queue.head->next;
	// Удаляем элемент с головы очереди
	delete rem;
	// Если элементов в очереди не осталось, обнуляем и указатель на ее конец.
	if (!queue.head) queue.tail = nullptr;
	queue.size--;
	return true;
}
void clear(queue& queue)
{
	// Проходим по всем элементам очереди, пока она не опустеет
	while (queue.head)
	{
		// Сохраняем ссылку на удаляемый элемент
		elem* rem = queue.head;
		// Переносим указатель вперед
		queue.head = queue.head->next;
		// Удаляем элемент по сохраненному указателю
		delete rem;
	}
	// Обнуляем размер очереди
	queue.size = 0;
	// Обновляем указатель на последний элемент
	queue.tail = nullptr;
}
