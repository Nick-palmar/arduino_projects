#include "sequential_list.h"

SequentialList::SequentialList(unsigned int cap)
{
    // initialize an empty array pointer of size cap
    data_ = new DataType[cap];
    // set the cap with the given capacity
    capacity_ = cap;

    // the list is currently empty
    size_ = 0;
}

SequentialList::~SequentialList()
{
    // note: capacity_ and size_ member variables are statically allocated so they are deallocated by default
    // manually deallocate memory for the entire array pointer and remove the pointer for data_
    delete [] data_;
    data_ = nullptr;
}

unsigned int SequentialList::size() const
{
    // return the member variable for size indicating number of elements
    return size_;
}

unsigned int SequentialList::capacity() const
{
    // return the capacity of the list from the member variable
    return capacity_;
}

bool SequentialList::empty() const
{
    // check size, if it is zero it must be empty
    return size_ == 0;
}

bool SequentialList::full() const {
    // check if the size is equal to the capacity, this means full
    return size_ == capacity_;
}

SequentialList::DataType SequentialList::select(unsigned int index) const
{
    // deal with indices outside of the list size
    if (index < 0 || index >= size_) {
        // return the last list element if the list contains data
        if (size_ > 0)
            return data_[size_-1];
        // list has no data - this operation is undefined
        else {
            // arbitrarily return the element at position 0 to satisfy the function requirements
            return data_[0];
        }

    }

    // the index is valid - return the requested input element
    return data_[index];
}

unsigned int SequentialList::search(DataType val) const
{
    // loop through all values in data_ and return index if found
    for (unsigned int i = 0; i < size_; i++) {
        if (data_[i] == val) {
            // value found, return the index
            return i;
        }
    }

    // loop has not returned any values, return size if no value exists
    return size_;
}

bool SequentialList::insert(DataType val, unsigned int index)
{
    // check for an invalid insert
    if (index < 0 || index > size_) {
        return false;
    }

    // check if the list if full
    else if (size_ == capacity_) {
        return false;
    }

    // shift the elements before the insertion
    // do this by moving each element starting from the last until the position of insertion
    for (unsigned int i = size_; i > index; i--) {
        data_[i] = data_[i-1];
    }

    // finally, insert into the position
    data_[index] = val;
    // after the insertion, update the list size
    size_ += 1;
    return true;
}

bool SequentialList::insert_ordered(DataType val) {
  for (unsigned int i = 0; i < size_; i++) {
    if (val < data_[i])
      return insert(val, i); 
  }
  // it is the largest value, insert it at the end
  return insert(val, size_);
}

bool SequentialList::insert_front(DataType val) {
    // check if the array is full, fail the insertion
    if (size_ == capacity_)
        return false;

    // shift all elements forward by 1 to prepare for insertion
    for (unsigned int i = size_; i > 0; i--) {
        data_[i] = data_[i-1];
    }

    // insert at index 0
    data_[0] = val;
    // increase the size after inserting
    size_ += 1;
    return true;
}

bool SequentialList::insert_back(DataType val)
{
    // check if the array is full, fail the insertion
    if (size_ == capacity_)
        return false;

    // insert at the position of the size; this is the tail position of the list
    data_[size_] = val;
    // after the insertion, update the list size
    size_ += 1;
    return true;
}

bool SequentialList::remove(unsigned int index)
{
    // check if the index is not in the current size of the array
    if (index < 0 || index >= size_)
        return false;

    // check if the list is empty (nothing to remove)
    else if (size_ == 0)
        return false;

    // loop from the index until before the size and remove these elements
    // remove an element by changing it with the next element and decreasing the size at the end
    // this way, we ignore any indices after size (and overwrite the requested index)
    for (unsigned int i = index; i < size_-1; i++) {
        data_[i] = data_[i+1];
    }

    // decrease the size of the list to 'remove' the element
    size_--;
    return true;
}

bool SequentialList::remove_front()
{
    // check if the list is empty
    if (size_ == 0)
        return false;

    // start from the beginning of the array and change the value with the value at the next index
    for (unsigned int i = 0; i < size_-1; i++) {
        data_[i] = data_[i+1];
    }

    // decrease the size to 'remove' last element
    size_--;
    return true;
}

bool SequentialList::remove_back()
{
    // check if the list is empty
    if (size_ == 0)
        return false;

    // since it is the last element, no shifting is required
    // simply decrease size to 'remove' the element
    size_--;
    return true;
}

bool SequentialList::replace(unsigned int index, DataType val)
{
    // check if the list is empty
    if (size_ == 0)
        return false;

    // check if the index of insertion does not exist
    else if (index < 0 || index >= size_)
        return false;

    // replace index must be valid and the list contains elements, replace
    data_[index] = val;
    // do not change size since it is staying the same
    return true;
}
