#include <memory>
#include <type_traits>

template <typename T>
class CustomAllocator {
public:
    template <typename U>
    struct rebind {  // NOLINT
        // Your code goes here
        typedef CustomAllocator<U> other;
    };
    // Your code goes here
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    // Influence on container operations
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_swap = std::true_type;
    using is_always_equal = std::false_type;

    CustomAllocator();
    CustomAllocator(const CustomAllocator& other) noexcept;
    ~CustomAllocator();

    template <typename U>
    explicit CustomAllocator(const CustomAllocator<U>& other) noexcept;

    T* allocate(size_t n) {  // NOLINT
        // Your code goes here
        if (*arena_offset_ + n > ARENA_BASIC_SIZE) {
            throw std::runtime_error("Allocator's arena is full");
        }

        *arena_offset_ += n;

        return static_cast<pointer>(arena_) + (*arena_offset_ - n);
    }

    void deallocate(T*, size_t) {  // NOLINT
    }

    template <typename... Args>
    void construct(pointer p, Args&&... args) {  // NOLINT
        // Your code goes here
        ::new (p) value_type(std::forward<Args>(args)...);
    };

    void destroy(pointer p) {  // NOLINT
        // Your code goes here
        p->~T();
    };

    void* get_arena() const {
        return arena_;
    }

    size_t* get_arena_offset() const {
        return arena_offset_;
    }

    size_t* get_allocators_count() const {
        return allocators_count_;
    }

    template <typename K, typename U>
    friend bool operator==(const CustomAllocator<K>& lhs, const CustomAllocator<U>& rhs) noexcept;
    template <typename K, typename U>
    friend bool operator!=(const CustomAllocator<K>& lhs, const CustomAllocator<U>& rhs) noexcept;

private:
    // Your code goes here
    static const size_t ARENA_BASIC_SIZE = 65536;
    void* arena_ = nullptr;
    size_t* arena_offset_ = nullptr;
    size_t* allocators_count_ = nullptr;
};

template <typename T>
CustomAllocator<T>::CustomAllocator() {
    arena_ = ::operator new(ARENA_BASIC_SIZE * sizeof(T));
    arena_offset_ = new size_type(0);
    allocators_count_ = new size_type(1);
}

template <typename T>
CustomAllocator<T>::CustomAllocator(const CustomAllocator& other) noexcept
    : arena_(other.arena_),
      arena_offset_(other.arena_offset_),
      allocators_count_(other.allocators_count_) {
    ++(*allocators_count_);
}

template <typename T>
CustomAllocator<T>::~CustomAllocator() {
    --(*allocators_count_);
    if (*allocators_count_ == 0) {
        ::operator delete(arena_);
        delete arena_offset_;
        delete allocators_count_;
    }
}

template <typename T>
template <typename U>
CustomAllocator<T>::CustomAllocator(const CustomAllocator<U>& other) noexcept
    : arena_(other.get_arena()),
      arena_offset_(other.get_arena_offset()),
      allocators_count_(other.get_allocators_count()) {
    ++(*allocators_count_);
}

template <typename T, typename U>
bool operator==(const CustomAllocator<T>& lhs, const CustomAllocator<U>& rhs) noexcept {
    // Your code goes here
    return lhs.arena_ == rhs.arena_;
}

template <typename T, typename U>
bool operator!=(const CustomAllocator<T>& lhs, const CustomAllocator<U>& rhs) noexcept {
    // Your code goes here
    return lhs.arena_ != rhs.arena_;
}