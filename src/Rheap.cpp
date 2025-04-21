#include <memory>
#include <array>
#include <random>
#include <stdexcept>
#include <utility>
#include <functional>  // for std::greater

template <typename T, typename Compare = std::less<T>>
class Rheap {
    class Node;
    using NodePtr = std::shared_ptr<Node>;

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
        if (!a || !b) return a ? a : b;
        if (comp(a->value, b->value))
            std::swap(a, b);

        NodePtr* cur = &a;
        while (true) {
            if (!(*cur)->child[0]) {
                (*cur)->child[0] = b;
                break;
            }

            if (rng() & 1)
                std::swap((*cur)->child[0], (*cur)->child[1]);

            cur = &(*cur)->child[0];
            if (comp((*cur)->value, b->value))
                std::swap(*cur, b);
        }

        return a;
    }

public:
    Rheap() = default;

    void push(const T &val) {
        root = merge(root, std::make_shared<Node>(val));
    }

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

    [[nodiscard]] bool empty() const {
        return !root;
    }

    void meld(Rheap &other) {
        if (this == &other) return;
        root = merge(root, other.root);
        other.root.reset();
    }
};
