/* ##########################################################################
# #
# Minheap.h #
# Man Nguyen #
# Compression #
# 05/11/19 #
# #
# #
  A template file for a minheap data structure that uses an array with
  item T.
# #
########################################################################## */
#ifndef PQ
#define PQ

template <typename T>
class minheap
{
public:
  minheap(int (*cmp)(const T &, const T &));
  void add(const T & item); // adds element
  void remove(); // removes front
  T front() const; // returns front
  size_t size() const; // returns size

private:
  T *_data; // array of T
  size_t _capacity; // _capacity of array
  size_t _size; // number of elements in minheap
  int (*_cmp)(const T&a, const T&b);
  void _reheap_down(size_t k);
  void _reheap_up(size_t k);
  void _swap(size_t x, size_t y); // swap queue elements
  size_t _find_parent(size_t k); // find parent of child
  size_t _left(size_t k); // find left child
  size_t _right(size_t k); // find right child
  void _grow(); // makes the array bigger
};

#include "minheap.cc"

#endif
