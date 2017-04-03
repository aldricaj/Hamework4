#pragma once
#include <string>
using namespace std;
template <class T>
class BinaryTree {
private:
	static const int BALANCE_THRESHOLD = 1; // difference in Height between subnodes that causes a tree to be considered unbalenced
	int size; 
	

	class Node {
	public:
		

		T* value;
		Node* left;
		Node* right;
		Node* parent;
		Node(T* val, Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr);

		/*
			Returns ther number of nodes below this node in the tree

			The Total size of a Treee can be determined by: GetNumberOfSubNodes() + 1
		*/
		int GetNumberOfSubNodes();

		/*
			Returns the longest path to a leaf node from this node
		*/
		int GetHeight();

		/*
			Returns true if the Node represents a Leaf node which is
			defined as left and right are null pointers
		*/
		bool IsLeaf() {
			return left == nullptr && right == nullptr;
		}

		/*
			Returns the flat JSON(ish) string for debug purposes

			a leaf node would look like { value: `value as string`, left: NULL, right NULL }
		*/
		string GetJson();

		~Node() {
			if (left != nullptr) delete left;
			if (right != nullptr) delete right;
		}
	};
	Node* root;

protected:
	int GetBalance(Node* n);
	/*
		Returns whether or not the node needs 
		to be rebalanced and which branch is
		unbalanced (-1, 0, 1) for (left, balanced, right)
	*/
	int TreeUnbalanced(Node* n = nullptr);

	/*
		Rebalance the node through rotation
		[0] is the node to be rebalenced
		[1] is which side of the node is causing the unbalence
	*/
	void Rebalance(Node* n, int weight);
	/*
		Rotate the tree Left at pivot
	*/
	void RotateLeft(Node* parent, Node* pivot);

	/*
		Rotate the tree right at pivot
	*/
	void RotateRight(Node* parent, Node* pivot);


public:
	BinaryTree();

	/*
		Insert the item into the tree. Will rebalance if 
		the difference between the root node's left and 
		right side is greater than BALANCE_THRESHOLD  
	*/
	void Insert(T* item, Node* n = nullptr);

	/*
		Finds the item that matches the key passed
	*/
	T* Find(T* itemKey);

	/*
		Returns an array of all of the nodes in ascending order
	*/
	int GetAllAscending(T* arr[], Node* n = nullptr, int i = 0);

	/*
		Returns an array of all of the nodes in descending order
	*/
	int GetAllDescending(T* arr[], Node* n = nullptr, int i = 0);

	/*
		Empties the tree
	*/
	void EmptyTree();

	/*
		Removes the item matching the passed key and returns
		the pointer to the item to the calling function
	*/
	T* Remove(T* itemKey, Node* n = nullptr, Node* parent = nullptr);

	/*
		returns the size of the tree
	*/
	int Size() { return size; }


	~BinaryTree();
};
class DuplicateEntryException {};

/*
	Was in seperate .cpp but had linking issues so now it lives here
*/

// --------------------------------------------------------------------------
// Declarations for the Binary tree class declared in BinaryTree.h
// --------------------------------------------------------------------------
template <class T>
BinaryTree<T>::BinaryTree() {
	size = 0;
	root = nullptr;
}

template <class T>
void BinaryTree<T>::EmptyTree() {
	delete root;
	size = 0;
}

template <class T>
T* BinaryTree<T>::Find(T* item) {
	// iterate over all nodes until the item is found or
	// a leaf node is reached
	T* returnValue = nullptr;
	Node* currentNode = root;
	while (returnValue == nullptr && !currentNode.IsLeaf()) {
		// item is found
		if (returnValue->value == *item) {
			returnValue = currentNode->value;
		}
		else {
			// select next node to search
			if (*item < currentNode->Value) {
				currentNode = currentNode->left;
			}
			else {
				currentNode = currentNode->right;
			}
		}
	}
	return returnValue;
}

template <class T>
int BinaryTree<T>::GetAllAscending(T* arr[], Node* n = nullptr, int i = 0) {
	if (root == nullptr) {
		return 0;
	}
	if (n == nullptr) {
		n = root;
	}
	if (n->left != nullptr) {
		i = GetAllAscending(arr, n->left, i);
	}
	arr[i] = n->value;
	i++;
	if (n->right != nullptr) {
		i = GetAllAscending(arr, n->right, i);
	}
	return i;
}

template <class T>
int BinaryTree<T>::GetAllDescending(T* arr[], Node* n = nullptr, int i = 0) {
	if (root == nullptr) {
		return 0;
	}
	if (n == nullptr) {
		n = root;
	}
	if (n->right != nullptr) {
		i = GetAllDescending(arr, n->right, i);
	}
	arr[i] = n->value;
	i++;
	if (n->left != nullptr) {
		i = GetAllDescending(arr, n->left, i);
	}
	return i;
}

