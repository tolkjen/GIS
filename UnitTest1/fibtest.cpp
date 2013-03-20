#include "stdafx.h"
#include "CppUnitTest.h"
#include "FibHeap.h"
#include <random>
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest1 
{		
	TEST_CLASS(FibHeapTest)
	{
	public:
		TEST_METHOD(EmptyHeap)
		{
			FibHeap<int, int> heap;
			Assert::AreEqual(heap.empty(), true);
		}

		TEST_METHOD(NonEmptyHeap) {
			FibHeap<int, int> heap;
			heap.insert(0, 0);
			Assert::AreEqual(heap.empty(), false);
		}

		TEST_METHOD(ClearEmptyHeap) {
			FibHeap<int, int> heap;
			heap.clear();
			Assert::AreEqual(heap.empty(), true);
		}

		TEST_METHOD(ClearNonEmptyHeap) {
			FibHeap<int, int> heap;
			heap.insert(5, 8);
			heap.clear();
			Assert::AreEqual(heap.empty(), true);
		}

		TEST_METHOD(RemoveIncreasingItems) {
			FibHeap<int, int> heap;
			for (int i=0; i<10000; i++) {
				heap.insert(i, i);
			}
			Assert::AreEqual(checkRemovingOrder(heap), true);
		}

		TEST_METHOD(RemoveDecreasingItems) {
			FibHeap<int, int> heap;
			for (int i=0; i<10000; i++) {
				heap.insert(1000-i, 1000-i);
			}
			Assert::AreEqual(checkRemovingOrder(heap), true);
		}

		TEST_METHOD(RemoveRandomItems) {
			uniform_int_distribution<int> dist(0, 1000);
			random_device random;
			mt19937 rng(random());

			FibHeap<int, int> heap;
			for (int i=0; i<10000; i++) {
				heap.insert(i, dist(rng));
			}
			Assert::AreEqual(checkRemovingOrder(heap), true);
		}

		TEST_METHOD(DecreaseCaseOne) {
			FibHeap<int, int> heap;
			FibHeap<int, int>::TreeNode* nodes[10000];
			for (int i=0; i<10000; i++) {
				nodes[i] = heap.insert(i, i);
			}
			for (int i=0; i<10000; i++) {
				heap.decreaseKey(nodes[i], nodes[i]->key() - 1);
			}
			Assert::AreEqual(checkRemovingOrder(heap), true);
		}

		TEST_METHOD(DecreaseCaseTwo) {
			FibHeap<int, int> heap;
			FibHeap<int, int>::TreeNode* nodes[10000];
			for (int i=0; i<10000; i++) {
				nodes[i] = heap.insert(i, i);
			}
			heap.removeMin();
			for (int i=1; i<10000; i++) {
				heap.decreaseKey(nodes[i], nodes[i]->key() - 1);
			}
			Assert::AreEqual(checkRemovingOrder(heap), true);
		}

		TEST_METHOD(DecreaseCaseThree) {
			FibHeap<int, int> heap;
			FibHeap<int, int>::TreeNode* nodes[10000];
			for (int i=0; i<10000; i++) {
				nodes[i] = heap.insert(i, i);
			}
			bool ok = true;
			int previous = heap.min().value();
			for (int i=0; i<4000; i++) {
				heap.decreaseKey(nodes[10000-i-1], nodes[10000-i-1]->key() - 1);
				if (previous <= heap.min().value()) {
					previous = heap.min().value();
				} else {
					ok = false;
					break;
				}
				heap.removeMin();
			}
			Assert::AreEqual(ok, true);
		}

	private:
		template <typename Value, typename Key>
		bool checkRemovingOrder(FibHeap<Value, Key> &heap) {
			int previous = heap.min().key();
			while (!heap.empty()) {
				if (previous <= heap.min().key()) {
					previous = heap.min().key();
				} else {
					return false;
				}
				heap.removeMin();
			}
			return true;
		}
	};
}