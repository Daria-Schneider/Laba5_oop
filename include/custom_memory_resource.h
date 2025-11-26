#pragma once
#include <memory_resource>
#include <list>

namespace memory_strategy {

class DynamicListMemoryResource : public std::pmr::memory_resource {
public:
    DynamicListMemoryResource() = default;
    ~DynamicListMemoryResource() override;
    
    DynamicListMemoryResource(const DynamicListMemoryResource&) = delete;
    DynamicListMemoryResource& operator=(const DynamicListMemoryResource&) = delete;

protected:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override;
    void do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

private:
    struct MemoryBlock {
        void* address;
        std::size_t size;
        bool is_used;
        
        MemoryBlock(void* addr, std::size_t sz) 
            : address(addr), size(sz), is_used(true) {}
    };
    
    std::list<MemoryBlock> memory_blocks;
};

}