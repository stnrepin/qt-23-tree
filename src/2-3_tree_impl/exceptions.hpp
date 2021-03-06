#ifndef TWO_THREE_TREE_IMPL_EXCEPTIONS_HPP
#define TWO_THREE_TREE_IMPL_EXCEPTIONS_HPP

#include <stdexcept>

namespace two_three_tree {

class KeyExistsException : public std::runtime_error {
    using std::runtime_error::runtime_error;

public:
    KeyExistsException() : std::runtime_error("node key already exists")
    { }
};

class KeyNotExistsException : public std::runtime_error {
    using std::runtime_error::runtime_error;

public:
    KeyNotExistsException() : std::runtime_error("node key is not exists")
    { }
};

} // namespace two_three_tree

#endif // !TWO_THREE_TREE_IMPL_EXCEPTIONS_HPP

