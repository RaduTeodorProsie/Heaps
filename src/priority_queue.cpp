#include <algorithm>
#include <functional>
#include <memory>
#include <queue>
#include <vector>

template <typename T, typename Compare = std::less<T>> class PriorityQueue {
  std::priority_queue<T, std::vector<T>, Compare> pq;

  // O(1) swap, top, O(log(heap_size)) for pop and push, O(log(heap_size))
  // amortized for meld. (by small to large)

public:
  PriorityQueue() = default;

  void push(const T &val) { pq.push(val); }

  void pop() { pq.pop(); }

  const T &top() const { return pq.top(); }

  [[nodiscard]] bool empty() const { return pq.empty(); }

  [[nodiscard]] size_t size() const { return pq.size(); }

  void swap(PriorityQueue &other) noexcept {
    if (this == &other)
      return;
    std::swap(pq, other.pq);
  }

  void meld(PriorityQueue &other) {

    if (size() < other.size())
      swap(other);

    while (!other.empty()) {
      push(other.top());
      other.pop();
    }
  }
};
