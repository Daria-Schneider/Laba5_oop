#pragma once
#include <stdexcept>
#include <string>

namespace container_exceptions {

class QueueOperationException : public std::runtime_error {
public:
    explicit QueueOperationException(const std::string& message)
        : std::runtime_error(message) {}
};

class EmptyQueueException : public QueueOperationException {
public:
    explicit EmptyQueueException(const std::string& message)
        : QueueOperationException(message) {}
};

}