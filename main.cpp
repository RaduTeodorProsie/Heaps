#include <iostream>
#include "src/quaternary.cpp"
#include "src/Rheap.cpp"

int main() {
    Rheap<int> heap; heap.push(1);
    heap.push(2); std::cout << heap.top() << std::endl;
    Rheap<int> heap2; heap2.push(2); std::cout << heap2.top() << std::endl;
    heap.meld(heap2); std::cout << heap.top();
}