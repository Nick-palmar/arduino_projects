#include "queue.h"

// assign the empty queue a value of -9999
CircularQueue::QueueItem const CircularQueue::EMPTY_QUEUE = -9999;

// default constructor
//CircularQueue::CircularQueue() {
//    // use capacity 16 for default constructor of circular queue
//    capacity_ = 16;
//
//    items_ = new int[capacity_];
//    size_ = 0;
//    head_ = 0;
//    tail_ = 0;
//
//}

void CircularQueue::printQ(int startIdx, int endIdx) const {

    Serial.print("[");
    // iterate the size of the list from ifront
    for (int i = head_ + startIdx; i < (head_ + endIdx); i++) {
        // account for ciruclar array condition
        int iCircle = i % capacity_;

        if (i+1 != (head_ + size_)) {
            Serial.print(items_[iCircle]);
            Serial.print(", ");
        }
        else
            Serial.print(items_[iCircle]);

    }
    Serial.println("]");
}

CircularQueue::CircularQueue(unsigned int capacity) {

    // set capacity according to given limit
    capacity_ = capacity;

    items_ = new int[capacity_];
    size_ = 0;
    head_ = 0;
    tail_ = 0;
}

CircularQueue::~CircularQueue() {

    // delete dynamically allocated items variable
    delete [] items_;
}

bool CircularQueue::empty() const {
    return size_ == 0;
}

bool CircularQueue::full() const {
    return size_ == capacity_;
}

int CircularQueue::size() const {
    return size_;
}

bool CircularQueue::enqueue(QueueItem value) {

    // check if q is full
    if (size_ == capacity_)
        return false;

    // assume tail is pointing to a valid position
    items_[tail_] = value;

    // move tail while respecting circular property
    tail_ = (tail_+1) % capacity_;
    size_++;

    return true;

}

CircularQueue::QueueItem CircularQueue::dequeue() {
    // deal with empty q
    if (empty())
        return EMPTY_QUEUE;

    // q is not empty
    int frontItem = items_[head_];

    // move head index
    head_ = (head_ + 1) % capacity_;
    size_--;
    return frontItem;

}

CircularQueue::QueueItem CircularQueue::peek() const {

    // check if the queue is empty
    if (size_ == 0)
        return EMPTY_QUEUE;

    // not empty
    return items_[head_];
}
