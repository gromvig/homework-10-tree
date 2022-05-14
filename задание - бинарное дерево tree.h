#pragma once
#include "tree_struct.h"
/// Функция добавления элемента в дерево
void add_to_tree(tree& t, rec* value, int mode);
/// Функция удаления элемента из дерева
bool remove_from_tree(tree& tree, rec* value);
/// Функция поиска элемента в дереве
int find_in_tree(tree tree, rec* value);
/// Функция удаления дерева
void drop_tree(tree& tree);
/// Обход дерева вглубь. Префиксный вариант (NLR)
queue prefix_traverse(tree tree);
/// Обход дерева вглубь. Инфиксный вариант. (LNR)
queue infix_traverse(tree tree);
/// Обход дерева вглубь. Постфиксный вариант (LRN)
queue postfix_traverse(tree tree);
/// Обход дерева вширь
queue wide_traverse(tree tree, bool include_empty = true);
