#include <stdexcept>
#include <vector>

template <typename T, typename Compare = std::greater<T>> class Quaternary {
  std::vector<T> heap;
  Compare comp;

  // O(1) swap, top, O(log(heap_size)) for pop and push, O(log(heap_size))
  // amortized for meld. (by small to large)

public:
  [[nodiscard]] size_t size() const { return heap.size(); }
  [[nodiscard]] bool empty() const { return heap.empty(); }

  void push(const T &val) {
    heap.push_back(val);
    size_t i = heap.size() - 1;
    while (i > 0) {
      size_t p = (i - 1) >> 2;
      if (!comp(heap[i], heap[p]))
        break;
      std::swap(heap[i], heap[p]);
      i = p;
    }
  }

  const T &top() const {
    if (heap.empty())
      throw std::out_of_range("top on empty");
    return heap[0];
  }

  void pop() {
    if (heap.empty())
      throw std::out_of_range("empty sequence");

    std::swap(heap[0], heap[heap.size() - 1]);
    heap.pop_back();

    for (size_t i = 0;;) {
      size_t best = i;
      for (int k = 1; k <= 4; k++) {
        size_t c = (i << 2) | k;
        if (c < heap.size() && comp(heap[c], heap[best]))
          best = c;
      }

      if (best == i)
        return;
      std::swap(heap[i], heap[best]);
      i = best;
    }
  }

  void swap(Quaternary &other) noexcept {
    std::swap(heap, other.heap);
    std::swap(comp, other.comp);
  }

  void meld(Quaternary<T> &other) {
    if (other.size() > size())
      swap(other);

    for (const T &val : other.heap) {
      push(val);
    }
  }
};