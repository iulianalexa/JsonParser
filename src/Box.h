#ifndef BOX_H
#define BOX_H
#include <memory>

// Similar to Rust's Box. Used to prevent a circular dependency.

template <typename T>
class Box {
public:
    // Boxing constructor
    Box(T value) {
        ptr = std::make_unique<T>(value);
    }

    // Move constructor
    Box(Box &&other) noexcept {
        ptr = std::move(other.ptr);
    }

    // Copy constructor
    Box(const Box &other) {
        ptr = other.ptr ? std::make_unique<T>(*other.ptr) : nullptr;
    }

    // Move assignment operator
    Box &operator=(Box &&other) noexcept {
        if (this != &other) {
            ptr = std::move(other.ptr);
        }

        return *this;
    }

    // Copy assignment operator
    Box &operator=(const Box &other) {
        if (this != &other) {
            ptr = other.ptr ? std::make_unique<T>(*other.ptr) : nullptr;
        }

        return *this;
    }

    // Implicit conversion operator to T for unboxing
    operator T() const {
        return *ptr;
    }

private:
    std::unique_ptr<T> ptr;
};

#endif //BOX_H
