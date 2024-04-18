#ifndef AVL_H
#define AVL_H

#include <cstdlib>
#include <utility>

template <typename KeyType, typename DataType> class AVL {
public:
  class iterator;
  struct node {
    KeyType first;
    DataType second;
    int children_high_difference;
    node *parent;
    node *right_child;
    node *left_child;

    node(KeyType key, DataType data, int high, node *parent, node *right_child,
         node *left_child)
        : first(key), second(data), children_high_difference(high),
          parent(parent), right_child(right_child), left_child(left_child) {}
  };

  void insert(const std::pair<KeyType, DataType> &data) {
    ++m_size;
    node *runner = m_root;
    node *parent = nullptr;
    while (runner != nullptr) {
      parent = runner;
      if (runner->first > data.first) {
        runner = runner->left_child;
      } else if (runner->first < data.first) {
        runner = runner->right_child;
      } else {
        return; // chave ja existe
      }
    }
    node *new_node =
        new node(data.first, data.second, 0, parent, nullptr, nullptr);
    if (parent == nullptr) { // tree empty
      m_root = new_node;
    } else if (data.first < parent->first) {
      ++(parent->children_high_difference);
      parent->left_child = new_node;
    } else {
      ++(parent->children_high_difference);
      parent->right_child = new_node;
    }
    while (parent != nullptr) {
      if (parent->children_high_difference == 2) {
        left_rotation(parent);
        return;
      } else if (parent->children_high_difference == -2) {
        return;
        right_rotation(parent);
      }
      parent = parent->parent;
    }
  }

  void erase(const KeyType &key) {
    // TODO balancear a arvore
    iterator it = find(key);
    if (it == nullptr) {
      return;
    }

    node *parent = &it.get_parent();
    node *substitute = (&it)->right_child;
    node *save = nullptr;
    if (substitute != nullptr and substitute->left_child == nullptr) {
      save = substitute;
      substitute->parent = parent;
      substitute->left_child = (&it)->left_child;
      if ((&it)->left_child != nullptr) {
        (&it)->left_child->parent = substitute;
      }
    } else if (substitute != nullptr) {
      while (substitute->left_child != nullptr) {
        substitute = substitute->left_child;
      }
      if (substitute->right_child != nullptr) {
        substitute->right_child->parent = substitute->parent;
        substitute->parent->left_child = substitute->right_child;
      }
      substitute->right_child = (&it)->right_child;
      substitute->left_child = (&it)->left_child;
      if ((&it)->right_child != nullptr) {
        (&it)->right_child->parent = substitute;
      }
      if ((&it)->left_child != nullptr) {
        (&it)->left_child->parent = substitute;
      }
      save = substitute;
    } else if (parent == nullptr) {
      m_root = (&it)->left_child;
      if ((&it)->left_child != nullptr) {
        (&it)->left_child->parent = nullptr;
      }
    } else if ((&it)->left_child != nullptr) {
      save = (&it)->left_child;
      (&it)->left_child->parent = parent;
    }

    if (parent != nullptr) {
      if ((&it)->first < parent->first) {
        parent->left_child = save;
      } else {
        parent->right_child = save;
      }
    }
    --m_size;
    if (m_size == 0) {
      m_root = nullptr;
    }
    delete &it;
  }

  iterator find(const KeyType &key) {
    iterator curr = m_root;
    while (curr != nullptr) {
      if (curr->first > key) {
        curr = curr->left_child;
      } else if (curr->first < key) {
        curr = curr->right_child;
      } else {
        return curr;
      }
    }
    return nullptr;
  }

  iterator begin() {
    if (m_root == nullptr) {
      return nullptr;
    }
    node *curr = m_root;
    while (curr->left_child != nullptr) {
      curr = curr->left_child;
    }
    return curr;
  }

  iterator end() {
    if (m_root == nullptr) {
      return nullptr;
    }
    node *curr = m_root;
    while (curr->right_child != nullptr) {
      curr = curr->right_child;
    }
    return curr;
  }

  ~AVL() {
    if (m_root == nullptr) {
      return;
    }
    clear_helper(m_root);
    m_root = nullptr;
    m_size = 0;
  }

  class iterator {
  public:
    iterator();
    iterator(node *pointer) : m_pointer(pointer) {}
    iterator &operator=(const iterator &it) { return m_pointer = it.m_pointer; }
    iterator &operator=(node *pointer) {
      m_pointer = pointer;
      return *this;
    }
    friend bool operator==(const iterator &lhs, const iterator &rhs) {
      return lhs.m_pointer == rhs.m_pointer;
    }
    friend bool operator!=(const iterator &lhs, const iterator &rhs) {
      return !(lhs == rhs);
    }
    DataType operator*() const { return m_pointer->second; }
    iterator operator++() {
      if (m_pointer->right_child != nullptr) {
        m_pointer = m_pointer->right_child;
        while (m_pointer->left_child != nullptr) {
          m_pointer = m_pointer->left_child;
        }
      } else {
        node *parent = m_pointer->parent;
        node *runner = m_pointer;
        while (parent != nullptr) {
          if (runner == parent->left_child) {
            m_pointer = parent;
            break;
          }
          runner = runner->parent;
          parent = runner->parent;
        }
      }
      return iterator(m_pointer);
    }
    iterator operator++(int) {
      auto cp = m_pointer;
      ++(*this);
      return cp;
    }
    iterator operator--() {
      if (m_pointer->left_child != nullptr) {
        m_pointer = m_pointer->left_child;
        while (m_pointer->right_child != nullptr) {
          m_pointer = m_pointer->right_child;
        }
      } else {
        node *parent = m_pointer->parent;
        node *runner = m_pointer;
        while (parent != nullptr) {
          if (runner == parent->right_child) {
            m_pointer = parent;
            break;
          }
          runner = runner->parent;
          parent = runner->parent;
        }
      }
      return iterator(m_pointer);
    }
    iterator operator--(int) {
      auto cp = m_pointer;
      --(*this);
      return cp;
    }
    node *operator->() { return m_pointer; }
    iterator get_parent() { return iterator(m_pointer->parent); }
    node *operator&() { return m_pointer; }

  private:
    node *m_pointer{nullptr};
  };

private:
  void clear_helper(node *node) {
    if (node->left_child != nullptr) {
      clear_helper(node->left_child);
    }
    if (node->right_child != nullptr) {
      clear_helper(node->right_child);
    }
    delete node;
  }
  void left_rotation(node *root) {
    node *new_root = root->right_child;
    if (root == m_root) {
      m_root = new_root;
    } else if (root == root->parent->left_child) {
      root->parent->left_child = new_root;
    } else {
      root->parent->right_child = new_root;
    }
    new_root->parent = root->parent;
    root->parent = new_root;
    root->right_child = new_root->left_child;
    new_root->left_child = root;
    if (root->right_child != nullptr) {
      root->right_child->parent = root;
    }
  }
  void right_rotation(node *root) {
    node *new_root = root->left_child;
    if (root == m_root) {
      m_root = new_root;
    } else if (root == root->parent->left_child) {
      root->parent->left_child = new_root;
    } else {
      root->parent->right_child = new_root;
    }
    new_root->parent = root->parent;
    root->parent = new_root;
    root->left_child = new_root->right_child;
    new_root->right_child = root;
    if (root->left_child != nullptr) {
      root->left_child->parent = root;
    }
  }

  size_t m_size{0};
  node *m_root{nullptr};
};

#endif // #ifndef AVL_H
