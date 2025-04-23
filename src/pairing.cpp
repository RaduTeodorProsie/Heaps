#include <array>
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

template <typename T, typename Compare = std::less<T>> 
class PairingHeap {
  struct Node;
  using NodePtr = std::shared_ptr<Node>;

  // O(1) swap, top, meld, push(amortized),  O(log(heap_size)) for pop

  struct Node {
    T value;
    std::array<NodePtr, 2> ptr;
    explicit Node(const T &v) : value(v) { ptr[0] = ptr[1] = nullptr; }
  };

  NodePtr root;
  Compare comp;

  NodePtr meld(NodePtr a, NodePtr b) {
    if (!a)
      return b;
    if (!b)
      return a;
    if (comp(a->value, b->value))
      std::swap(a, b);
    b->ptr[1] = a->ptr[0];
    a->ptr[0] = b;
    return a;
  }

public:
  PairingHeap() : root(nullptr), comp(Compare()) {}

  void push(const T &val) { root = meld(root, std::make_shared<Node>(val)); }

  const T &top() const {
    if (!root)
      throw std::out_of_range("Heap is empty");
    return root->value;
  }

  void pop() {
    if (!root)
      throw std::out_of_range("Heap is empty");
    std::vector<NodePtr> trees;
    for (NodePtr curr = root->ptr[0]; curr; curr = curr->ptr[1]) {
      trees.push_back(curr);
    }
    root.reset();
    if (trees.empty())
      return;

    std::vector<NodePtr> merged;
    merged.reserve((trees.size() + 1) / 2);
    for (size_t i = 0; i + 1 < trees.size(); i += 2) {
      merged.push_back(meld(trees[i], trees[i + 1]));
    }
    if (trees.size() & 1) {
      merged.push_back(trees.back());
    }

    NodePtr h = merged.back();
    for (int i = static_cast<int>(merged.size()) - 2; i >= 0; --i) {
      h = meld(merged[i], h);
    }
    root = h;
  }
  void swap(PairingHeap&other) noexcept {
    std::swap(root, other.root);
  }

  [[nodiscard]] bool empty() const { return !root; }

  void meld(PairingHeap &other) {
    root = meld(root, other.root);
    other.root.reset();
  }
};
