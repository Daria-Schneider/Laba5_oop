#include <memory>

namespace container {

template <typename T>
DynamicQueue<T>::DynamicQueue(std::pmr::memory_resource* resource) 
    : node_allocator(resource), front_node(nullptr), back_node(nullptr), element_count(0) {}

template <typename T>
DynamicQueue<T>::~DynamicQueue() {
    CleanupResources();
}

template <typename T>
DynamicQueue<T>::DynamicQueue(DynamicQueue&& other) noexcept
    : node_allocator(other.node_allocator),
      front_node(other.front_node),
      back_node(other.back_node),
      element_count(other.element_count) {
    other.front_node = nullptr;
    other.back_node = nullptr;
    other.element_count = 0;
}

template <typename T>
DynamicQueue<T>& DynamicQueue<T>::operator=(DynamicQueue&& other) noexcept {
    if (this != &other) {
        CleanupResources();
        
        front_node = other.front_node;
        back_node = other.back_node;
        element_count = other.element_count;
        node_allocator = other.node_allocator;
        
        other.front_node = nullptr;
        other.back_node = nullptr;
        other.element_count = 0;
    }
    return *this;
}

template <typename T>
template<typename... Args>
void DynamicQueue<T>::Enqueue(Args&&... args) {
    QueueNode* new_node = node_allocator.allocate(1);
    try {
        node_allocator.construct(new_node, std::forward<Args>(args)...);
    } catch (...) {
        node_allocator.deallocate(new_node, 1);
        throw;
    }
    
    if (IsEmpty()) {
        front_node = back_node = new_node;
    } else {
        back_node->next_node = new_node;
        back_node = new_node;
    }
    ++element_count;
}

template <typename T>
void DynamicQueue<T>::Dequeue() {
    if (IsEmpty()) {
        throw container_exceptions::EmptyQueueException("Cannot dequeue from empty queue");
    }
    
    QueueNode* old_front = front_node;
    front_node = front_node->next_node;
    
    if (front_node == nullptr) {
        back_node = nullptr;
    }
    
    node_allocator.destroy(old_front);
    node_allocator.deallocate(old_front, 1);
    --element_count;
}

template <typename T>
T& DynamicQueue<T>::PeekFront() {
    if (IsEmpty()) {
        throw container_exceptions::EmptyQueueException("Cannot peek front of empty queue");
    }
    return front_node->data;
}

template <typename T>
T& DynamicQueue<T>::PeekBack() {
    if (IsEmpty()) {
        throw container_exceptions::EmptyQueueException("Cannot peek back of empty queue");
    }
    return back_node->data;
}

template <typename T>
bool DynamicQueue<T>::IsEmpty() const noexcept {
    return front_node == nullptr;
}

template <typename T>
std::size_t DynamicQueue<T>::GetSize() const noexcept {
    return element_count;
}

template <typename T>
typename DynamicQueue<T>::QueueIterator DynamicQueue<T>::Begin() noexcept {
    return QueueIterator(front_node);
}

template <typename T>
typename DynamicQueue<T>::QueueIterator DynamicQueue<T>::End() noexcept {
    return QueueIterator(nullptr);
}

template <typename T>
void DynamicQueue<T>::Clear() noexcept {
    CleanupResources();
    front_node = nullptr;
    back_node = nullptr;
    element_count = 0;
}

template <typename T>
void DynamicQueue<T>::CleanupResources() {
    while (!IsEmpty()) {
        QueueNode* current = front_node;
        front_node = front_node->next_node;
        node_allocator.destroy(current);
        node_allocator.deallocate(current, 1);
    }
}

}