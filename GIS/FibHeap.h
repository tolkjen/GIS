// --------------------------------------------------------------------------
// FibHeap.h
// Piotr T¹kiel 2013
// 
// Structure implements Fibonacci heap. This data structure allows very fast 
// item insertions O(1), minimum item retrieval O(1), decreasing item key 
// O(1) and fast minimum item deletion O(log n).
// --------------------------------------------------------------------------

#ifndef FIBHEAP_H
#define FIBHEAP_H

#include <cmath>
#include <algorithm>

// --------------------------------------------------------------------------
// DEFINITIONS
// --------------------------------------------------------------------------

template <typename Value, typename Key>
class FibHeap
{
public:
	class TreeNode {
	friend class FibHeap;
	public:
		TreeNode();
		TreeNode(Value val, Key k);
		inline Value value() { return _value; }
		inline Key key() { return _key; }

	private:
		TreeNode *parent, *child, *next, *prev;
		Value _value;
		Key _key;
		int degree;
		bool marked;
	};

	FibHeap(void);
	~FibHeap(void);

// 	-------------------------------------------------------------------------
// 	insert
// 
// 	Inserts a pair of value and a key into the structure. Ordering is done 
// 	based on the key value. Returns a pointer to the added pair.
	TreeNode* insert(Value value, Key key);

// 	------------------------------------------------------------------------
// 	clear
// 
// 	Removes all items from the structure.
	void clear();

// 	------------------------------------------------------------------------
// 	decreaseKey
// 
// 	Decreases a value of the item's key and rearranges the structure so that 
// 	the ordering property is kept. Argument 'key' means new key value, which 
// 	MUST be less or equal than the previous.
	void decreaseKey(TreeNode *node, Key key);

// 	------------------------------------------------------------------------
// 	empty 
// 
// 	Checks if the structure is empty.
	inline bool empty() const;

// 	------------------------------------------------------------------------
// 	min
// 
// 	Returns an object holding a pair of the lowest key in the heap and it's 
// 	corresponding value.
	TreeNode min() const;

// 	------------------------------------------------------------------------
// 	removeMin
// 
// 	Removes the element from the heap, which has the smallest key value.
	void removeMin();

// 	------------------------------------------------------------------------
// 	size
// 
// 	Returns the number of elements in the structure.
	int size() const;

private:
	inline void cleanup();
	void deleteList(TreeNode *node);
	inline int log2(int x);
	inline void mergeDupes(TreeNode *rootNode, TreeNode **tab);
	void promote(TreeNode *node);

	TreeNode *_minPtr;
	int _size, _rootCount;
};

// --------------------------------------------------------------------------
// IMPLEMENTATION
// --------------------------------------------------------------------------

template <typename Value, typename Key>
FibHeap<Value, Key>::TreeNode::TreeNode() :
	parent(NULL), child(NULL), next(NULL), prev(NULL), degree(0), marked(false)
{
}

template <typename Value, typename Key>
FibHeap<Value, Key>::TreeNode::TreeNode(Value val, Key k) :
	parent(NULL), child(NULL), next(NULL), prev(NULL), _value(val), _key(k), degree(0), marked(false)
{
}

template <typename Value, typename Key>
FibHeap<Value, Key>::FibHeap( void ) :
	_minPtr(NULL), _size(0), _rootCount(0)
{
}

template <typename Value, typename Key>
FibHeap<Value, Key>::~FibHeap( void ) {
	if (_minPtr != NULL) {
		deleteList(_minPtr);
	}
}

template <typename Value, typename Key>
typename FibHeap<Value, Key>::TreeNode* FibHeap<Value, Key>::insert( Value value, Key key ) {
	TreeNode *node = new TreeNode(value, key);
	if (_minPtr == NULL) {
		_minPtr = node;
		node->next = node;
		node->prev = node;
	} else {
		node->next = _minPtr->next;
		node->prev = _minPtr;
		_minPtr->next->prev = node;
		_minPtr->next = node;
		if ( key < _minPtr->_key ) {
			_minPtr = node;
		}
	}
	_size++;
	_rootCount++;
	return node;
}

template <typename Value, typename Key>
void FibHeap<Value, Key>::clear() {
	if (_minPtr != NULL) {
		deleteList(_minPtr);
		_minPtr = NULL;
		_size = 0;
		_rootCount = 0;
	}
}

template <typename Value, typename Key>
void FibHeap<Value, Key>::decreaseKey( TreeNode *node, Key key ) {
	node->_key = key;
	promote(node);
	// update minimum value pointer if we should
	if (key < _minPtr->_key) {
		_minPtr = node;
	}
}

