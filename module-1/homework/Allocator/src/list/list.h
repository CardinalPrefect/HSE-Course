#pragma once

#include <memory>
#include <type_traits>
#include <vector>

namespace task {

template <typename T>
class Node {
public:
    Node(Node* prev, Node* next, T value) : value(value), prev(prev), next(next) {
    }

    void push_back(Node* node) {
        next = node;
        if (node)
            node->prev = this;
    }

    void push_front(Node* node) {
        prev = node;
        if (node)
            node->next = this;
    }

    ~Node() {
    }

    T value;
    Node* prev = nullptr;
    Node* next = nullptr;

    // friend class List::Iterator;
};

template <typename T, typename Allocator = std::allocator<T>>
class List {
private:
    class Iterator;

public:
    using value_type = T;
    // Your code goes here
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using iterator = Iterator;
    using const_iterator = const Iterator;

    // Special member functions
    List() {
        Node<T>* x = alloc_.allocate(1);
        alloc_.construct(x, nullptr, nullptr, T());
        head_ = tail_ = x;
    }
    List(const List& other);
    List(const List& other, const Allocator& alloc);
    List(List&& other) noexcept;
    List(List&& other, const Allocator& alloc);

    ~List();

    List& operator=(const List& other);

    List& operator=(List&& other) noexcept;

    // Element access
    reference Front();
    const_reference Front() const;
    reference Back();
    const_reference Back() const;

    // Iterators
    iterator Begin() noexcept;
    const_iterator Begin() const noexcept;

    iterator End() noexcept;
    const_iterator End() const noexcept;

    // Capacity
    bool Empty() const noexcept;

    size_type Size() const noexcept;

    size_type MaxSize() const noexcept;

    // Modifiers
    void Clear();
    void Swap(List& other) noexcept;

    void PushBack(const T& value);
    void PushBack(T&& value);

    template <typename... Args>
    void EmplaceBack(Args&&... args);
    void PopBack();
    void PushFront(const T& value);
    void PushFront(T&& value);
    template <typename... Args>
    void EmplaceFront(Args&&... args);
    void PopFront();

    void Resize(size_type count);

    // Operations
    void Remove(const T& value);
    void Unique();
    void Sort();

    allocator_type GetAllocator() const noexcept;

private:
    typedef typename std::allocator_traits<Allocator>::template rebind_alloc<Node<T>> node_alocator;

    node_alocator alloc_;
    Node<T>* head_;
    Node<T>* tail_;
    std::size_t list_size_ = 0;

private:
    class Iterator {
    public:
        using value_type = List::value_type;
        using pointer = List::pointer;
        using reference = List::reference;
        using difference_type = ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;


        explicit Iterator(Node<T>* node) : node_(node) {
        }

        Iterator& operator++() {
            node_ = node_->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator iter(*this);
            ++(*this);
            return iter;
        }

        Iterator& operator--() {
            node_ = node_->prev;
            return *this;
        }

        Iterator operator--(int) {
            Iterator iter(*this);
            --(*this);
            return iter;
        }

        bool operator==(const iterator& other) {
            return node_ == other.node_;
        }

        bool operator!=(const iterator& other) {
            return node_ != other.node_;
        }

        pointer operator->() const {
            return &node_->value;
        }

        reference operator*() const {
            return node_->value;
        }

