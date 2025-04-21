#include <stdexcept>
#include <vector>

template <typename T, typename Compare = std::greater<T>> class quaternary {
  std::vector<T> heap;
  Compare comp;

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

  T top() {
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

  static void swap(quaternary &a, quaternary &b) noexcept {
    using std::swap;
    swap(a.heap, b.heap);
    swap(a.comp, b.comp);
  }

  void meld(quaternary<T> &other) {
    if (other.size() > size())
      quaternary::swap(*this, other);

    for (const T &val : other.heap) {
      push(val);
    }
  }
};