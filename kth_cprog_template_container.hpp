/* kth_cprog_template_container.hpp - Robert Alnesjö <alnesjo@kth.se> */

#ifndef VECTOR_H__CPROG
#define VECTOR_H__CPROG

#include <cstddef>
#include <stdexcept>
#include <initializer_list>
#include <type_traits>

template <typename T>
class Vector
{
     static_assert(std::is_move_constructible<T>::value &&
		   std::is_move_assignable<T>::value,
		   "Vector requires value_type to be move "
		   "-constructible and -assignable");
public:
     typedef T                                                        value_type;
     typedef std::size_t                                               size_type;
     typedef value_type &                                              reference;
     typedef value_type const &                                  const_reference;
     typedef value_type *                                                pointer;
     typedef value_type const *                                    const_pointer;
     typedef pointer                                                    iterator;
     typedef const_pointer                                        const_iterator;
     typedef std::reverse_iterator<iterator>                    reverse_iterator;
     typedef std::reverse_iterator<const_iterator>        const_reverse_iterator;
     
     void push_back(value_type val)
	  { insert(_size, val); };
     void insert(size_type, value_type);
     void clear(void)
	  { _size = 0; };
     void erase(size_type);
     size_type size(void) const
	  { return _size; };
     size_type capacity(void) const
	  { return _capacity; };

     iterator find(const_reference);
     iterator begin(void)
	  { return _array; };
     iterator end(void)
	  { return _array+_size; };
     reverse_iterator rbegin(void)
	  { return reverse_iterator(end()); };
     reverse_iterator rend(void)
	  { return reverse_iterator(begin()); };

     const_iterator find(const_reference) const;
     const_iterator begin(void) const
	  { return _array; };
     const_iterator end(void) const
	  { return _array+_size; };
     const_reverse_iterator rbegin(void) const
	  { return const_reverse_iterator(end()); };
     const_reverse_iterator rend(void) const
	  { return const_reverse_iterator(begin()); };

     void reset(void);     

     reference operator[](size_type);
     const_reference operator[](size_type) const;

     Vector & operator=(Vector const &);
     Vector & operator=(Vector &&);

     Vector(void) : _array(nullptr), _capacity(0), _size(0) 
	  {};
     Vector(size_type, value_type);
     Vector(Vector const & src)
	  { _copy(src); };
     Vector(Vector && src)
	  { _move(std::move(src)); };
     Vector(std::initializer_list<value_type>);
     ~Vector(void)
	  { if(_capacity > 0) delete [] _array; };
          
protected:
     pointer                      _array;
     size_type                 _capacity;
     size_type                     _size;

private:
     void _copy(Vector const &);
     void _move(Vector &&);
     void _realloc(size_type);
};

/* Member functions */

template <typename T>
inline void Vector<T>::insert(size_type index, value_type val)
{
     /* reallocate resources to two times the new size */
     if(_size == _capacity) _realloc(_capacity ? _capacity*2 : 1);
     if(index < _size++)
     {
	  /* push back trailing elements */
	  reverse_iterator head, tail, tar = rend() - index;
	  head = tail = rbegin();
	  for(head++; head != tar; *tail++ = *head++);
     }
     /* operator[] may throw out_of_range exception */
     (*this)[index] = val;
}

template <typename T>
inline void Vector<T>::erase(size_type index)
{
     (*this)[index]; /* may throw out_of_range */
     if(index < _size-1);
     {
	  iterator head, tail, tar = end();
	  head = tail = begin() + index;
	  for(head++; head != tar; *tail++ = *head++);
     }
     _size--;
}

template <typename T>
inline auto Vector<T>::find(const_reference ref)
     -> iterator
{
     auto tar = end();
     for(auto it = begin(); it != tar; it++)
     {
	  if(*it == ref)
	  {
	       tar = it;
	       break;
	  }
     }
     return tar;
}

template <typename T>
inline auto Vector<T>::find(const_reference ref) const
     -> const_iterator
{
     auto tar = end();
     for(auto it = begin(); it != tar; it++)
     {
	  if(*it == ref)
	  {
	       tar = it;
	       break;
	  }
     }
     return tar;
}

template <typename T>
inline void Vector<T>::reset(void)
{
     for(auto & elem : *this) elem = value_type();
}

template <typename T>
inline void Vector<T>::_copy(Vector<value_type> const & other)
{
     /* allocate new resources and copy contents of other to this */
     _capacity = other._capacity;
     _size = other._size;
     if(_capacity > 0)
     {
	  _array = new value_type [_capacity];
	  iterator dest = begin();
	  const_iterator src = other.begin();
	  while(dest != end()) *dest++ = *src++;
     }
     else _array = nullptr;
}

template <typename T>
inline void Vector<T>::_move(Vector<value_type> && other)
{
     /* make a shallow copy of other and clear it's values */
     _capacity = other._capacity;
     _size = other._size;
     _array = other._array;
     other._capacity = 0;
     other._size = 0;
     other._array = nullptr;
}

template <typename T>
inline void Vector<T>::_realloc(size_type new_capacity)
{
     pointer new_array = new value_type [new_capacity];
     iterator it = new_array;
     for(auto & elem : *this) *it++ = elem;
     if(_capacity > 0) delete [] _array;
     _array = new_array;
     _capacity = new_capacity;
}

/* Operators */

template <typename T>
inline auto Vector<T>::operator[](size_type index)
     -> reference
{
     if(index < 0 || index >= _size)
	  throw std::out_of_range("OUT OF RANGE");
     return _array[index];
}

template <typename T>
inline auto Vector<T>::operator[](size_type index) const
     -> const_reference
{
     if(index < 0 || index >= _size)
	  throw std::out_of_range("OUT OF RANGE");
     return _array[index];
}

template <typename T>
inline auto Vector<T>::operator=(Vector<value_type> const & other)
     -> Vector<value_type> &
{
     if(this != &other)
     {
	  /* free existing dynamically allocated data */
	  if(_capacity > 0) delete [] _array;
	  _copy(other);
     }
     return *this;
}

template <typename T>
inline auto Vector<T>::operator=(Vector<value_type> && other)
     -> Vector<value_type> &
{
     if(this != &other)
     {
	  /* free existing dynamically allocated data */
	  if(_capacity > 0) delete [] _array;
	  _move(std::move(other));
     }
     return *this;
}

/* Constructors */

template <typename T>
inline Vector<T>::Vector(size_type n,
			 value_type val = value_type())
{
     _capacity = _size = n;
     if(_capacity > 0)
     {
	  _array = new value_type [_capacity];
	  for(auto & elem : *this) elem = val;
     }
     else _array = nullptr;
}

template <typename T>
inline Vector<T>::Vector(std::initializer_list<value_type> il)
{
     _capacity = _size = il.size();
     if(_capacity > 0)
     {
	  _array = new value_type [_capacity];
	  std::copy(il.begin(), il.end(), _array);
     }
     else _array = nullptr;
}

#endif