    private:
        Node<T>* node_;
    };
};

template <typename T, typename Allocator>
List<T, Allocator>::List(const List<T, Allocator>& other) : alloc_(other.alloc_) {
    for (Node<T>* x = other.head_; x != nullptr; x = x->next)
        PushBack(x->value);
    --list_size_;
}

template <typename T, typename Allocator>
List<T, Allocator>::List(const List<T, Allocator>& other, const Allocator& alloc) {
    alloc_ = alloc;
    for (Node<T>* x = other.head_; x != nullptr; x = x->next)
        PushBack(x->value);
    --list_size_;
}

template <typename T, typename Allocator>
List<T, Allocator>::List(List<T, Allocator>&& other) noexcept {
    *this = std::move(other);
}

template <typename T, typename Allocator>
List<T, Allocator>::List(List<T, Allocator>&& other, const Allocator& alloc) {
    alloc_ = alloc;
    for (Node<T>* x = other.head_; x != nullptr; x = x->next)
        PushBack(std::move(x->value));

    other.Clear();
}

template <typename T, typename Allocator>
List<T, Allocator>::~List() {
    Clear();
    alloc_.destroy(tail_);
    alloc_.deallocate(tail_, 1);
}

template <typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(const List& other) {
    if (this == &other)
        return *this;

    Clear();

    if (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value)
        alloc_ = other.alloc_;

   
    for (Node<T>* x = other.head_; x != other.tail_; x = x->next)
        PushBack(x->value);

    return *this;
}

template <typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(List<T, Allocator>&& other) noexcept {
    if (this == &other)
        return *this;

    Clear();

    if (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {

        other.alloc_.destroy(tail_);
        other.alloc_.deallocate(tail_, 1);
        alloc_ = other.alloc_;

        head_ = other.head_;
        other.head_ = nullptr;

        tail_ = other.tail_;
        other.tail_ = nullptr;

        list_size_ = other.list_size_;

        Node<T>* x = alloc_.allocate(1);
        alloc_.construct(x, nullptr, nullptr, T());
        other.head_ = other.tail_ = x;
        other.list_size_ = 0;
        

    } else {
        for (Node<T>* x = other.head_; x != other.tail_; x = x->next)
            PushBack(std::move(x->value));

        other.Clear();
        other.alloc_.destroy(other.tail_);
        other.alloc_.deallocate(other.tail_, 1);
    }

    return *this;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::reference List<T, Allocator>::Front() {
    return head_->value;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_reference List<T, Allocator>::Front() const {
    return head_->value;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::reference List<T, Allocator>::Back() {
    return tail_->prev->value;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_reference List<T, Allocator>::Back() const {
    return tail_->prev->value;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::Begin() noexcept {
    return iterator(head_);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_iterator List<T, Allocator>::Begin() const noexcept {
    return const_iterator(head_);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::End() noexcept {
    return iterator(tail_);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_iterator List<T, Allocator>::End() const noexcept {
    return const_iterator(tail_);
}

template <typename T, typename Allocator>
bool List<T, Allocator>::Empty() const noexcept {
    return list_size_ == 0;
}

template <typename T, typename Allocator>
size_t List<T, Allocator>::Size() const noexcept {
    return list_size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::Clear() {
    while (list_size_ != 0)
        PopBack();
}

template <typename T, typename Allocator>
void List<T, Allocator>::Swap(List& other) noexcept {
    if (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
        std::swap(alloc_, other.alloc_);
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(list_size_, other.list_size_);
    } else {
        List t = std::move(other);
        other = std::move(*this);
        *this = std::move(t);
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::PushBack(const T& value) {
    Node<T>* x = alloc_.allocate(1);
    if (Empty()) {
        alloc_.construct(x, nullptr, nullptr, value);
        head_ = x;
        head_->next = tail_;
        tail_->prev = head_;
    } else {
        alloc_.construct(x, nullptr, tail_, value);
        tail_->prev->push_back(x);
        tail_->prev = x;
    }
    ++list_size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::PushBack(T&& value) {
    Node<T>* x = alloc_.allocate(1);
    if (Empty()) {
        alloc_.construct(x, nullptr, nullptr, std::move(value));
        head_ = x;
        head_->next = tail_;
        tail_->prev = head_;
    } else {
        alloc_.construct(x, nullptr, tail_, std::move(value));
        tail_->prev->push_back(x);
        tail_->prev = x;
    }
    ++list_size_;
}

template <typename T, typename Allocator>
template <typename... Args>
void List<T, Allocator>::EmplaceBack(Args&&... args) {
    Node<T>* x = alloc_.allocate(1);
    alloc_.construct(x, tail_->prev, tail_, std::forward<Args>(args)...);


    if (head_ == tail_) {
        head_ = x;
    } else {
        x->prev->next = x;
    }
    tail_->prev = x;
    
    ++list_size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::PopBack() {
    if (list_size_ == 1) {
        alloc_.destroy(head_);
        alloc_.deallocate(head_, 1);
        head_ = tail_;
        --list_size_;
    } else if (list_size_ > 1) {
        Node<T>* x = tail_->prev;
        tail_->prev = tail_->prev->prev;
        tail_->prev->next = tail_;

        alloc_.destroy(x);
        alloc_.deallocate(x, 1);
        --list_size_;
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::PushFront(const T& value) {
    Node<T>* x = alloc_.allocate(1);
    if (Empty()) {
        alloc_.construct(x, nullptr, tail_, value);
        head_ = x;
        tail_->prev = head_;
    } else {
        alloc_.construct(x, nullptr, head_, value);
        head_->push_front(x);
        head_ = head_->prev;
    }
    ++list_size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::PushFront(T&& value) {
    Node<T>* x = alloc_.allocate(1);
    if (Empty()) {
        alloc_.construct(x, nullptr, tail_, std::move(value));
        head_ = x;
        tail_->prev = head_;
    } else {
        alloc_.construct(x, nullptr, head_, std::move(value));
        head_->push_front(x);
        head_ = head_->prev;
    }
    ++list_size_;
}

template <typename T, typename Allocator>
template <typename... Args>
void List<T, Allocator>::EmplaceFront(Args&&... args) {
    Node<T>* x = alloc_.allocate(1);
    alloc_.construct(x, nullptr, head_, std::forward<Args>(args)...);

    if (head_ != tail_) {
        head_->prev = x;
    }
    head_ = x;
}

template <typename T, typename Allocator>
void List<T, Allocator>::PopFront() {
    if (list_size_ == 1) {
        alloc_.destroy(head_);
        alloc_.deallocate(head_, 1);
        head_ = tail_;
        --list_size_;
    } else if (list_size_ > 1) {
        Node<T>* x = head_;
        head_ = head_->next;
        head_->prev = nullptr;

        alloc_.destroy(x);
        alloc_.deallocate(x, 1);
        --list_size_;
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::Resize(size_type count) {
    if (count < 0)
        return;
    while (count > list_size_)
        PushBack(T());

    while (count < list_size_)
        PopBack();
}

template <typename T, typename Allocator>
void List<T, Allocator>::Remove(const T& value) {
    if (list_size_ == 0)
        return;

    Node<T>* x = head_;
    Node<T>* delx = nullptr;

    while (x != tail_) {
        if (x->value == value) {
            if (x->prev) {
                x->next->prev = x->prev;
                x->prev->next = x->next;
            } else {
                x->next->prev = nullptr;
            }
            delx = x;

            if (x == head_) {
                head_ = x->next;
            }
        }
        x = x->next;
        if (delx) {
            alloc_.destroy(delx);
            alloc_.deallocate(delx, 1);
            --list_size_;
            delx = nullptr;
        }
    }


}

template <typename T, typename Allocator>
void List<T, Allocator>::Unique() {
    if (list_size_ < 2)
        return;
    T pred = head_->value;
    Node<T>* x = head_->next;
    Node<T>* delx = nullptr;

    while (x != tail_) {
        if (x->value == pred) {
            delx = x;
            x->prev->push_back(x->next);
            x = x->next;

            alloc_.destroy(delx);
            alloc_.deallocate(delx, 1);
            --list_size_;
        } else {
            pred = x->value;
            x = x->next;
        }
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::Sort() {
    std::vector<T> values;

    Node<T>* x = head_;
    while (x != tail_) {
        values.push_back(x->value);
        x = x->next;
    }

    std::sort(values.begin(), values.end());

    Clear();

    for (T& d : values)
        PushBack(d);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::allocator_type List<T, Allocator>::GetAllocator() const noexcept {
    return Allocator(alloc_);
}

}  // namespace task
