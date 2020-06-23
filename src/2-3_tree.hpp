#ifndef ONE_TWO_TREE_HPP
#define ONE_TWO_TREE_HPP

#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stack>
#include <utility>
#include <vector>

class TwoThreeTree {
public:
    using value_type = unsigned int;

    class iterator;

    TwoThreeTree();
    TwoThreeTree(std::initializer_list<unsigned int> init_list);
    TwoThreeTree(int a, int b);

    TwoThreeTree(const TwoThreeTree& src);
    TwoThreeTree& operator=(const TwoThreeTree& src);
    TwoThreeTree(TwoThreeTree&& src) noexcept;
    TwoThreeTree& operator=(TwoThreeTree&& src) noexcept;

    template<class InputIt1, class InputIt2>
    TwoThreeTree(InputIt1 beg, InputIt2 end) : TwoThreeTree() {
        for (auto it = beg; it != end; ++it) {
            Add(*it);
        }
    }

    void Add(unsigned int key);
    void Remove(unsigned int key);
    bool Find(unsigned int key) const;

    size_t size() const { return size_; };
    iterator begin() const;
    iterator end() const;
    void push_back(unsigned int key) { Add(key); }

    friend std::ostream& operator<<(std::ostream &os, const TwoThreeTree& tree);

    ~TwoThreeTree();

private:
    class TwoThreeTreeNode;
    class TwoThreeTreeRootNode;

    void BalanceFromNode(TwoThreeTreeNode* n, std::stack<TwoThreeTreeNode*>* path);
    void Remove(TwoThreeTreeNode* n, TwoThreeTreeNode* old_par);
    std::pair<TwoThreeTreeNode*, TwoThreeTreeNode*> FindMinimalFromNode(TwoThreeTreeNode* n) const;

    std::vector<TwoThreeTreeNode*> elems_;
    size_t size_;
    // Для упрощения некоторых моментов в алгоритме, root_ -- это не
    // истинная вершина дерева, а фиктивная, обоими потомками которой
    // является настоящая вершина дерева.
    TwoThreeTreeRootNode* root_;
};

class TwoThreeTree::TwoThreeTreeNode {
public:
    TwoThreeTreeNode(const TwoThreeTreeNode&) = default;
    TwoThreeTreeNode& operator=(const TwoThreeTreeNode&) = default;
    TwoThreeTreeNode(TwoThreeTreeNode&&) = default;
    TwoThreeTreeNode& operator=(TwoThreeTreeNode&&) = default;

    explicit TwoThreeTreeNode(unsigned int k) :
        key_(k << 1u), real_key_(k),  left_(nullptr), right_(nullptr)
    { }

    void ReplaceChild(const TwoThreeTreeNode* child, TwoThreeTreeNode* n) {
        if (left() == child) {
            set_left(n);
        }
        else {
            set_right(n);
        }
    }

    unsigned int key() const {
        return real_key_; // key_ >> 1;
    }

    void set_key(unsigned int k) {
        key_ = k << 1u;
        real_key_ = k;
    }

    bool next() const {
        return (key_ & 1u) != 0u;
    }

    void unset_next() {
        key_ &= ~1u;
    }

    void toggle_next() {
        key_ ^= 1u;
    }

    TwoThreeTreeNode* left()  const { return left_; }
    virtual void set_left(TwoThreeTreeNode* l) {
        left_ = l;
    }

    TwoThreeTreeNode* right() const { return right_; }
    virtual void set_right(TwoThreeTreeNode* r) {
        right_ = r;
    }

    virtual ~TwoThreeTreeNode() = default;

private:
    unsigned int key_;
    unsigned int real_key_; // Для отладки, можно убрать при желании. Никакой
                            // код вне этого класса на это поле не ссылается.
    TwoThreeTreeNode* left_;
    TwoThreeTreeNode* right_;
};

class TwoThreeTree::TwoThreeTreeRootNode : public TwoThreeTree::TwoThreeTreeNode {
public:
    TwoThreeTreeRootNode() : TwoThreeTreeNode(0)
    { }

    TwoThreeTreeRootNode(unsigned int k) = delete;

    void set_left(TwoThreeTreeNode* l) override {
        TwoThreeTreeNode::set_left(l);
        TwoThreeTreeNode::set_right(l);
    }

    void set_right(TwoThreeTreeNode* r) override {
        TwoThreeTreeNode::set_left(r);
        TwoThreeTreeNode::set_right(r);
    }

    void Clear() {
        set_left(nullptr);
    }
};

class TwoThreeTree::iterator :
        public std::iterator<
                std::input_iterator_tag,             // iterator_category
                unsigned int,                        // value_type
                unsigned int,                        // difference_type
                const unsigned int*,                 // pointer
                unsigned int                         // reference
            >
{
    friend class TwoThreeTree;
public:
    iterator() : cur_(nullptr)
    { }

    explicit iterator(const TwoThreeTree& tree);

    TwoThreeTree::TwoThreeTreeNode* cur() const { return cur_; }

    iterator& operator++();

    iterator operator++(int) { // NOLINT
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(iterator other) const {
        return cur_ == other.cur_;
    }

    bool operator!=(iterator other) const {
        return !(*this == other);
    }

    reference operator->() const {
        return *(*this);
    }

    reference operator*() const {
        if (cur_ == nullptr) {
            return 0;
        }
        return cur_->key();
    }

private:
    void DoStep();

    std::stack<TwoThreeTree::TwoThreeTreeNode*> elems_;
    TwoThreeTree::TwoThreeTreeNode* cur_;
};

#endif // !ONE_TWO_TREE_HPP

