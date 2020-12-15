#pragma once
#include <cstddef>


namespace task {
class Node {
public:
    Node(int value, Node* prev, Node* next):
        value(value), prev(prev), next(next) { }

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

    ~Node() { }
private:
    int value = 0;
    Node* prev = nullptr;
    Node* next = nullptr;

    friend class list;
};

class list {


public:

    list();
    list(const list& other);
    list(list&& other) noexcept;
    list(size_t count, const int& value = int());

    ~list();
    list& operator=(const list& other);
    list& operator=(list&& other) noexcept;


    int& front();
    const int& front() const;

    int& back();
    const int& back() const;


    bool empty() const;
    size_t size() const;
    void clear();


    void push_back(const int& value);
    void pop_back();
    void push_front(const int& value);
    void pop_front();
    void resize(size_t count);
    void swap(list& other);


    void remove(const int& value);
    void unique();
    void sort();

private:
    Node* head = nullptr;
    Node* tail = nullptr;
    std::size_t list_size = 0;

};

}  // namespace task
