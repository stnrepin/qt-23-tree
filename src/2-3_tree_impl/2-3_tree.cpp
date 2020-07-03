#include "2-3_tree_impl/2-3_tree.hpp"

#include <queue>

#include "2-3_tree_impl/exceptions.hpp"

namespace two_three_tree {

TwoThreeTree::TwoThreeTree() {
    size_ = 0;
    root_ = new TwoThreeTreeRootNode();
}

TwoThreeTree::TwoThreeTree(std::initializer_list<unsigned int> init_list)
                : TwoThreeTree()
{
    for (auto x : init_list) {
        Add(x);
    }
}

TwoThreeTree::TwoThreeTree(int a, int b) : TwoThreeTree() {
    if (a > b) {
        throw std::out_of_range("a should be less or equal b");
    }

    for (auto i = a; i <= b; i++) {
        Add(i);
    }
}

TwoThreeTree::TwoThreeTree(const TwoThreeTree& src) : TwoThreeTree() {
    elems_.reserve(src.size());
    for (auto* node : src.elems_) {
        Add(node->key());
    }
}

TwoThreeTree& TwoThreeTree::operator=(const TwoThreeTree& src) {
    if (&src == this) {
        return *this;
    }

    for (auto key : src) {
        Add(key);
    }
    return *this;
}

TwoThreeTree::TwoThreeTree(TwoThreeTree&& src) noexcept {
    size_ = src.size_;
    elems_ = std::move(src.elems_);
    root_ = src.root_;
    src.size_ = 0;
    src.root_ = nullptr;
}

TwoThreeTree& TwoThreeTree::operator=(TwoThreeTree&& src) noexcept {
    if (&src == this) {
        return *this;
    }

    size_ = src.size_;
    root_ = src.root_;
    elems_ = std::move(src.elems_);
    src.size_ = 0;
    src.root_ = nullptr;
    return *this;
}

void TwoThreeTree::Add(unsigned int key) {
    // root_ -- фиктивная вершина, см. комментарий в определении класса.
    //
    auto* n = new TwoThreeTreeNode(key);
    if (root_->right() == nullptr) {
        ++size_;
        root_->set_right(n);
        elems_.push_back(n);
        return;
    }

    TwoThreeTreeNode* current = root_;
    std::stack<TwoThreeTreeNode*> path;
    while(current != nullptr) {
        path.push(current);

        TwoThreeTreeNode* next = nullptr;
        if (key < current->key()) {
            next = current->left();
            if(next == nullptr) {
                current->set_left(n);
            }
        }
        else if (key > current->key()) {
            next = current->right();
            if(next == nullptr) {
                current->set_right(n);
            }
        }
        else {
            delete n;
            throw KeyExistsException();
            return;
        }

        current = next;
    }

    BalanceFromNode(n, &path);
    elems_.push_back(n);
    ++size_;
}

void TwoThreeTree::BalanceFromNode(TwoThreeTreeNode* n, std::stack<TwoThreeTreeNode*>* path)
{
    if (path->size() <= 1)
    {
        return;
    }

    auto* p = path->top();
    path->pop();
    auto* p_p = path->top();

    //
    // N справа
    //
    if (n == p->right()) {
        //
        // Узел в группе второй
        //
        if (!p_p->next() || p_p->left() == p) {
            p->toggle_next();
            return;
        }

        //
        // Узел в группе третий: он должен быть перемещен на уровень выше.
        //

        /*
            В соответствии с алгоритмом, поддерево всегда имеет такую структуру
            (P всегда справа от P_P, N всегда справа от P). A и B могу быть NULL.

                P_P_P                P_P_P
                 |                     |
                P_P -- P      =>       P
                 |    / \             / \
                 A   B   N          P_P  N
                                    / \
                                   A   B
        */
        path->pop(); // Удаляем P_P из пути.
        auto* p_p_p = path->top();

        // 1. Переносим указатель в P_P_P с P_P на P
        p_p_p->ReplaceChild(p_p, p);

        // 2. Удаляем P из потомков P_P
        p_p->set_right(nullptr);

        // 3. Вставляем P_P в дерево как потомка P
        //    и сохраняем предыдущего потомка P (который не N)
        p_p->set_right(p->left());
        p->set_left(p_p);

        // 4. У P_P и N биты next должны быть равны 0.
        n->unset_next();
        p_p->unset_next();
    }
    //
    // N слева
    //
    else {
        //
        // Узел в группе второй. Равносильно тому, что у P нет правого потомок.
        //
        if (!p->next()) {
            /*
                        P_P_P        P_P_P
                          |            |
                          P     =>     N -- P
                         / \                |
                        N   A               A
            */
            // Сдвигаем указатель с P на N.
            p_p->ReplaceChild(p, n);

            // Делаем N родителем P.
            p->set_left(n->right());
            n->set_right(p);

            // Новую балансироку начинаем с P, и N теперь должно входить в путь.
            path->push(n);
        }
        //
        // Узел в группе третий: родитель должен быть перемещен на уровень выше.
        //
        else {
            p->unset_next();
        }
    }
    //
    // Переходим к анализу следующиего уровня.
    //

    // C++ оптимизирует хвостовую рекурсию, так что фактически
    // это итерационный алгоритм
    // Например: https://stackoverflow.com/a/34129
    BalanceFromNode(p, path); 
}

void TwoThreeTree::Remove(unsigned int key) {
    TwoThreeTreeNode* par = root_;
    TwoThreeTreeNode* cur = par->left();
    while (cur != nullptr && cur->key() != key) {
        par = cur;
        if (key < cur->key()) {
            cur = cur->left();
        } 
        else {
            cur = cur->right();
        }
    }

    if (cur == nullptr) {
        throw KeyNotExistsException();
    }

    if (cur->left() == nullptr && cur->right() == nullptr) {
        par->ReplaceChild(cur, nullptr);
        Remove(cur, par);
        return;
    }

    if (cur->left() == nullptr) {
        par->ReplaceChild(cur, cur->right());
        Remove(cur, par);
        return;
    }

    if (cur->right() == nullptr) {
        par->ReplaceChild(cur, cur->left());
        Remove(cur, par);
        return;
    }

    auto [min, min_par] = FindMinimalFromNode(cur);

    // Заменяем cur на min, удаляем cur.
    //
    min_par->set_left(min->right());
    min->set_left(cur->left());
    min->set_right(cur->right());
    par->ReplaceChild(cur, min);
    Remove(cur, min_par);
}

void TwoThreeTree::Remove(TwoThreeTreeNode* n, TwoThreeTreeNode *old_par) {
    // Если в elems_ n->key() повторяется, то удаляем только второе вхождение,
    // так как первое содержится в дереве и представляет все остальные (его
    // удаление повлечет утечку остальных, то есть они окажутся недоступными
    // через методы, работающие с деревом)

    int first_index = -1;
    int second_index = -1;
    for (size_t i = 0; i < elems_.size(); ++i) {
        if (elems_[i]->key() == n->key()) {
            if (first_index == -1) {
                first_index = i;
            }
            else {
                second_index = i;
                break;
            }
        }
    }
    auto has_duplicates = second_index != -1;
    if (!has_duplicates) {
        second_index = first_index;
    }

    delete elems_[first_index];
    elems_.erase(elems_.begin()+second_index);
    --size_;

    if (has_duplicates) {
        return;
    }

    old_par->unset_next();
}

bool TwoThreeTree::Find(unsigned int key) const {
    TwoThreeTreeNode* cur = root_->left();
    while (cur != nullptr) {
        if (key < cur->key()) {
            cur = cur->left();
        }
        else if (key > cur->key()) {
            cur = cur->right();
        }
        else {
            return true;
        }
    }
    return false;
}

int TwoThreeTree::GetDepth() const {
    int depth = 0;
    std::queue<TwoThreeTreeNode*> q;
    TwoThreeTreeNode* node = root_->left();
    q.push(node);
    while (true) {
        auto q_size = q.size();
        if (q_size == 0) {
            break;
        }

        ++depth;

        while (q_size > 0) {
            node = q.front();
            q.pop();
            if (node->left() != nullptr) {
                q.push(node->left());
            }
            if (node->right() != nullptr) {
                q.push(node->right());
            }
            --q_size;
        }
    }
    return depth;
}

std::pair<TwoThreeTree::TwoThreeTreeNode*, TwoThreeTree::TwoThreeTreeNode*>
TwoThreeTree::FindMinimalFromNode(TwoThreeTreeNode* n) const {
    TwoThreeTreeNode* prev_prev = nullptr;
    TwoThreeTreeNode* prev = nullptr;
    while (n != nullptr) {
        prev_prev = prev;
        prev = n;
        n = n->left();
    }
    return { prev, prev_prev };
}

TwoThreeTree::iterator TwoThreeTree::begin() const {
    return TwoThreeTree::iterator(*this);
}

TwoThreeTree::iterator TwoThreeTree::end() const {
    return TwoThreeTree::iterator();
}

std::ostream& operator<<(std::ostream& os, const TwoThreeTree& tr) {
    if (tr.size() == 0) {
        os << "[  ]";
        return os;
    }

    os << "[  ";

    for (auto x = tr.begin(); x != tr.end(); ++x) {
        auto postfix = x.cur()->next() ? "* " : "  ";
        os << x.cur()->key() << postfix;
    }

    os << "]";

    return os;
}

TwoThreeTree::~TwoThreeTree() {
    if (root_ == nullptr) {
        return;
    }

    for (auto* x : elems_) {
        delete x;
    }
    size_ = 0;
    root_->Clear();
    delete root_;
}

void TwoThreeTree::iterator::DoStep() {
    if (cur_ == nullptr && elems_.empty()) {
        cur_ = nullptr;
        return;
    }

    while (cur_ != nullptr) {
        elems_.push(cur_);
        cur_ = cur_->left();
    }
    cur_ = elems_.top();
    elems_.pop();
}

TwoThreeTree::iterator::iterator(const TwoThreeTree& tree) {
    if (tree.size() == 0) {
        cur_ = nullptr;
        return;
    }
    cur_ = tree.root_->left();
    DoStep();
}

TwoThreeTree::iterator& TwoThreeTree::iterator::operator++() {
    if (cur_ == nullptr) {
        return *this;
    }
    cur_ = cur_->right();
    DoStep();
    return *this;
}

} // namespace two_three_tree

