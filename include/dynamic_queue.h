#pragma once
#include <memory_resource>
#include <iterator>
#include <cstddef>
#include "queue_exceptions.h"

namespace container {

template <typename T>
class DynamicQueue {
private:
    struct QueueNode {
        T data;
        QueueNode* next_node;
        
        template<typename... Args>
        explicit QueueNode(Args&&... args) 
            : data(std::forward<Args>(args)...), next_node(nullptr) {}
    };

public:
    class QueueIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        explicit QueueIterator(QueueNode* node = nullptr) : current_node(node) {}
        
        bool operator==(const QueueIterator& other) const { 
            return current_node == other.current_node; 
        }
        
        bool operator!=(const QueueIterator& other) const { 
            return !(*this == other); 
        }
        
        reference operator*() const { 
            return current_node->data; 
        }
        
        pointer operator->() const { 
            return &current_node->data; 
        }
        
        QueueIterator& operator++() {
            if (current_node) {
                current_node = current_node->next_node;
            }
            return *this;
        }
        
        QueueIterator operator++(int) {
            QueueIterator temp = *this;
            ++(*this);
            return temp;
        }

    private:
        QueueNode* current_node;
    };

    explicit DynamicQueue(std::pmr::memory_resource* resource = std::pmr::get_default_resource());
    ~DynamicQueue();
    
    DynamicQueue(const DynamicQueue&) = delete;
    DynamicQueue& operator=(const DynamicQueue&) = delete;
    
    DynamicQueue(DynamicQueue&& other) noexcept;
    DynamicQueue& operator=(DynamicQueue&& other) noexcept;

    template<typename... Args>
    void Enqueue(Args&&... args);
    
    void Dequeue();
    
    T& PeekFront();
    T& PeekBack();
    
    bool IsEmpty() const noexcept;
    std::size_t GetSize() const noexcept;
    
    QueueIterator Begin() noexcept;
    QueueIterator End() noexcept;

    QueueIterator begin() noexcept { return Begin(); }
    QueueIterator end() noexcept { return End(); }
    
    void Clear() noexcept;

private:
    void CleanupResources();
    
    using NodeAllocator = std::pmr::polymorphic_allocator<QueueNode>;
    NodeAllocator node_allocator;
    QueueNode* front_node;
    QueueNode* back_node;
    std::size_t element_count;
};

}

#include "dynamic_queue.ipp"