#include "list.h"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace task;

list::list() { }

list::list(const list& other) {
    Node* x = other.head;
    while (x) {
        push_back(x->value);
        x = x->next;
    }
}

list::list(list&& other) noexcept {
    head = other.head;
    tail = other.tail;
    list_size = other.list_size;
    other.head = other.tail = nullptr;
    other.list_size = 0;
}

list::list(size_t count, const int& value) {
    for (std::size_t i = 0; i < count; ++i)
        push_back(value);
}

list::~list() {
    clear();
}

list& list::operator=(const list& other) {
    if (this == &other)
        return *this;

    clear();
    Node* x = other.head;
    while (x) {
        push_back(x->value);
        x = x->next;
    }
    return *this;
}

list& list::operator=(list&& other) noexcept {
    if (this == &other)
        return *this;

    clear();
    head = other.head;
    tail = other.tail;
    list_size = other.list_size;
    other.head = other.tail = nullptr;
    other.list_size = 0;
    return *this;
}


int& list::front() {
    return head->value;
}

const int& list::front() const {
    return head->value;
}

int& list::back() {
    return tail->value;
}

const int& list::back() const {
    return tail->value;
}

bool list::empty() const {
    return list_size == 0;
}

size_t list::size() const {
    return list_size;
}

void list::clear() {
    Node* x = head;
    Node* nx = nullptr;
    while (x) {
        nx = x->next;
        delete x;
        x = nx;
    }

    head = tail = nullptr;
    list_size = 0;
}

void list::push_back(const int& value) {
    if (list_size == 0)
        head = tail = new Node(value, nullptr, nullptr);
    else {
        tail->push_back(new Node(value, tail, nullptr));
        tail = tail->next;
    }
    ++list_size;
}

void list::pop_back() {
    if (list_size == 1) {
        delete head;
        head = tail = nullptr;
        --list_size;
    } else if (list_size > 1) {
        Node* x = tail;
        tail = tail->prev;
        tail->next = nullptr;
        delete x;
        --list_size;
    }
}

void list::push_front(const int& value) {
    if (list_size == 0)
        head = tail = new Node(value, nullptr, nullptr);
    else {
        head->push_front(new Node(value, nullptr, head));
        head = head->prev;
    }
    ++list_size;
}

void list::pop_front() {
    if (list_size == 1) {
        delete head;
        head = tail = nullptr;
        --list_size;
    } else if (list_size > 1) {
        Node* x = head;
        head = head->next;
        head->prev = nullptr;
        delete x;
        --list_size;
    }
}

void list::resize(size_t count) {
    if (count > list_size) {
        while (count - list_size != 0)
            push_back(0);
    } else {
        while (count - list_size != 0)
            pop_back();
    }
}

void list::swap(list& other) {
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    std::swap(list_size, other.list_size);
}

void list::remove(const int& value) {
    int val = value;
    Node* x = head;

    while(x) {
        if (x->value == val) {
            if (x == head) {
                if (head == tail) {
                    head = tail = nullptr;
                } else {
                    head = head->next;
                    if (head)
                        head->prev = nullptr;
                }
            } else {
                x->prev->push_back(x->next);
                if (x == tail)
                    tail = x->prev;
            }

            auto dx = x;

            x = x->next;

            delete dx;
            --list_size;
        } else {
            x = x->next;
        }
    }
}

void list::unique() {
    if (list_size < 2)
        return;
    int pred = head->value;
    Node* x = head->next;
    Node* delx = nullptr;

    while (x) {
        if (x->value == pred) {
            delx = x;
            x->prev->push_back(x->next);
            x = x->next;
            delete delx;
            --list_size;
        } else {
            pred = x->value;
            x = x->next;
        }
    }
}

void list::sort() {
    std::vector<int> values;

    Node* x = head;
    while (x) {
        values.push_back(x->value);
        x = x->next;
    }

    std::sort(values.begin(), values.end());

    clear();

    for (int& d: values)
        push_back(d);
}
