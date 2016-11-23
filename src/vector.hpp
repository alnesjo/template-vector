/* vector.hpp - Robert Alnesjö <rhalnesjo@gmail.com> */

#pragma once

#include <cstddef>
#include <stdexcept>
#include <initializer_list>
#include <type_traits>
#include <algorithm>

namespace alnesjo {

  // Shifts elements ranging between begin and end by one step
  // TODO: Should probably be moved to another header

  template <typename T>
  class vector {
    template <typename U> friend void swap(vector<U> &, vector<U> &);
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
  public:

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

    // Insert value at the end of vector.
    void push_back(value_type value);

    // Insert value at pos. Will push back end-pos elements.
    void insert(size_type pos, value_type value);

    // Clear vector such stat it's size becomes zero.
    void clear(void);

    // Erase vector entry at pos and pull back end-pos elements.
    void erase(size_type pos);

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

    // Reallocates underlying storage to capacity. Behavior is undefined
    // for capacity less than vector size.
    void _realloc(size_type capacity);
  };

  template <typename T>
  void swap(vector<T> & lhs, vector<T> & rhs) {
    std::swap(lhs._capacity, rhs._capacity);
    std::swap(lhs._size, rhs._size);
    std::swap(lhs._array, rhs._array);
  }

  template <typename T>
  inline vector<T>::vector(void) : _array(nullptr), _capacity(0), _size(0) {}

  template <typename T>
  inline vector<T>::vector(size_type n, value_type val) {
    _capacity = _size = n;
    _array = new value_type [_capacity];
    std::fill(begin(), end(), val);
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
    insert(_size, val);
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
      _realloc(_capacity ? _capacity*2 : 1);
    }
    _size++;
    std::rotate(rend(), rend()+1, rbegin()-pos);
    _array[pos] = value;
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
    std::rotate(begin()+pos, begin()+pos+1, end());
    _size--;
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
    /*
    auto tar = end();
    for (auto it = begin(); it != tar; it++) {
      if (*it == ref) {
        tar = it;
        break;
      }
    }
    return tar;
    */
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
      throw std::out_of_range("Trying to acess element at position: "
                              + std::to_string(pos) + ", in a vector "
                              "of size: " + std::to_string(size())
                              + ".");
    }
    return _array[pos];
  }

  template <typename T>
  inline auto vector<T>::operator[](size_type index) const
    -> const_reference {
    if (index < 0 || index >= _size) {
      throw std::out_of_range("OUT OF RANGE");
    }
    return _array[index];
  }

  template <typename T>
  inline void vector<T>::_realloc(size_type new_capacity) {
    pointer const new_array = new value_type [new_capacity];
    std::copy(begin(), end(), new_array);
    delete [] _array;
    _array = new_array;
    _capacity = new_capacity;
  }
}
