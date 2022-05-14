#include<iostream>
#include<string>
#include<fstream>
#include "queue.h"
#include "tree.h"
#include<windows.h>
using namespace std;
enum traverse_type { prefix, infix, postfix, wide };
/// Функция отображает на экране элементы из очереди, составленной из элементов дерева
void show_tree1(tree tree, traverse_type tt = wide, bool show_empty = true)
{
	// Формирование очереди для выполнения обхода определенного вида
	queue tq;
	switch (tt)
	{
	case prefix:tq = prefix_traverse(tree); break;
	case infix:tq = infix_traverse(tree); break;
	case postfix:tq = postfix_traverse(tree); break;
	case wide:tq = wide_traverse(tree, show_empty); break;
	}
	// Получение элемента с головы очереди для начала обхода
	auto got = tq.head;
	// Пока в очереди есть элементы
	while (got)
	{
		auto n = got->node;
		// Если элемент не пуст, выводится его значение
		// и в скобках количество таких значений в исходном наборе
		if (n) cout << n->value->fio << " " << n->value->score << endl;
		// Если элемент пуст, вместо его значения выводится прочерк
		else cout << "-";
		// Переходим к следующему по очереди элементу
		got = got->next;
		// Выводим символ-разделитель: пробел, если в очереди еще есть элементы
		// или символ перехода на новую строку, если элемент был последним.
		//cout << (got ? " " : "\n");
	}
	// Удаление вспомогательной очереди
	clear(tq);
}
void show_tree2(tree tree, traverse_type tt = wide, bool show_empty = true)
{
	// Формирование очереди для выполнения обхода определенного вида
	queue tq;
	switch (tt)
	{
	case prefix:tq = prefix_traverse(tree); break;
	case infix:tq = infix_traverse(tree); break;
	case postfix:tq = postfix_traverse(tree); break;
	case wide:tq = wide_traverse(tree, show_empty); break;
	}
	// Получение элемента с головы очереди для начала обхода
	auto got = tq.head;
	// Пока в очереди есть элементы
	while (got)
	{
		auto n = got->node;
		// Если элемент не пуст, выводится его значение
		// и в скобках количество таких значений в исходном наборе
		if (n) cout << n->value->score << "(" << n->count << ")" << endl;
		// Если элемент пуст, вместо его значения выводится прочерк
		else cout << "-";
		// Переходим к следующему по очереди элементу
		got = got->next;
		// Выводим символ-разделитель: пробел, если в очереди еще есть элементы
		// или символ перехода на новую строку, если элемент был последним.
		//cout << (got ? " " : "\n");
	}
	// Удаление вспомогательной очереди
	clear(tq);
}
void show_tree3(tree tree, string b, traverse_type tt = wide, bool show_empty = true)
{
	// Формирование очереди для выполнения обхода определенного вида
	queue tq;
	switch (tt)
	{
	case prefix:tq = prefix_traverse(tree); break;
	case infix:tq = infix_traverse(tree); break;
	case postfix:tq = postfix_traverse(tree); break;
	case wide:tq = wide_traverse(tree, show_empty); break;
	}
	// Получение элемента с головы очереди для начала обхода
	auto got = tq.head;
	// Пока в очереди есть элементы
	while (got)
	{
		auto n = got->node;
		// Если элемент не пуст, выводится его значение
		// и в скобках количество таких значений в исходном наборе
		if ((n) && (n->value->fio.find(b)) == 0)
			cout << n->value->score << endl;
		// Если элемент пуст, вместо его значения выводится прочерк
		//else cout << "-";
		// Переходим к следующему по очереди элементу
		got = got->next;
	}
	// Удаление вспомогательной очереди
	clear(tq);
}
void load_data(const string filename, tree& tree, int mode)
{
	ifstream f(filename);
	if (f.is_open()) {
		char* buf = new char[256];
		char* fio = new char[50];
		int score;
		while (f.getline(buf, 256))
		{
			sscanf_s(buf, "%49[^0-9] %d", fio, 50, &score);
			string s_fio(fio);
			rec* newel = new rec;
			newel->fio = s_fio;
			newel->score = score;
			add_to_tree(tree, newel, mode);
		}
	}
	else {
		cout << "ошибка открытия файла " << filename;
		exit(0);
	}

}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	tree tree;
	int a;
	cout << "Введите 1, если хотите построить дерево по фамилии\n"
		<< "Введите 2, если хотите построить дерево по среднему баллу\n"
		<< "Введите 3, если хотите вывести из дерева баллы всех студентов с фамилией на заданную букву\n";
	cin >> a;
	switch (a) {
	case(1):
		load_data("data.txt", tree, 1);
		show_tree1(tree, infix);
		drop_tree(tree);
		break;
	case(2):
		load_data("data.txt", tree, 2);
		show_tree2(tree, infix);
		drop_tree(tree);
		break;
	case(3):
		string b;
		cout << "Введите букву:\n";
		cin >> b;
		load_data("data.txt", tree, 1);
		show_tree3(tree, b, infix);
		drop_tree(tree);
		break;
	}
}