/**
* implement a container like std::map
*/
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
   class Key,
   class T,
   class Compare = std::less <Key>
   > class map {
  public:
   typedef pair<const Key, T> value_type;

  private:
   enum Color { RED, BLACK };

   struct Node {
       value_type *data;
       Node *left;
       Node *right;
       Node *parent;
       Color color;

       Node(const value_type &val, Node *p = nullptr)
           : left(nullptr), right(nullptr), parent(p), color(RED) {
           data = new value_type(val);
       }

       ~Node() {
           if (data) delete data;
       }
   };

   Node *root;
   Node *nil_node; // sentinel node representing end()
   size_t node_count;
   Compare comp;

   // Helper function to check if two keys are equal
   bool key_equal(const Key &a, const Key &b) const {
       return !comp(a, b) && !comp(b, a);
   }

   // Tree rotations
   void rotate_left(Node *x) {
       Node *y = x->right;
       x->right = y->left;
       if (y->left != nullptr) {
           y->left->parent = x;
       }
       y->parent = x->parent;
       if (x->parent == nullptr) {
           root = y;
       } else if (x == x->parent->left) {
           x->parent->left = y;
       } else {
           x->parent->right = y;
       }
       y->left = x;
       x->parent = y;
   }

   void rotate_right(Node *x) {
       Node *y = x->left;
       x->left = y->right;
       if (y->right != nullptr) {
           y->right->parent = x;
       }
       y->parent = x->parent;
       if (x->parent == nullptr) {
           root = y;
       } else if (x == x->parent->right) {
           x->parent->right = y;
       } else {
           x->parent->left = y;
       }
       y->right = x;
       x->parent = y;
   }

   // Insert fixup to maintain Red-Black Tree properties
   void insert_fixup(Node *z) {
       while (z->parent != nullptr && z->parent->color == RED) {
           if (z->parent->parent == nullptr) break;

           if (z->parent == z->parent->parent->left) {
               Node *y = z->parent->parent->right;
               if (y != nullptr && y->color == RED) {
                   z->parent->color = BLACK;
                   y->color = BLACK;
                   z->parent->parent->color = RED;
                   z = z->parent->parent;
               } else {
                   if (z == z->parent->right) {
                       z = z->parent;
                       rotate_left(z);
                   }
                   z->parent->color = BLACK;
                   z->parent->parent->color = RED;
                   rotate_right(z->parent->parent);
               }
           } else {
               Node *y = z->parent->parent->left;
               if (y != nullptr && y->color == RED) {
                   z->parent->color = BLACK;
                   y->color = BLACK;
                   z->parent->parent->color = RED;
                   z = z->parent->parent;
               } else {
                   if (z == z->parent->left) {
                       z = z->parent;
                       rotate_right(z);
                   }
                   z->parent->color = BLACK;
                   z->parent->parent->color = RED;
                   rotate_left(z->parent->parent);
               }
           }
       }
       root->color = BLACK;
   }

   // Delete fixup to maintain Red-Black Tree properties
   void delete_fixup(Node *x, Node *x_parent) {
       while (x != root && (x == nullptr || x->color == BLACK)) {
           if (x == x_parent->left) {
               Node *w = x_parent->right;
               if (w->color == RED) {
                   w->color = BLACK;
                   x_parent->color = RED;
                   rotate_left(x_parent);
                   w = x_parent->right;
               }
               if ((w->left == nullptr || w->left->color == BLACK) &&
                   (w->right == nullptr || w->right->color == BLACK)) {
                   w->color = RED;
                   x = x_parent;
                   x_parent = x->parent;
               } else {
                   if (w->right == nullptr || w->right->color == BLACK) {
                       if (w->left != nullptr) w->left->color = BLACK;
                       w->color = RED;
                       rotate_right(w);
                       w = x_parent->right;
                   }
                   w->color = x_parent->color;
                   x_parent->color = BLACK;
                   if (w->right != nullptr) w->right->color = BLACK;
                   rotate_left(x_parent);
                   x = root;
               }
           } else {
               Node *w = x_parent->left;
               if (w->color == RED) {
                   w->color = BLACK;
                   x_parent->color = RED;
                   rotate_right(x_parent);
                   w = x_parent->left;
               }
               if ((w->right == nullptr || w->right->color == BLACK) &&
                   (w->left == nullptr || w->left->color == BLACK)) {
                   w->color = RED;
                   x = x_parent;
                   x_parent = x->parent;
               } else {
                   if (w->left == nullptr || w->left->color == BLACK) {
                       if (w->right != nullptr) w->right->color = BLACK;
                       w->color = RED;
                       rotate_left(w);
                       w = x_parent->left;
                   }
                   w->color = x_parent->color;
                   x_parent->color = BLACK;
                   if (w->left != nullptr) w->left->color = BLACK;
                   rotate_right(x_parent);
                   x = root;
               }
           }
       }
       if (x != nullptr) x->color = BLACK;
   }

   // Find node with given key
   Node* find_node(const Key &key) const {
       Node *current = root;
       while (current != nullptr) {
           if (key_equal(key, current->data->first)) {
               return current;
           } else if (comp(key, current->data->first)) {
               current = current->left;
           } else {
               current = current->right;
           }
       }
       return nullptr;
   }

   // Find minimum node in subtree
   Node* tree_minimum(Node *x) const {
       while (x->left != nullptr) {
           x = x->left;
       }
       return x;
   }

   // Find maximum node in subtree
   Node* tree_maximum(Node *x) const {
       while (x->right != nullptr) {
           x = x->right;
       }
       return x;
   }

   // Find successor node
   Node* tree_successor(Node *x) const {
       if (x->right != nullptr) {
           return tree_minimum(x->right);
       }
       Node *y = x->parent;
       while (y != nullptr && x == y->right) {
           x = y;
           y = y->parent;
       }
       return y;
   }

   // Find predecessor node
   Node* tree_predecessor(Node *x) const {
       if (x->left != nullptr) {
           return tree_maximum(x->left);
       }
       Node *y = x->parent;
       while (y != nullptr && x == y->left) {
           x = y;
           y = y->parent;
       }
       return y;
   }

   // Deep copy tree
   Node* copy_tree(Node *other, Node *other_nil, Node *parent = nullptr) {
       if (other == nullptr || other == other_nil) return nullptr;

       Node *node = new Node(*other->data, parent);
       node->color = other->color;
       node->left = copy_tree(other->left, other_nil, node);
       node->right = copy_tree(other->right, other_nil, node);
       return node;
   }

   // Delete tree
   void delete_tree(Node *node) {
       if (node == nullptr) return;
       delete_tree(node->left);
       delete_tree(node->right);
       delete node;
   }

  public:
   /**
  * see BidirectionalIterator at CppReference for help.
  *
  * if there is anything wrong throw invalid_iterator.
  *     like it = map.begin(); --it;
  *       or it = map.end(); ++end();
    */
   class const_iterator;
   class iterator {
      private:
       /**
    * TODO add data members
    *   just add whatever you want.
        */
       Node *node;
       const map *container;

       friend class map;
       friend class const_iterator;

      public:
       iterator(Node *n = nullptr, const map *m = nullptr) : node(n), container(m) {}

       iterator(const iterator &other) : node(other.node), container(other.container) {}

       /**
    * TODO iter++
        */
       iterator operator++(int) {
           if (node == nullptr) throw invalid_iterator();
           iterator tmp = *this;
           if (node->right != nullptr) {
               node = container->tree_minimum(node->right);
           } else {
               Node *y = node->parent;
               while (y != nullptr && node == y->right) {
                   node = y;
                   y = y->parent;
               }
               node = y;
           }
           return tmp;
       }

       /**
    * TODO ++iter
        */
       iterator &operator++() {
           if (node == nullptr) throw invalid_iterator();
           if (node->right != nullptr) {
               node = container->tree_minimum(node->right);
           } else {
               Node *y = node->parent;
               while (y != nullptr && node == y->right) {
                   node = y;
                   y = y->parent;
               }
               node = y;
           }
           return *this;
       }

       /**
    * TODO iter--
        */
       iterator operator--(int) {
           iterator tmp = *this;
           if (node == nullptr) {
               if (container->root == nullptr) throw invalid_iterator();
               node = container->tree_maximum(container->root);
           } else if (node->left != nullptr) {
               node = container->tree_maximum(node->left);
           } else {
               Node *y = node->parent;
               while (y != nullptr && node == y->left) {
                   node = y;
                   y = y->parent;
               }
               if (y == nullptr) throw invalid_iterator();
               node = y;
           }
           return tmp;
       }

       /**
    * TODO --iter
        */
       iterator &operator--() {
           if (node == nullptr) {
               if (container->root == nullptr) throw invalid_iterator();
               node = container->tree_maximum(container->root);
           } else if (node->left != nullptr) {
               node = container->tree_maximum(node->left);
           } else {
               Node *y = node->parent;
               while (y != nullptr && node == y->left) {
                   node = y;
                   y = y->parent;
               }
               if (y == nullptr) throw invalid_iterator();
               node = y;
           }
           return *this;
       }

       /**
    * a operator to check whether two iterators are same (pointing to the same memory).
        */
       value_type &operator*() const {
           if (node == nullptr) throw invalid_iterator();
           return *(node->data);
       }

       bool operator==(const iterator &rhs) const {
           return node == rhs.node && container == rhs.container;
       }

       bool operator==(const const_iterator &rhs) const {
           return node == rhs.node && container == rhs.container;
       }

       /**
    * some other operator for iterator.
        */
       bool operator!=(const iterator &rhs) const {
           return !(*this == rhs);
       }

       bool operator!=(const const_iterator &rhs) const {
           return !(*this == rhs);
       }

       /**
    * for the support of it->first.
    * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
        */
       value_type *operator->() const noexcept {
           return node->data;
       }
   };
   class const_iterator {
       // it should has similar member method as iterator.
       //  and it should be able to construct from an iterator.
      private:
       // data members.
       Node *node;
       const map *container;

       friend class map;
       friend class iterator;

      public:
       const_iterator(Node *n = nullptr, const map *m = nullptr) : node(n), container(m) {}

       const_iterator(const const_iterator &other) : node(other.node), container(other.container) {}

       const_iterator(const iterator &other) : node(other.node), container(other.container) {}

       const_iterator operator++(int) {
           if (node == nullptr) throw invalid_iterator();
           const_iterator tmp = *this;
           if (node->right != nullptr) {
               node = container->tree_minimum(node->right);
           } else {
               Node *y = node->parent;
               while (y != nullptr && node == y->right) {
                   node = y;
                   y = y->parent;
               }
               node = y;
           }
           return tmp;
       }

       const_iterator &operator++() {
           if (node == nullptr) throw invalid_iterator();
           if (node->right != nullptr) {
               node = container->tree_minimum(node->right);
           } else {
               Node *y = node->parent;
               while (y != nullptr && node == y->right) {
                   node = y;
                   y = y->parent;
               }
               node = y;
           }
           return *this;
       }

       const_iterator operator--(int) {
           const_iterator tmp = *this;
           if (node == nullptr) {
               if (container->root == nullptr) throw invalid_iterator();
               node = container->tree_maximum(container->root);
           } else if (node->left != nullptr) {
               node = container->tree_maximum(node->left);
           } else {
               Node *y = node->parent;
               while (y != nullptr && node == y->left) {
                   node = y;
                   y = y->parent;
               }
               if (y == nullptr) throw invalid_iterator();
               node = y;
           }
           return tmp;
       }

       const_iterator &operator--() {
           if (node == nullptr) {
               if (container->root == nullptr) throw invalid_iterator();
               node = container->tree_maximum(container->root);
           } else if (node->left != nullptr) {
               node = container->tree_maximum(node->left);
           } else {
               Node *y = node->parent;
               while (y != nullptr && node == y->left) {
                   node = y;
                   y = y->parent;
               }
               if (y == nullptr) throw invalid_iterator();
               node = y;
           }
           return *this;
       }

       const value_type &operator*() const {
           if (node == nullptr) throw invalid_iterator();
           return *(node->data);
       }

       bool operator==(const iterator &rhs) const {
           return node == rhs.node && container == rhs.container;
       }

       bool operator==(const const_iterator &rhs) const {
           return node == rhs.node && container == rhs.container;
       }

       bool operator!=(const iterator &rhs) const {
           return !(*this == rhs);
       }

       bool operator!=(const const_iterator &rhs) const {
           return !(*this == rhs);
       }

       const value_type *operator->() const noexcept {
           return node->data;
       }
   };

   /**
  * TODO two constructors
    */
   map() : root(nullptr), nil_node(nullptr), node_count(0) {}

   map(const map &other) : root(nullptr), nil_node(nullptr), node_count(other.node_count) {
       if (other.root != nullptr) {
           root = copy_tree(other.root, other.nil_node);
       }
   }

   /**
  * TODO assignment operator
    */
   map &operator=(const map &other) {
       if (this == &other) return *this;
       clear();
       node_count = other.node_count;
       if (other.root != nullptr) {
           root = copy_tree(other.root, other.nil_node);
       }
       return *this;
   }

   /**
  * TODO Destructors
    */
   ~map() {
       clear();
   }

   /**
  * TODO
  * access specified element with bounds checking
  * Returns a reference to the mapped value of the element with key equivalent to key.
  * If no such element exists, an exception of type `index_out_of_bound'
    */
   T &at(const Key &key) {
       Node *node = find_node(key);
       if (node == nullptr) throw index_out_of_bound();
       return node->data->second;
   }

   const T &at(const Key &key) const {
       Node *node = find_node(key);
       if (node == nullptr) throw index_out_of_bound();
       return node->data->second;
   }

   /**
  * TODO
  * access specified element
  * Returns a reference to the value that is mapped to a key equivalent to key,
  *   performing an insertion if such key does not already exist.
    */
   T &operator[](const Key &key) {
       Node *node = find_node(key);
       if (node != nullptr) {
           return node->data->second;
       }

       // Insert new element with default-constructed value
       Node *new_node = new Node(value_type(key, T()), nullptr);
       node_count++;

       if (root == nullptr) {
           root = new_node;
           root->color = BLACK;
           return new_node->data->second;
       }

       Node *parent = nullptr;
       Node *current = root;
       while (current != nullptr) {
           parent = current;
           if (comp(key, current->data->first)) {
               current = current->left;
           } else {
               current = current->right;
           }
       }

       new_node->parent = parent;
       if (comp(key, parent->data->first)) {
           parent->left = new_node;
       } else {
           parent->right = new_node;
       }

       insert_fixup(new_node);
       return new_node->data->second;
   }

   /**
  * behave like at() throw index_out_of_bound if such key does not exist.
    */
   const T &operator[](const Key &key) const {
       return at(key);
   }

   /**
  * return a iterator to the beginning
    */
   iterator begin() {
       if (root == nullptr) return iterator(nullptr, this);
       return iterator(tree_minimum(root), this);
   }

   const_iterator cbegin() const {
       if (root == nullptr) return const_iterator(nullptr, this);
       return const_iterator(tree_minimum(root), this);
   }

   /**
  * return a iterator to the end
  * in fact, it returns past-the-end.
    */
   iterator end() {
       return iterator(nullptr, this);
   }

   const_iterator cend() const {
       return const_iterator(nullptr, this);
   }

   /**
  * checks whether the container is empty
  * return true if empty, otherwise false.
    */
   bool empty() const {
       return node_count == 0;
   }

   /**
  * returns the number of elements.
    */
   size_t size() const {
       return node_count;
   }

   /**
  * clears the contents
    */
   void clear() {
       delete_tree(root);
       root = nullptr;
       node_count = 0;
   }

   /**
  * insert an element.
  * return a pair, the first of the pair is
  *   the iterator to the new element (or the element that prevented the insertion),
  *   the second one is true if insert successfully, or false.
    */
   pair<iterator, bool> insert(const value_type &value) {
       Node *existing = find_node(value.first);
       if (existing != nullptr) {
           return pair<iterator, bool>(iterator(existing, this), false);
       }

       Node *new_node = new Node(value, nullptr);
       node_count++;

       if (root == nullptr) {
           root = new_node;
           root->color = BLACK;
           return pair<iterator, bool>(iterator(new_node, this), true);
       }

       Node *parent = nullptr;
       Node *current = root;
       while (current != nullptr) {
           parent = current;
           if (comp(value.first, current->data->first)) {
               current = current->left;
           } else {
               current = current->right;
           }
       }

       new_node->parent = parent;
       if (comp(value.first, parent->data->first)) {
           parent->left = new_node;
       } else {
           parent->right = new_node;
       }

       insert_fixup(new_node);
       return pair<iterator, bool>(iterator(new_node, this), true);
   }

   /**
  * erase the element at pos.
  *
  * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
    */
   void erase(iterator pos) {
       if (pos.node == nullptr || pos.container != this) {
           throw invalid_iterator();
       }

       Node *z = pos.node;
       Node *y = z;
       Node *x;
       Node *x_parent;
       Color y_original_color = y->color;

       if (z->left == nullptr) {
           x = z->right;
           x_parent = z->parent;
           if (z->parent == nullptr) {
               root = z->right;
           } else if (z == z->parent->left) {
               z->parent->left = z->right;
           } else {
               z->parent->right = z->right;
           }
           if (z->right != nullptr) {
               z->right->parent = z->parent;
           }
       } else if (z->right == nullptr) {
           x = z->left;
           x_parent = z->parent;
           if (z->parent == nullptr) {
               root = z->left;
           } else if (z == z->parent->left) {
               z->parent->left = z->left;
           } else {
               z->parent->right = z->left;
           }
           if (z->left != nullptr) {
               z->left->parent = z->parent;
           }
       } else {
           y = tree_minimum(z->right);
           y_original_color = y->color;
           x = y->right;

           if (y->parent == z) {
               x_parent = y;
               if (x != nullptr) x->parent = y;
           } else {
               x_parent = y->parent;
               if (y->parent->left == y) {
                   y->parent->left = x;
               } else {
                   y->parent->right = x;
               }
               if (x != nullptr) {
                   x->parent = y->parent;
               }
               y->right = z->right;
               if (y->right != nullptr) {
                   y->right->parent = y;
               }
           }

           if (z->parent == nullptr) {
               root = y;
           } else if (z == z->parent->left) {
               z->parent->left = y;
           } else {
               z->parent->right = y;
           }
           y->parent = z->parent;
           y->left = z->left;
           if (y->left != nullptr) {
               y->left->parent = y;
           }
           y->color = z->color;
       }

       if (y_original_color == BLACK) {
           delete_fixup(x, x_parent);
       }

       delete z;
       node_count--;
   }

   /**
  * Returns the number of elements with key
  *   that compares equivalent to the specified argument,
  *   which is either 1 or 0
  *     since this container does not allow duplicates.
  * The default method of check the equivalence is !(a < b || b > a)
    */
   size_t count(const Key &key) const {
       return find_node(key) != nullptr ? 1 : 0;
   }

   /**
  * Finds an element with key equivalent to key.
  * key value of the element to search for.
  * Iterator to an element with key equivalent to key.
  *   If no such element is found, past-the-end (see end()) iterator is returned.
    */
   iterator find(const Key &key) {
       Node *node = find_node(key);
       if (node == nullptr) return end();
       return iterator(node, this);
   }

   const_iterator find(const Key &key) const {
       Node *node = find_node(key);
       if (node == nullptr) return cend();
       return const_iterator(node, this);
   }
};

}

#endif
