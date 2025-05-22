#include <array>
#include <functional>
#include <memory>
#include <random>
#include <stdexcept>
#include <utility>

template <typename T, typename Compare = std::less<T>> class Rheap {
  class Node;
  using NodePtr = std::shared_ptr<Node>;

  // O(1) swap and top, O(log(heap_size)) all others (on average)

  class Node {
  public:
    T value;
    std::array<NodePtr, 2> child{};

    explicit Node(const T &val) : value(val) {}
  };

  NodePtr root = nullptr;
  std::mt19937 rng{std::random_device{}()};
  Compare comp{};

  NodePtr merge(NodePtr a, NodePtr b) {
    if (!a || !b)
      return a ? a : b;
    if (comp(a->value, b->value))
      std::swap(a, b);

    if (rng() & 1)
      std::swap(a->child[0], a->child[1]);
    a->child[0] = merge(a->child[0], b);
    return a;
  }

public:
  void push(const T &val) { root = merge(root, std::make_shared<Node>(val)); }

  const T &top() const {
    if (!root)
      throw std::out_of_range("Heap is empty");
    return root->value;
  }

  void pop() {
    if (!root)
      throw std::out_of_range("Heap is empty");
    root = merge(root->child[0], root->child[1]);
  }

  [[nodiscard]] bool empty() const { return !root; }

  void swap(Rheap &other) noexcept { std::swap(root, other.root); }

  void meld(Rheap &other) {
    root = merge(root, other.root);
    other.root.reset();
  }
};
