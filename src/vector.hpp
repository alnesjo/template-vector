/* vector.hpp - Robert Alnesjö <rhalnesjo@gmail.com> */

#pragma once

#include <cstddef>
#include <stdexcept>
#include <initializer_list>
#include <type_traits>

namespace alnesjo {

  template <typename T>
  class Vector {
    static_assert(std::is_move_constructible<T>::value &&
                  std::is_move_assignable<T>::value,
                  "Vector requires value_type to be move -constructible "
                  "and -assignable");
  public:
    typedef T value_type;
    typedef std::size_t size_type;
    typedef value_type & reference;
    typedef value_type const & const_reference;
    typedef value_type * pointer;
    typedef value_type const * const_pointer;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // Empty vector with zero capacity.
    Vector(void);

    // Count elements initialized to value
    Vector(size_type count, value_type value = value_type());

    // Vector containing initializer list elements, preserves order.
    Vector(std::initializer_list<value_type>);

    // Copy and move construction/assignment.
    Vector(Vector const & other);
    Vector(Vector && other);
    Vector & operator=(Vector other);
    template <typename U> friend void swap(Vector<U> &, Vector<U> &);

    ~Vector(void);

    // Insert value at the end of vector.
    void push_back(value_type value);

    // Insert value at pos. Will push back end-pos elements.
    void insert(size_type pos, value_type value);

    // Clear vector such stat it's size becomes zero.
    void clear(void);

    // Erase vector entry at pos and pull back end-pos elements.
    void erase(size_type pos);

    // Reset all elements to default value: value_type().
    void reset(void);

    // The number of elements contained in vector.
    size_type size(void) const;

    // The number of elements that can be held in currently allocated storage.
    size_type capacity(void) const;

    // Iterator of first element equal to value.
    iterator find(const_reference value);
    const_iterator find(const_reference value) const;

    // Iterator at beginning of vector.
    iterator begin(void);
    const_iterator begin(void) const;

    // Iterator at end (past last element) of vector.
    iterator end(void);
    const_iterator end(void) const;

    // Reverse iterator at beginning (past first element) of vector.
    reverse_iterator rbegin(void);
    const_reverse_iterator rbegin(void) const;

    // Reverse iterator at end of vector.
    reverse_iterator rend(void);
    const_reverse_iterator rend(void) const;

    // Equivalent to: begin() + pos, but does range checking.
    reference operator[](size_type pos);
    const_reference operator[](size_type pos) const;

  protected:
    pointer _array;
    size_type _capacity;
    size_type _size;

  private:
    void _realloc(size_type);
  };

  template <typename T>
  inline Vector<T>::Vector(void) : _array(nullptr), _capacity(0), _size(0) {}

  template <typename T>
  inline Vector<T>::Vector(size_type n, value_type val) {
    _capacity = _size = n;
    if (_capacity > 0) {
      _array = new value_type [_capacity];
      for (auto & elem : *this) {
        elem = val;
      }
    } else {
      _array = nullptr;
    }
  }

  template <typename T>
  inline Vector<T>::Vector(std::initializer_list<value_type> il) {
    _capacity = _size = il.size();
    if (_capacity > 0) {
      _array = new value_type [_capacity];
      std::copy(il.begin(), il.end(), _array);
    } else {
      _array = nullptr;
    }
  }

  template <typename T>
  inline Vector<T>::Vector(Vector const & other) {
    // allocate new resources and copy contents of other to this
    _capacity = other._capacity;
    _size = other._size;
    if (_capacity > 0) {
      _array = new value_type [_capacity];
      iterator dest = begin();
      const_iterator src = other.begin();
      while(dest != end()) {
        *dest++ = *src++;
      }
    } else {
      _array = nullptr;
    }
  }

  template <typename T>
  inline Vector<T>::Vector(Vector && other) : Vector() {
    swap(*this, other);
  }

  template <typename T>
  inline Vector<T>::~Vector(void) {
    if (_capacity > 0) {
      delete [] _array;
    }
  }

  template <typename T>
  inline auto Vector<T>::operator=(Vector<value_type> other)
    -> Vector<value_type> & {
    swap(*this, other);
    return *this;
  }

  template <typename T>
  void swap(Vector<T> & lhs, Vector<T> & rhs) {
    std::swap(lhs._capacity, rhs._capacity);
    std::swap(lhs._size, rhs._size);
    std::swap(lhs._array, rhs._array);
  }

