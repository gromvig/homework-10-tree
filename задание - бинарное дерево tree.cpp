#include "tree.h"
#include "queue.h"
/// Добавление элемента в дерево. Вспомогательная (внутренняя) функция.
bool _add_to_tree(node*& root, rec* value);
bool _add_to_tree1(node*& root, rec* value);
/// Удаление элемента из дерева. Вспомогательная (внутренняя) функция.
bool _remove_from_tree(node*& root, rec* value);
/// Поиск элемента в дереве. Вспомогательная (внутренняя) функция.
int _find_in_tree(const node* root, rec* value);
/// Удаление дерева. Вспомогательная (внутренняя) функция.
void _drop_tree(node*& root);
/// Перемещение значений из самого правого узла левого поддерева,
/// либо самого левого узла правого поддерева в узел, значение которого
/// нужно удалить.
void _move_node(node* rem);
/// Находит предка для элемента, являющегося ближайшим к удаляемому из дерева.
node* _find_prev_nearest(node* removing);
/// Определяет, с левым ли поддеревом необходимо работать при удалении элемента
inline bool _is_left(const node* n);
/// Копирование значений основных информационных полей из одного узла в другой
inline void _copy_value(node* to, const node* from);
/// Функция для удаления узла дерева
inline void _del(node*& n);
/// Перечисление возможных видов обхода дерева вглубь. Вспомогательный элемент.
enum _traverse_type { infix, prefix, postfix };
/// Функция обхода дерева вглубь. Вспомогательная (внутренняя) функция
void _traverse(queue& q, node* root, _traverse_type type);
/// Функция обхода дерева вширь
void _wide_traverse(queue& q, node*& root, bool include_empty);
/// Безопасное получение высоты поддерева для заданного узла и
/// сохранение (обновление) информации в узле
int _get_height(node* n); 
/// Функция балансировки дерева или его части
void _balance(node*& n);
/// Функция выполнения малого левого поворота
void _small_left_rotate(node*& n);
/// Функция выполнения малого правого поворота
void _small_right_rotate(node*& n);
/// Функция выполнения большого левого поворота
void _big_left_rotate(node*& n);
/// Функция выполнения болоьшого правого поворота
void _big_right_rotate(node*& n);
//----------------------------------------------------------------
void add_to_tree(tree& t, rec* value,int mode)
{
	if(mode == 1)
	{
		if (_add_to_tree(t.root, value))
			// Если добавился новый элемент, увеличиваем счетчик количества элементов дерева
			t.elem_count++;
	}
	else {
		if (_add_to_tree1(t.root, value))
			// Если добавился новый элемент, увеличиваем счетчик количества элементов дерева
			t.elem_count++;
	}
}
bool remove_from_tree(tree& t, rec* value)
{
	auto res = _remove_from_tree(t.root, value);
	if (res)
		// Если элемент был удален из дерева, уменьшаем счетчик элементов
		t.elem_count--;
	return res;
}
int find_in_tree(tree t, rec* value)
{
	return _find_in_tree(t.root, value);
}
void drop_tree(tree& t)
{
	_drop_tree(t.root);
	// В дереве больше нет элементов
	t.elem_count = 0;
}
bool _add_to_tree(node*& root, rec* value)
{
	// Проверяем, существует ли дерево
	if (root)
	{
		// Дерево уже существует. Определяем, куда разместить новый элемент
		// В зависимости от значения нового элемента относительного текущего…
		auto& branch =
			value->fio.compare(root->value->fio)  < 0 ? root->left : root->right;
		// …рекурсивно размещаем элемент в левом или правом поддереве
		bool r = _add_to_tree(branch, value);
		// Выполнение балансировки дерева после добавления нового узла
		// Пересчет высоты узлов внесен в функцию балансировки
		if (r) _balance(root);
		return r;
	}
		// Если дерева нет, то новый элемент становится корневым
		// Создаем новый узел для размещения в дереве
		node* newnode = new node;
	newnode->value = value;
	// И помещаем в качестве текущего (корневого) узла
	root = newnode;
	return true;
}
bool _add_to_tree1(node*& root, rec* value)
{
	// Проверяем, существует ли дерево
	if (root)
	{
		// Дерево уже существует. Определяем, куда разместить новый элемент
		if (value->score == root->value->score)
		{
			// Если значение в новом элементе совпадает со значением в текущем элементе дерева
			// Вместо добавления элемента, увеличиваем количество таких элементов
			root->count++;
			return false;
		}
		// В зависимости от значения нового элемента относительного текущего…
		auto& branch =
			value->score < root->value->score ? root->left : root->right;
		// …рекурсивно размещаем элемент в левом или правом поддереве
		bool r = _add_to_tree1(branch, value);
		// Выполнение балансировки дерева после добавления нового узла
		// Пересчет высоты узлов внесен в функцию балансировки
		if (r) _balance(root);
		return r;
	}
	// Если дерева нет, то новый элемент становится корневым
	// Создаем новый узел для размещения в дереве
	node* newnode = new node;
	newnode->value = value;
	// И помещаем в качестве текущего (корневого) узла
	root = newnode;
	return true;
}
bool _remove_from_tree(node*& root, rec* value)
{
	if (root) {
		if (value-> fio.compare(root->value->fio) == 0)
		{
			// Удаляемый элемент найден.
			if (!root->left && !root->right)
				// Если это лист, просто удаляем его.
				_del(root);
			else
				// Иначе выполняем перенос
				_move_node(root);
			return true;
		}
		// Выполняем рекурсивный поиск удаляемого элемента в левом или правом поддереве
		auto& subtree = value-> fio.compare(root->value->fio) < 0 ? root->left : root->right;
		bool r = _remove_from_tree(subtree, value);
		// Если элемент был удален, выполняем балансировку для корневого узла
		if (r) _balance(root);
		return r;
	}
	return false;
}
int _find_in_tree(const node* root, rec* value)
{
	// Если корень пуст, значит искомое значение не найдено, количество = 0
	if (!root) return 0;
	// Если значение имеется в корневом элементе, возвращаем количество
	if  (value->fio.compare(root->value->fio) == 0) return root->count;
	// Иначе продолжаем рекурсивный поиск в поддеревьях
	if (value->fio.compare(root->value->fio) < 0)
		return _find_in_tree(root->left, value);
	return _find_in_tree(root->right, value);
}
void _drop_tree(node*& root)
{
	if (root) // Если есть, что удалять
	{
		// Рекурсивно удаляется левое поддерево
		_drop_tree(root->left);
		// Рекурсивно удаляется правое поддерево
		_drop_tree(root->right);
		// Удаляется корневой элемент
		delete root;
		root = nullptr;
	}
}
void _move_node(node* rem)
{
	auto parent = _find_prev_nearest(rem);
	// Определяем, будем ли работать с левым поддеревом
	auto is_lft = _is_left(rem);
	// Если ближайший узел находится сразу за удаляемым,
	// инверсируем признак поддерева
	if (parent == rem) is_lft = !is_lft;
	// Находим элемент, ближайший к удаляемому.
	// Он справа от найденного предпоследнего узла в левом поддереве
	// и слева - в правом.
	node*& to_remove = is_lft ? parent->right : parent->left;
	// Если есть продолжение дерева после фактически удаляемого элемента
	node* child = nullptr;
	if (to_remove)
		child = is_lft ? to_remove->right : to_remove->left;
	// Копируем значения(!) из ближайшего к удаляемому узла в удаляемый узел
	// При этом сам удаляемый узел останется в памяти (с новым значением),
	// А снизу дерева будет удален лишний элемент
	_copy_value(rem, to_remove);
	// Выполняем удаление лишнего узла
	_del(to_remove);
	// Прицепляем остаток дерева к родительскому узлу
	to_remove = child;
	// Выполняем балансировку дерева относительно вершины parent
	_balance(parent);
}
node* _find_prev_nearest(node* rem)
{
	// Если удаляемого узла нет, сразу выходим
	if (!rem) {
		return rem;
	}
	// Устанавливаем родительским удаляемый элемент
	auto prev = rem;
	// Определяем с каким поддеревом работать
	if (_is_left(rem))
	{
		// Поиск в левом поддереве крайнего правого элемента
		// и его предка
		auto last = rem->left;
		while (last && last->right) {
			prev = last;
			last = last->right;
		}
	}
	else
	{
		// Поиск в правом поддереве крайнего левого элемента
		// и его предка
		auto last = rem->right;
		while (last && last->left) {
			prev = last;
			last = last->left;
		}
	}
	return prev;
}
inline bool _is_left(const node* n)
{
	auto lh = _get_height(n->left);
	auto rh = _get_height(n->right);
	return lh >= rh;
}
inline void _copy_value(node* to, const node* from)
{
	// Если оба узла корректные
	if (to && from)
	{
		// Выполняем копирование значений основных информационных полей
		to->value = from->value;
		to->count = from->count;
	}
}
inline void _del(node*& n)
{
	delete n;
	n = nullptr;
}
void _traverse(queue& q, node* root, _traverse_type type) {
	// Реализация обхода дерева вглубь
	if (root)
	{
		switch (type)
		{
		case infix: // Обход вида LNR
		// Рекурсивный вывод значений левого поддерева (L)
			if (root->left) _traverse(q, root->left, type);
			// Вывод значений узла (N)
			enqueue(q, root);
			// Рекурсивный вывод значений правого поддерева (R)
			if (root->right) _traverse(q, root->right, type);
			break;
		case prefix: // Обход вида NLR
		// Вывод значения узла (N)
			enqueue(q, root);
			// Рекурсивный вывод значений левого поддерева (L)
			if (root->left) _traverse(q, root->left, type);
			// Рекурсивный вывод значений правого поддерева (R)
			if (root->right) _traverse(q, root->right, type);
			break;
		case postfix: // Обход вида LRN
		// Рекурсивный вывод значений левого поддерева (L)
			if (root->left) _traverse(q, root->left, type);
			// Рекурсивный вывод значений правого поддерева (R)
			if (root->right) _traverse(q, root->right, type);
			// Вывод значения узла (N)
			enqueue(q, root);
		}
	}
}
queue prefix_traverse(tree t)
{
	queue q;
	_traverse(q, t.root, prefix);
	return q;
}
queue infix_traverse(tree t)
{
	queue q;
	_traverse(q, t.root, infix);
	return q;
}
queue postfix_traverse(tree t)
{
	queue q;
	_traverse(q, t.root, postfix);
	return q;
}
queue wide_traverse(tree t, bool include_empty)
{
	queue q;
	_wide_traverse(q, t.root, include_empty);
	return q;
}
void _wide_traverse(queue& queue, node*& r, bool include_empty)
{
	// Проверка того, что дерево существует
	if (!r) return;
	// Корень дерева добавляется в очередь
	enqueue(queue, r);
	// Получение указателя на начало очереди
	auto q = queue.head;
	// Вычисление количества максимального числа узлов для дерева данной высоты
	// (без учета последнего уровня)
	const auto cnt = 1 << (r->height - 1);
	// Производится проход по очереди
	for (int i = 1; i < cnt && q; i++)
	{
		// Получаем указатели на левый и правый подузлы для текущего
		auto ln = q->node ? q->node->left : nullptr;
		auto rn = q->node ? q->node->right : nullptr;
		// Добавляем узлы в очередь если они не пустые, либо
		// параметр include_empty позволяет сохранять там пустые значения.
		// (Последнее необходимо для организации вывода на экран структуры дерева)
		if (ln || include_empty) enqueue(queue, ln);
		if (rn || include_empty) enqueue(queue, rn);
		q = q->next;
	}
}
int _get_height(node* n)
{
	// Если узел пуст, сразу прерываем вычисления и возвращаем высоту = 0.
	if (!n) return 0;
	// Определяем сохраненные в структуру высоты левого и правого поддеревьев (если они существуют)
	auto lh = n->left ? n->left->height : 0;
	auto rh = n->right ? n->right->height : 0;
	// Высоту текущего узла рассчитываем как наибольшую из высот поддеревьев + 1
	return n->height = 1 + (lh > rh ? lh : rh);
}
void _balance(node*& n)
{
	// Проверяем, что переданный узел существует
	if (!n) return;
	// Определяем высоты левого и правого поддеревьев
	auto lh = _get_height(n->left);
	auto rh = _get_height(n->right);
	// Определяем разность высот двух поддеревьев
	auto dh = lh - rh;
	// Находим абсолютную величину разности
	dh = dh >= 0 ? dh : -dh;
	// Если разность высот 2 или более, требуется балансировка
	if (dh >= 2)
	{
		// Если правая ветвь длиннее, делаем левый поворот
		if (rh > lh)
		{
			// Определим высоты поддеревьев в правом поддереве
			auto rlh = _get_height(n->right->left);
			auto rrh = _get_height(n->right->right); 
				// Если правое под-поддерево больше, делаем малый поворот
				if (rrh > rlh) _small_left_rotate(n);
			// иначе - большой
				else _big_left_rotate(n);
		}
		// Будем делать правый поворот, если левая ветвь больше
		else
		{
			// Определим высоты поддеревьев в левом поддереве
			auto llh = _get_height(n->left->left);
			auto lrh = _get_height(n->left->right);
			// Если левое под-поддерево больше, делаем малый поворот
			if (llh > lrh) _small_right_rotate(n);
			// иначе - большой
			else _big_right_rotate(n);
		}
	}
	else
	{
		// Разбалансировки нет. Просто уточняем высоту текущего узла
		_get_height(n);
	}
}
void _small_left_rotate(node*& n)
{
	// Узел правого поддерева будет новым корнем
	node* new_root = n->right;
	// Указатель на правое поддерево смещаем на узел, который стоит слева от нового корня
	n->right = new_root->left;
	// Указатель на левое поддерево для нового корня устанавливаем на старый корень
	new_root->left = n;
	// Выполняем пересчет высоты для старого корня
	_get_height(n);
	// Выполняем фактический перенос корня
	n = new_root;
	// Выполняем пересчет высоты для нового корня
	_get_height(n);
}
void _small_right_rotate(node*& n)
{
	// Данная функция является полностью зеркальной относительно предыдущей
	// Узел левого поддерева будет новым корнем
	node* new_root = n->left;
	// Указатель на левое поддерево смещаем на узел, который стоит справа от нового корня
	n->left = new_root->right;
	// Указатель на правое поддерево для нового корня устанавливаем на старый корень
	new_root->right = n;
	// Выполняем пересчет высоты для старого корня
	_get_height(n);
	// Выполняем фактический перенос корня
	n = new_root;
	// Выполняем пересчет высоты для нового корня
	_get_height(n);
}
void _big_left_rotate(node*& n)
{
	_small_right_rotate(n->right);
	_small_left_rotate(n);
}
void _big_right_rotate(node*& n)
{
	// Данная функция является полностью зеркальной относительно предыдущей
	_small_left_rotate(n->left);
	_small_right_rotate(n);
}
