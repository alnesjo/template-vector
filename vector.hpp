/* vector.hpp - Robert Alnesjö <rhalnesjo@gmail.com> */

#pragma once

#include <cstddef>
#include <stdexcept>
#include <initializer_list>
#include <type_traits>

namespace alnesjo {

  template <typename T>
  class vector {
    static_assert(std::is_move_constructible<T>::value &&
                  std::is_move_assignable<T>::value,
                  "vector requires value_type to be move -constructible "
                  "and -assignable");
  public:
    typedef T                                     value_type;
    typedef std::size_t                           size_type;
    typedef value_type &                          reference;
    typedef value_type const &                    const_reference;
    typedef value_type *                          pointer;
    typedef value_type const *                    const_pointer;
    typedef pointer                               iterator;
    typedef const_pointer                         const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    void                   push_back(value_type value);
                           /* Insert value at the end of vector.
                            */
    void                   insert(size_type pos, value_type value);
                           /* Insert value at pos. Will push back end-pos
                            * elements.
                            */
    void                   clear(void);
                           /* Clear vector such stat it's size becomes zero.
                            */
    void                   erase(size_type pos);
                           /* Erase vector entry at pos and pull back end-pos
                            * elements.
                            */
    size_type              size(void) const;
                           /* The number of elements contained in vector.
                            */
    size_type              capacity(void) const;
                           /* The number of elements that can be held in
                            * currently allocated storage.
                            */
    iterator               find(const_reference value);
    iterator               begin(void);
    iterator               end(void);
    reverse_iterator       rbegin(void);
    reverse_iterator       rend(void);

    const_iterator         find(const_reference value) const;
    const_iterator         begin(void) const;
    const_iterator         end(void) const;
    const_reverse_iterator rbegin(void) const;
    const_reverse_iterator rend(void) const;

    void                   reset(void);

    reference              operator[](size_type pos);
    const_reference        operator[](size_type pos) const;

    vector &               operator=(vector const & other);
    vector &               operator=(vector && other);

                           vector(void);
                           vector(size_type count,
                                  value_type value = value_type());
                           vector(vector const & other);
                           vector(vector && other);
                           vector(std::initializer_list<value_type>);
                           ~vector(void);
  protected:
    pointer                _array;
    size_type              _capacity;
    size_type              _size;

  private:
    void                   _copy(vector const &);
    void                   _move(vector &&);
    void                   _realloc(size_type);
  };

  /* Member functions */


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
      /* allocate more space */
      _realloc(_capacity ? _capacity*2 : 1);
    }
    ++_size;
    reverse_iterator head, tail, tar = rbegin() - pos;
    head = tail = rend();
    /* push back trailing elements */
    for (head++; head != tar; *tail++ = *head++);
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
    iterator head, tail, tar = end();
    head = tail = begin() + pos;
    for (head++; head != tar; *tail++ = *head++);
    _size--;
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
  inline auto vector<T>::find(const_reference ref) const
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
  inline void vector<T>::reset(void) {
    for (auto & elem : *this) {
      elem = value_type();
    }
  }

  template <typename T>
  inline void vector<T>::_copy(vector<value_type> const & other) {
    /* allocate new resources and copy contents of other to this */
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
  inline void vector<T>::_move(vector<value_type> && other) {
    /* make a shallow copy of other and clear it's values */
    _capacity = other._capacity;
    _size = other._size;
    _array = other._array;
    other._capacity = 0;
    other._size = 0;
    other._array = nullptr;
  }

  template <typename T>
  inline void vector<T>::_realloc(size_type new_capacity) {
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

  /* Operators */

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
  inline auto vector<T>::operator=(vector<value_type> const & other)
    -> vector<value_type> & {
    if (this != &other) {
      /* free existing dynamically allocated data */
      if (_capacity > 0) {
        delete [] _array;
      }
      _copy(other);
    }
    return *this;
  }

  template <typename T>
  inline auto vector<T>::operator=(vector<value_type> && other)
    -> vector<value_type> & {
    if (this != &other) {
      /* free existing dynamically allocated data */
      if (_capacity > 0) {
        delete [] _array;
      }
      _move(std::move(other));
    }
    return *this;
  }

  /* Constructors */

  template <typename T>
  inline vector<T>::vector(void) : _array(nullptr), _capacity(0), _size(0) {}

  template <typename T>
  inline vector<T>::vector(size_type n, value_type val) {
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
  inline vector<T>::vector(vector const & src) {
    _copy(src);
  }

  template <typename T>
  inline vector<T>::vector(vector && src) {
    _move(std::move(src));
  }

  template <typename T>
  inline vector<T>::vector(std::initializer_list<value_type> il) {
    _capacity = _size = il.size();
    if (_capacity > 0) {
      _array = new value_type [_capacity];
      std::copy(il.begin(), il.end(), _array);
    } else {
      _array = nullptr;
    }
  }

  template <typename T>
  inline vector<T>::~vector(void) {
    if (_capacity > 0) {
      delete [] _array;
    }
  }
}
