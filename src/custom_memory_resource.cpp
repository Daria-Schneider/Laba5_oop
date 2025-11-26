#include <algorithm>
#include "custom_memory_resource.h"

namespace memory_strategy {

DynamicListMemoryResource::~DynamicListMemoryResource() {
    for (auto& block : memory_blocks) {
        if (block.is_used) {
            ::operator delete(block.address);
        }
    }
}

void* DynamicListMemoryResource::do_allocate(std::size_t bytes, std::size_t alignment) {
    if (bytes == 0) return nullptr;

    auto it = std::find_if(memory_blocks.begin(), memory_blocks.end(),
        [bytes](const MemoryBlock& block) {
            return !block.is_used && block.size >= bytes;
        });
    
    if (it != memory_blocks.end()) {
        it->is_used = true;
        return it->address;
    }
    
    void* new_block = ::operator new(bytes);
    memory_blocks.emplace_back(new_block, bytes);
    return new_block;
}

void DynamicListMemoryResource::do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) {
    if (!ptr) return;
    
    auto it = std::find_if(memory_blocks.begin(), memory_blocks.end(),
        [ptr](const MemoryBlock& block) {
            return block.address == ptr && block.is_used;
        });
    
    if (it != memory_blocks.end()) {
        it->is_used = false;
    }
}

bool DynamicListMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}

}