template <class T>
void BinaryTree<T>::Insert(T* item, Node* parent = nullptr) {
	if (parent == nullptr) {
		parent = root;
	}
	if (root == nullptr) { // this means that root is NULL
		root = new Node(item);
	}
	else {
		if (*item < *parent->value) {
			if (parent->left == nullptr) { // we have found a spot
				parent->left = new Node(item, parent);
			}
			else {
				Insert(item, parent->left);
				Rebalance(parent, GetBalance(parent));
			}
		}
		else if (*item == *parent->value) {
			throw DuplicateEntryException();
		}
		else { // the item > the parent->value
			if (parent->right == nullptr) {
				parent->right = new Node(item, parent);
			}
			else {
				Insert(item, parent->right);
				Rebalance(parent, GetBalance(parent));
			}
		}
	}
	size++;
}

template <class T>
T* BinaryTree<T>::Remove(T* item, Node* n = nullptr, Node* parent = nullptr) {
	T* deletedValue = nullptr;
	if (n == nullptr) {
		n = root;
	}
	if (*item < *n->value) {
		deletedValue = Remove(item, n->left, n);
		Rebalance(n, GetBalance(n));
	}
	else if (*item > *n->value) {
		deletedValue = Remove(item, n->right, n);
		Rebalance(n, GetBalance(n));
	}
	else { // item == parent->value aka we are deleting this node
		deletedValue = n->value;
		bool leftNotNull = n->left != nullptr;
		bool rightNotNull = n->right != nullptr;
		if (leftNotNull && rightNotNull) {
			Node* replacement = n->right;
			while (replacement->left != nullptr) {
				replacement = replacement->left;
			}
			n->value = replacement->value;

			// make sure we don't accidenlntly delete reassigned nodes
			replacement->left = nullptr;
			replacement->right = nullptr;
			delete replacement;
		}
		else if (leftNotNull) {
			parent->left = n->left;
			parent->left->parent = parent;
			n->left = nullptr;
			delete n;
		}
		else if (rightNotNull) {
			parent->right = n->right;
			parent->right->parent = parent;
			n->right = nullptr;
			delete n;
		}
		else {
			delete n;
		}
	}
	size--;
	return deletedValue;
}


template<class T>
inline void BinaryTree<T>::Rebalance(Node * n, int weight)
{
	if (weight < -1) { // left unbalanced
		RotateLeft(n, n->left);
	}
	if (weight > 1) {
		RotateRight(n, n->right);
	}
}

template <class T>
void BinaryTree<T>::RotateLeft(Node* parent, Node* pivot) {
	bool pivotLeft = parent->left == pivot;
	if (pivotLeft) {
		parent->left = pivot->right;
		parent->left->left = pivot;
	}
	else {
		parent->right = pivot;
		parent->right->left = pivot;
	}
}

template <class T>
void BinaryTree<T>::RotateRight(Node* parent, Node* pivot) {
	bool pivotRight = parent->right == pivot;
	if (pivotRight) {
		parent->right = pivot->left;
		parent->right->right = pivot;
	}
	else {
		parent->left = pivot;
		parent->left->right = pivot;
	}
}

template<class T>
inline int BinaryTree<T>::GetBalance(Node * n)
{
	return -n->left->GetHeight() + n->right->GetHeight();
}

template <class T>
int BinaryTree<T>::TreeUnbalanced(Node* n = nullptr) {
	if (n == nullptr) {
		n = root;
	}
	int balance = GetBalance(n);
	if (balance > 0) {
		balance = 1;
	}
	else if (balance < 0) {
		balance = -1;
	}
	return balance;
}

template <class T>
BinaryTree<T>::~BinaryTree() {
	if (root != nullptr) {
		EmptyTree();
	}
}

//------------------------------------------------------------------------
// Declarations for Node Sub-Class
//------------------------------------------------------------------------
template <class T>
BinaryTree<T>::Node::Node(T* val, Node* parent = nullptr, Node* right = nullptr, Node* left = nullptr) {
	value = val;
	this->left = left;
	this->right = right;
	this->parent = parent;
}
template <class T>
int BinaryTree<T>::Node::GetNumberOfSubNodes() {
	if (IsLeaf()) {
		return 0;
	}
	else {
		int leftSum = (left != nullptr) ? left->GetNumberOfSubNodes() + 1 : 0;
		int rightSum = (right != nullptr) ? right->GetNumberOfSubNodes() + 1 : 0;
		return leftSum + rightSum;
	}
}

template <class T>
int BinaryTree<T>::Node::GetHeight() {
	if (IsLeaf()) {
		return 1;
	}
	else {
		int leftHeight = 0;
		int rightHeight = 0;
		if (left != nullptr) {
			leftHeight = left->GetHeight();
		}
		if (right != nullptr) {
			rightHeight = right->GetHeight();
		}
		return ((leftHeight > rightHeight) ? leftHeight : rightHeight) + 1;
	}
}

template <class T>
string BinaryTree<T>::Node::GetJson() {
	string json = "{" +
		"value:" + value + "," +
		"left:" + (left != nullptr) ? left->GetJson() : "NIL" + "," +
		"right:" + (right != nullptr) ? right->GetJson() : "NIL" + "" +
		"}";
	return json;
}