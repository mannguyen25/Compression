/* ##########################################################################
# #
# Minheap.cc #
# Man Nguyen #
# Compression #
# 05/11/19 #
# #
    A template file for a minheap data structure that uses an array with
    item T.
# #
########################################################################## */
template <typename T>
minheap<T>::minheap(int (*cmp)(const T &, const T &))
{
  _capacity = 256;
  _size = 0;
  _cmp = cmp;
  _data = new T[_capacity];
}

template <typename T>
size_t minheap<T>::_find_parent(size_t k)
{ /* given a child index, returns the parent */
  return (k - 1) / 2;
}

template <typename T>
size_t minheap<T>::_left(size_t k)
{ /* given a parent index, returns the left child */
  return k * 2 + 1;
}

template <typename T>
size_t minheap<T>::_right(size_t k)
{ /* given a parent index, returns the right child */
  return k * 2 + 2;
}

template <typename T>
void minheap<T>::_swap(size_t x, size_t y)
{ /* swaps to index values
     Pre: Tree is not empty
     Post: Values in two indexes are swapped */
    T temp = _data[x];
    _data[x] = _data[y];
    _data[y] = temp;
}

template <typename T>
void minheap<T>::_reheap_up(size_t k)
{ /* Reheaps the complete tree up by swapping parent
     and child values stored in the array
     Pre: k != _capacity
     Post: tree follows the rules of minheap */
  while (k > 0 && _cmp(_data[k], _data[_find_parent(k)]) < 0) {
    _swap(k, _find_parent(k));
    k = _find_parent(k);
  }
}


template <typename T>
void minheap<T>::_reheap_down(size_t k)
{ /* Reheaps the complete tree down by swapping parent
     and child values stored in the array
     Pre: size != 0
     Post: tree follows the rules of minheap */

  // return when child is out of range
  if (_left(k) >= _size) return;

  // if it's a leaf return
  if (!_data[_left(k)]) return;
  else
  {
    size_t child = _left(k);

    // if there is a right child and it's less than the left child
    // set child equal to right
    if (_data[_right(k)] && _cmp(_data[_right(k)], _data[child]) < 0)
    child = _right(k);

    // when the parent is greater than child, swap
    if (_cmp(_data[k], _data[child]) > 0) {
      _swap(k, child);
      _reheap_down(child);
    }
  }
}

template <typename T>
void minheap<T>::_grow()
{ /* Grows the array
     Pre: _size == _capacity
     Post: _capacity *= 2 */

   size_t new_capacity = _capacity * 2;
   T *new_data = new T[new_capacity];
   for (size_t i = 0; i < _size; i++)
     new_data[i] = _data[i];
   delete [] _data;
   _data = new_data;
   _capacity = new_capacity;
}

template <typename T>
void minheap<T>::add(const T & item)
{ /* Adds element T to a array
     Pre: _size != _capacity
     Post: _size += 1 and element add to the array */

  // set last item in queue equal to item then increase size
  _data[_size] = item;
  _size++;

  // reheap-up until it becomes a minheap
  _reheap_up(_size - 1);
  if (_size == _capacity) _grow();
}


template <typename T>
void minheap<T>::remove()
{ /* Removes element T in array
     Pre: _size != 0
     Post: _size -= 1 and element is removed from the array */

  _data[0] = _data[--_size];  // note well, pre-decrement operator!
  _reheap_down(0);
}

template <typename T>
T minheap<T>::front() const
{ /* returns front of array */
  return _data[0];
}

template <typename T>
size_t minheap<T>::size() const
{ /* returns number of elements in array */
  return _size;
}
