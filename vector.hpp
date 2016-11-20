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

          void                   push_back(value_type);
          void                   insert(size_type, value_type);
          void                   clear(void);
          void                   erase(size_type);
          size_type              size(void) const;
          size_type              capacity(void) const;

          iterator               find(const_reference);
          iterator               begin(void);
          iterator               end(void);
          reverse_iterator       rbegin(void);
          reverse_iterator       rend(void);

          const_iterator         find(const_reference) const;
          const_iterator         begin(void) const;
          const_iterator         end(void) const;
          const_reverse_iterator rbegin(void) const;
          const_reverse_iterator rend(void) const;

          void                   reset(void);

          reference              operator[](size_type);
          const_reference        operator[](size_type) const;

          Vector &               operator=(Vector const &);
          Vector &               operator=(Vector &&);

                                 Vector(void);
                                 Vector(size_type, value_type v = value_type());
                                 Vector(Vector const &);
                                 Vector(Vector &&);
                                 Vector(std::initializer_list<value_type>);
          ~Vector(void);
     protected:
          pointer                _array;
          size_type              _capacity;
          size_type              _size;

     private:
          void                   _copy(Vector const &);
          void                   _move(Vector &&);
          void                   _realloc(size_type);
     };

/* Member functions */


     template <typename T>
     inline void Vector<T>::push_back(value_type val) {
          insert(_size, val);
     }

     template <typename T>
     inline void Vector<T>::insert(size_type index, value_type val) {
          /* reallocate resources to two times the new size */
          if (_size == _capacity) {
               _realloc(_capacity ? _capacity*2 : 1);
          }
          if (index < _size++) {
               /* push back trailing elements */
               iterator head, tail, tar = end() - index;
               head = tail = begin();
               for (head++; head != tar; *tail++ = *head++);
          }
          /* operator[] may throw out_of_range exception */
          (*this)[index] = val;
     }

     template <typename T>
     inline void Vector<T>::clear(void) {
          _size = 0;
     }

     template <typename T>
     inline void Vector<T>::erase(size_type index) {
          (*this)[index]; /* may throw out_of_range */
          // If statement redundant if operator[] throws
          if (index < _size-1) {
               iterator head, tail, tar = end();
               head = tail = begin() + index;
               for (head++; head != tar; *tail++ = *head++);
          }
          _size--;
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
     inline void Vector<T>::reset(void) {
          for (auto & elem : *this) {
               elem = value_type();
          }
     }

     template <typename T>
     inline void Vector<T>::_copy(Vector<value_type> const & other) {
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
     inline void Vector<T>::_move(Vector<value_type> && other) {
          /* make a shallow copy of other and clear it's values */
          _capacity = other._capacity;
          _size = other._size;
          _array = other._array;
          other._capacity = 0;
          other._size = 0;
          other._array = nullptr;
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

/* Operators */

     template <typename T>
     inline auto Vector<T>::operator[](size_type index)
          -> reference {
          if (index < 0 || index >= _size) {
               throw std::out_of_range("OUT OF RANGE");
          }
          return _array[index];
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
     inline auto Vector<T>::operator=(Vector<value_type> const & other)
          -> Vector<value_type> & {
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
     inline auto Vector<T>::operator=(Vector<value_type> && other)
          -> Vector<value_type> & {
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
     inline Vector<T>::Vector(Vector const & src) {
          _copy(src);
     }

     template <typename T>
     inline Vector<T>::Vector(Vector && src) {
          _move(std::move(src));
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
     inline Vector<T>::~Vector(void) {
          if (_capacity > 0) {
               delete [] _array;
          }
     }
}
