#include "src/Quaternary.cpp"
#include "src/Rheap.cpp"
#include "src/pairing.cpp"
#include "src/priority_queue.cpp"
#include <iostream>

int main() {

  /*declarare : nume_heap<clasa, comparator (nu e obligatoriu)> nume
   *functii:
   *push, pop, top
   *heap1.swap(heap2) -> face swap intre cele doua heapuri
   *heap1.meld(heap2) -> insereaza heapul 2 in heapul 1. nu se garanteaza nimic
   *in legatura cu heap2 dupa aceasta operatie
   */

  PriorityQueue<int> heap;
  heap.push(1);
  heap.push(2);
  std::cout << heap.top() << std::endl;

  PriorityQueue<int> heap2;
  heap2.push(3);
  std::cout << heap2.top() << std::endl;

  heap2.swap(heap);
  heap2.meld(heap);
  heap.swap(heap2);

  std::cout << heap.top();

  Quaternary<int> quater;
  quater.push(3);

  Rheap<int> rheap;
  rheap.push(1);

  PairingHeap<int> pheap;
  pheap.push(3);
}