#include <iostream>
#include "BinaryTree.h"
using namespace std;
int main() {
	BinaryTree<string> bt = BinaryTree<string>();
	string* s = new string("ANDREW");
	bt.Insert(s);
	s = new string("JOHN");
	bt.Insert(s);
	s = new string("ABBY");
	bt.Insert(s);
	s = new string("ABCY");
	bt.Insert(s);
	string* arr[50];
	bt.GetAllAscending(arr);
}