template <typename Value, typename Key>
bool FibHeap<Value, Key>::empty() const {
	return (_size == 0);
}

template <typename Value, typename Key>
typename FibHeap<Value, Key>::TreeNode FibHeap<Value, Key>::min() const {
	return *_minPtr;
}

template <typename Value, typename Key>
void FibHeap<Value, Key>::removeMin() {
	if (_minPtr == NULL) {
		return;
	}

	TreeNode *destroyer = _minPtr;
	// if root has children
	if (_minPtr->child != NULL) {
		// if there are other roots
		if (_minPtr->next != _minPtr) {
			_rootCount += _minPtr->degree;
			_minPtr->prev->next = _minPtr->child->next;
			_minPtr->next->prev = _minPtr->child;
			_minPtr->child->next->prev = _minPtr->prev;
			_minPtr->child->next = _minPtr->next;
			_minPtr = _minPtr->child;
		} else {
			_rootCount += _minPtr->degree;
			_minPtr = _minPtr->child;
		}
	} else {
		if (_minPtr->next != _minPtr) {
			_minPtr->next->prev = _minPtr->prev;
			_minPtr->prev->next = _minPtr->next;
			_minPtr = _minPtr->next;
		} else {
			_minPtr = NULL;
		}
	}
	delete destroyer;
	_size--;
	_rootCount--;

	cleanup();
}

template <typename Value, typename Key>
int FibHeap<Value, Key>::size() const {
	return _size;
}

template <typename Value, typename Key>
void FibHeap<Value, Key>::cleanup() {
	if (_size == 0) {
		return;
	}

	int tableSize = log2(_size) + 1;
	TreeNode **tab = new TreeNode*[tableSize];
	for (int i = 0; i < tableSize; i++) {
		tab[i] = NULL;
	}

	TreeNode *walker = _minPtr;
	int rootsUntouched = _rootCount;
	for (int i = 0; i < rootsUntouched; i++) {
		walker->parent = NULL;
		if (walker->_key < _minPtr->_key) {
			_minPtr = walker;
		}
		walker = walker->next;
		mergeDupes(walker->prev, tab);
	}

	delete [] tab;
}

template <typename Value, typename Key>
void FibHeap<Value, Key>::mergeDupes( TreeNode *rootNode, TreeNode **tab ) {
	TreeNode *linkedNode = tab[rootNode->degree];
	while (linkedNode != NULL) {
		tab[rootNode->degree] = NULL;
		if (linkedNode->_key < rootNode->_key || linkedNode == _minPtr) {
			std::swap(linkedNode, rootNode);
		}
		// remove from the root list
		linkedNode->next->prev = linkedNode->prev;
		linkedNode->prev->next = linkedNode->next;
		_rootCount--;
		// add linkedNode as a child to rootNode
		if (rootNode->child != NULL) {
			linkedNode->next = rootNode->child->next;
			linkedNode->prev = rootNode->child;
			rootNode->child->next->prev = linkedNode;
			rootNode->child->next = linkedNode;
		} else {
			rootNode->child = linkedNode;
			linkedNode->next = linkedNode;
			linkedNode->prev = linkedNode;
		}
		rootNode->degree++;
		linkedNode->parent = rootNode;
		// pick another node for linking
		linkedNode = tab[rootNode->degree];
	}
	tab[rootNode->degree] = rootNode;
}

template <typename Value, typename Key>
void FibHeap<Value, Key>::deleteList( TreeNode *node ) {
	TreeNode *walker, *destroyer;
	walker = node;
	do {
		if (walker->child != NULL) {
			deleteList(walker->child);
		}
		destroyer = walker;
		walker = walker->next;
		delete destroyer;
	} while (walker != node);
}

#define INV_LOGE2 1.44269504088896340736

template <typename Value, typename Key>
int FibHeap<Value, Key>::log2(int x) {
	return (int) ceil(log(x) * INV_LOGE2);
}

template <typename Value, typename Key>
void FibHeap<Value, Key>::promote( TreeNode *node ) {
	node->marked = false;
	if (node->parent != NULL) {
		// remove from parent's children.
		// if node is the only child.
		if (node->parent->degree == 1) {
			node->parent->child = NULL;
		} else {
			node->parent->child = node->prev;
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
		// add node to the root list
		node->next = _minPtr->next;
		node->prev = _minPtr;
		_minPtr->next->prev = node;
		_minPtr->next = node;
		// mark or promote parent
		if (node->parent->marked) {
			promote(node->parent);
		} else {
			node->parent->marked = true;
		}
		// keep integral values
		node->parent->degree--;
		node->parent = NULL;
		_rootCount++;
	}
}

#endif
