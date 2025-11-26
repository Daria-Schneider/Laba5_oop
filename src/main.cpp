#include <iostream>
#include <string>
#include "custom_memory_resource.h"
#include "dynamic_queue.h"

struct ComplexData {
    std::string name;
    int identifier;
    double value;

    ComplexData() = default;
    ComplexData(const std::string& n, int id, double val) 
        : name(n), identifier(id), value(val) {}
    
    friend std::ostream& operator<<(std::ostream& os, const ComplexData& data) {
        os << "ComplexData{name:\"" << data.name << "\", id:" << data.identifier 
           << ", value:" << data.value << "}";
        return os;
    }
};

int main() {
    try {
        memory_strategy::DynamicListMemoryResource custom_resource;
        
        std::cout << "--- Testing with integers ---" << std::endl;
        container::DynamicQueue<int> int_queue(&custom_resource);
        
        int_queue.Enqueue(100);
        int_queue.Enqueue(200);
        int_queue.Enqueue(300);
        
        std::cout << "Queue size: " << int_queue.GetSize() << std::endl;
        std::cout << "Queue elements: ";
        for (const auto& item : int_queue) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
        
        int_queue.Dequeue();
        std::cout << "After dequeue - Front: " << int_queue.PeekFront() 
                  << ", Back: " << int_queue.PeekBack() << std::endl;
        
        std::cout << "\n--- Testing with ComplexData ---" << std::endl;
        container::DynamicQueue<ComplexData> complex_queue(&custom_resource);
        
        complex_queue.Enqueue("First", 1, 1.1);
        complex_queue.Enqueue("Second", 2, 2.2);
        complex_queue.Enqueue("Third", 3, 3.3);
        
        std::cout << "Complex queue elements:" << std::endl;
        for (const auto& item : complex_queue) {
            std::cout << "  " << item << std::endl;
        }
        
        std::cout << "Front: " << complex_queue.PeekFront() << std::endl;
        std::cout << "Back: " << complex_queue.PeekBack() << std::endl;
        
        std::cout << "\n--- Testing move semantics ---" << std::endl;
        container::DynamicQueue<int> source_queue(&custom_resource);
        source_queue.Enqueue(777);
        source_queue.Enqueue(888);
        
        container::DynamicQueue<int> moved_queue = std::move(source_queue);
        std::cout << "Moved queue size: " << moved_queue.GetSize() << std::endl;
        std::cout << "Source queue is empty: " << std::boolalpha << source_queue.IsEmpty() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}