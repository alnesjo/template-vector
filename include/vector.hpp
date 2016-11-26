/* vector.hpp - Robert Alnesjö <rhalnesjo@gmail.com> */

#pragma once

#include <cstddef>
#include <stdexcept>
#include <initializer_list>
#include <type_traits>
#include <algorithm>

namespace alnesjo {

  template <typename T>
  class vector {
    static_assert(std::is_move_constructible<T>::value &&
                  std::is_move_assignable<T>::value,
                  "vector requires value_type to be move -constructible "
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
    vector(void);

    // Count elements initialized to value
    vector(size_type count, value_type value = value_type());

    // Vector containing initializer list elements, preserves order.
    vector(std::initializer_list<value_type>);

    // Copy and move construction/assignment.
    vector(vector const & other);
    vector(vector && other);
    vector & operator=(vector other);

    ~vector(void);

    // Reallocates underlying storage to capacity. Does nothing for capacity
    // less than vector size.
    void reserve(size_type capacity);

    // Insert value at the end of vector.
    void push_back(value_type value);

    // Push back value constructed by args.
    template <typename... Args> void emplace_back(Args... args);

    // Insert value at pos. Will push back end-pos elements.
    void insert(size_type pos, value_type value);

    // Insert value constructed by args at pos.
    template <typename... Args> void emplace(size_type pos, Args... args);

    // Clear vector such stat it's size becomes zero.
    void clear(void);

    // Erase vector entry at pos and pull back end-pos elements.
    // Alternatively erase elements in range [from, to), or if to > from
    // [begin, from) and [to, end).
    void erase(size_type pos);
    void erase(iterator from, iterator to);

    // Set all elements to value.
    void fill(const_reference value);

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

  private:
    pointer _array;
    size_type _capacity;
    size_type _size;

    template <typename U> friend void swap(vector<U> &, vector<U> &);
  };

  template <typename T>
  inline vector<T>::vector(void) : _array(nullptr), _capacity(0), _size(0) {}

  template <typename T>
  inline vector<T>::vector(size_type size, value_type value) {
    if (size >= 0) {
      _capacity = _size = size;
      _array = new value_type [_capacity];
      std::fill(begin(), end(), value);
    } else {
      throw std::length_error("Trying to construct vector of size: "
                              + std::to_string(size) + ".");
    }
  }

  template <typename T>
  inline vector<T>::vector(std::initializer_list<value_type> il) {
    _capacity = _size = il.size();
    _array = new value_type [_capacity];
    std::copy(il.begin(), il.end(), begin());
  }

  template <typename T>
  inline vector<T>::vector(vector const & other) {
    // allocate new resources and copy contents of other to this
    _capacity = other._capacity;
    _size = other._size;
    _array = new value_type [_capacity];
    std::copy(other.begin(), other.end(), begin());
  }

  template <typename T>
  inline vector<T>::vector(vector && other) : vector() {
    swap(*this, other);
  }

  template <typename T>
  inline vector<T>::~vector(void) {
    delete [] _array;
  }

  template <typename T>
  inline auto vector<T>::operator=(vector<value_type> other)
    -> vector<value_type> & {
    swap(*this, other);
    return *this;
  }

  template <typename T>
  inline void vector<T>::push_back(value_type val) {
    insert(size(), val);
  }

  template <typename T>
  template <typename... Args>
  inline void vector<T>::emplace_back(Args... args) {
    emplace(size(), args...);
  }

  template <typename T>
  inline void vector<T>::insert(size_type pos, value_type value) {
    if (pos < 0 || pos > _size) {
      throw std::out_of_range("Trying to insert element at position: "
                              + std::to_string(pos) + ", in a vector "
                              "of size: " + std::to_string(size())
                              + ".");
    }
    if (_size == _capacity) {
      reserve(_capacity ? _capacity*2 : 1);
    }
    _size++;
    std::rotate(rend(), rend()+1, rbegin()-pos);
    _array[pos] = value;
  }