  template <typename T>
  inline void Vector<T>::push_back(value_type val) {
    insert(_size, val);
  }

  template <typename T>
  inline void Vector<T>::insert(size_type pos, value_type value) {
    if (pos < 0 || pos > _size) {
      throw std::out_of_range("Trying to insert element at position: "
                              + std::to_string(pos) + ", in a Vector "
                              "of size: " + std::to_string(size())
                              + ".");
    }
    if (_size == _capacity) {
      _realloc(_capacity ? _capacity*2 : 1);
    }
    _size++;
    reverse_iterator head, tail, tar = rbegin() - pos;
    head = tail = rend();
    // push back trailing elements
    for (head++; head != tar; *tail++ = *head++);
    _array[pos] = value;
  }

  template <typename T>
  inline void Vector<T>::clear(void) {
    _size = 0;
  }

  template <typename T>
  inline void Vector<T>::erase(size_type pos) {
    if (pos < 0 || pos >= _size) {
      throw std::out_of_range("Trying to erase element at position: "
                              + std::to_string(pos) + ", in a Vector "
                              "of size: " + std::to_string(size())
                              + ".");
    }
    iterator head, tail, tar = end();
    head = tail = begin() + pos;
    // pull back trailing elements
    for (head++; head != tar; *tail++ = *head++);
    _size--;
  }

  template <typename T>
  inline void Vector<T>::reset(void) {
    for (auto & elem : *this) {
      elem = value_type();
    }
  }

  template <typename T>
  inline auto Vector<T>::size(void) const
    -> size_type {
    return _size;
  }

  template <typename T>
  inline auto Vector<T>::capacity(void) const
    -> size_type {
    return _capacity;
  }

  template <typename T>
  inline auto Vector<T>::find(const_reference ref)
    -> iterator {
    auto tar = end();
    for (auto it = begin(); it != tar; it++) {
      if (*it == ref) {
        tar = it;
        break;
      }
    }
    return tar;
  }

  template <typename T>
  inline auto Vector<T>::find(const_reference ref) const
    -> const_iterator {
    auto tar = end();
    for (auto it = begin(); it != tar; it++) {
      if (*it == ref) {
        tar = it;
        break;
      }
    }
    return tar;
  }

  template <typename T>
  inline auto Vector<T>::begin(void)
    -> iterator {
    return _array;
  }

  template <typename T>
  inline auto Vector<T>::end(void)
    -> iterator {
    return _array+_size;
  }

  template <typename T>
  inline auto Vector<T>::rbegin(void)
    -> reverse_iterator {
    return reverse_iterator(begin());
  }

  template <typename T>
  inline auto Vector<T>::rend(void)
    -> reverse_iterator {
    return reverse_iterator(end());
  }

  template <typename T>
  inline auto Vector<T>::begin(void) const
    -> const_iterator {
    return _array;
  }

  template <typename T>
  inline auto Vector<T>::end(void) const
    -> const_iterator {
    return _array+_size;
  }

  template <typename T>
  inline auto Vector<T>::rbegin(void) const
    -> const_reverse_iterator {
    return const_reverse_iterator(begin());
  }

  template <typename T>
  inline auto Vector<T>::rend(void) const
    -> const_reverse_iterator {
    return const_reverse_iterator(end());
  }

  template <typename T>
  inline auto Vector<T>::operator[](size_type pos)
    -> reference {
    if (pos < 0 || pos >= _size) {
      throw std::out_of_range("Trying to acess element at position: "
                              + std::to_string(pos) + ", in a Vector "
                              "of size: " + std::to_string(size())
                              + ".");
    }
    return _array[pos];
  }

  template <typename T>
  inline auto Vector<T>::operator[](size_type index) const
    -> const_reference {
    if (index < 0 || index >= _size) {
      throw std::out_of_range("OUT OF RANGE");
    }
    return _array[index];
  }

  template <typename T>
  inline void Vector<T>::_realloc(size_type new_capacity) {
    pointer new_array = new value_type [new_capacity];
    iterator it = new_array;
    for (auto & elem : *this) {
      *it++ = elem;
    }
    if (_capacity > 0) {
      delete [] _array;
    }
    _array = new_array;
    _capacity = new_capacity;
  }
}
