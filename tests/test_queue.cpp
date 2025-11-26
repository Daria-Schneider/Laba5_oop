#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "custom_memory_resource.h"
#include "dynamic_queue.h"

struct TestStructure {
    std::string text;
    int number;
    
    bool operator==(const TestStructure& other) const {
        return text == other.text && number == other.number;
    }
};

class QueueTest : public ::testing::Test {
protected:
    void SetUp() override {
        resource = std::make_unique<memory_strategy::DynamicListMemoryResource>();
    }
    
    std::unique_ptr<memory_strategy::DynamicListMemoryResource> resource;
};

TEST_F(QueueTest, InitialState) {
    container::DynamicQueue<int> queue(resource.get());
    EXPECT_TRUE(queue.IsEmpty());
    EXPECT_EQ(queue.GetSize(), 0);
}

TEST_F(QueueTest, BasicEnqueueDequeue) {
    container::DynamicQueue<int> queue(resource.get());
    
    queue.Enqueue(10);
    queue.Enqueue(20);
    queue.Enqueue(30);
    
    EXPECT_EQ(queue.GetSize(), 3);
    EXPECT_EQ(queue.PeekFront(), 10);
    EXPECT_EQ(queue.PeekBack(), 30);
    
    queue.Dequeue();
    EXPECT_EQ(queue.GetSize(), 2);
    EXPECT_EQ(queue.PeekFront(), 20);
    
    queue.Dequeue();
    queue.Dequeue();
    EXPECT_TRUE(queue.IsEmpty());
}

TEST_F(QueueTest, EmptyQueueExceptions) {
    container::DynamicQueue<int> queue(resource.get());
    
    EXPECT_THROW(queue.PeekFront(), container_exceptions::EmptyQueueException);
    EXPECT_THROW(queue.PeekBack(), container_exceptions::EmptyQueueException);
    EXPECT_THROW(queue.Dequeue(), container_exceptions::EmptyQueueException);
}

TEST_F(QueueTest, IteratorFunctionality) {
    container::DynamicQueue<int> queue(resource.get());
    
    queue.Enqueue(1);
    queue.Enqueue(2);
    queue.Enqueue(3);
    
    auto it = queue.Begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    it++;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(it, queue.End());
}

TEST_F(QueueTest, RangeBasedForLoop) {
    container::DynamicQueue<int> queue(resource.get());
    
    for (int i = 1; i <= 5; ++i) {
        queue.Enqueue(i * 10);
    }
    
    std::vector<int> collected;
    for (const auto& item : queue) {
        collected.push_back(item);
    }
    
    ASSERT_EQ(collected.size(), 5);
    EXPECT_EQ(collected[0], 10);
    EXPECT_EQ(collected[4], 50);
}

TEST_F(QueueTest, MoveOperations) {
    container::DynamicQueue<std::string> source(resource.get());
    source.Enqueue("hello");
    source.Enqueue("world");
    
    container::DynamicQueue<std::string> destination = std::move(source);
    
    EXPECT_TRUE(source.IsEmpty());
    EXPECT_EQ(destination.GetSize(), 2);
    EXPECT_EQ(destination.PeekFront(), "hello");
    EXPECT_EQ(destination.PeekBack(), "world");
}

TEST_F(QueueTest, ComplexTypeOperations) {
    container::DynamicQueue<TestStructure> queue(resource.get());
    
    TestStructure s1{"alpha", 1};
    TestStructure s2{"beta", 2};
    
    queue.Enqueue(s1);
    queue.Enqueue(s2);
    
    EXPECT_EQ(queue.GetSize(), 2);
    EXPECT_EQ(queue.PeekFront(), s1);
    EXPECT_EQ(queue.PeekBack(), s2);
    
    queue.Dequeue();
    EXPECT_EQ(queue.PeekFront(), s2);
}