  template <typename T>
  template <typename... Args>
  inline void vector<T>::emplace(size_type pos, Args... args) {
    insert(pos, value_type(args...));
  }

  template <typename T>
  inline void vector<T>::clear(void) {
    _size = 0;
  }

  template <typename T>
  inline void vector<T>::erase(size_type pos) {
    if (pos < 0 || pos >= _size) {
      throw std::out_of_range("Trying to erase element at position: "
                              + std::to_string(pos) + ", in a vector "
                              "of size: " + std::to_string(size())
                              + ".");
    }
    erase(begin()+pos, begin()+pos+1);
  }

  template <typename T>
  inline void vector<T>::erase(iterator from, iterator to) {
    if (to > from) {
      std::rotate(from, to, end());
      _size -= std::distance(from, to);
    } else if (from > to) {
      erase(to, end());
      erase(begin(), from);
    } else {
      // empty range, do nothing
    }
  }

  template <typename T>
  inline void vector<T>::fill(const_reference value) {
    std::fill(begin(), end(), value);
  }

  template <typename T>
  inline auto vector<T>::size(void) const
    -> size_type {
    return _size;
  }

  template <typename T>
  inline auto vector<T>::capacity(void) const
    -> size_type {
    return _capacity;
  }

  template <typename T>
  inline auto vector<T>::find(const_reference ref)
    -> iterator {
    return std::find(begin(), end(), ref);
  }

  template <typename T>
  inline auto vector<T>::find(const_reference ref) const
    -> const_iterator {
    return std::find(begin(), end(), ref);
  }

  template <typename T>
  inline auto vector<T>::begin(void)
    -> iterator {
    return _array;
  }

  template <typename T>
  inline auto vector<T>::end(void)
    -> iterator {
    return _array+_size;
  }

  template <typename T>
  inline auto vector<T>::rbegin(void)
    -> reverse_iterator {
    return reverse_iterator(begin());
  }

  template <typename T>
  inline auto vector<T>::rend(void)
    -> reverse_iterator {
    return reverse_iterator(end());
  }

  template <typename T>
  inline auto vector<T>::begin(void) const
    -> const_iterator {
    return _array;
  }

  template <typename T>
  inline auto vector<T>::end(void) const
    -> const_iterator {
    return _array+_size;
  }

  template <typename T>
  inline auto vector<T>::rbegin(void) const
    -> const_reverse_iterator {
    return const_reverse_iterator(begin());
  }

  template <typename T>
  inline auto vector<T>::rend(void) const
    -> const_reverse_iterator {
    return const_reverse_iterator(end());
  }

  template <typename T>
  inline auto vector<T>::operator[](size_type pos)
    -> reference {
    if (pos < 0 || pos >= _size) {
      throw std::out_of_range("Trying to access element at position: "
                              + std::to_string(pos) + ", in a vector "
                              "of size: " + std::to_string(size())
                              + ".");
    }
    return _array[pos];
  }

  template <typename T>
  inline auto vector<T>::operator[](size_type pos) const
    -> const_reference {
    if (pos < 0 || pos >= _size) {
      throw std::out_of_range("Trying to access element at position: "
                              + std::to_string(pos) + ", in a vector "
                              "of size: " + std::to_string(size())
                              + ".");
    }
    return _array[pos];
  }

  template <typename T>
  inline void vector<T>::reserve(size_type new_capacity) {
    if (new_capacity > _capacity) {
      pointer const new_array = new value_type [new_capacity];
      std::copy(begin(), end(), new_array);
      delete [] _array;
      _array = new_array;
      _capacity = new_capacity;
    }
  }

  template <typename T>
  void swap(vector<T> & lhs, vector<T> & rhs) {
    std::swap(lhs._capacity, rhs._capacity);
    std::swap(lhs._size, rhs._size);
    std::swap(lhs._array, rhs._array);
  }
}
