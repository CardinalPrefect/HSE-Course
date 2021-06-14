#pragma once

#include <atomic>

class SharedCount {
public:
    size_t Get() const {
        return counter_.load();
    }

    virtual bool Empty() {
        return counter_ > 0;
    }

    virtual bool Remove() {
        if (counter_ == 0) {
            return false;
        }

        --counter_;
        return true;
    }

    void Add() {
        ++counter_;
    }

protected:
    std::atomic<size_t> counter_{0};
};

class SharedWeakCount : public SharedCount {
public:
    void AddWeak() {
        ++weak_counter_;
    }

    bool RemoveWeak() {
        if (weak_counter_ == 0) {
            return false;
        }

        --weak_counter_;
        return true;
    }

    size_t GetWeak() const {
        return weak_counter_.load();
    }

    bool Empty() override {
        return (counter_ == 0) and (weak_counter_ == 0);
    }

protected:
    std::atomic<size_t> weak_counter_{0};
};

template <typename T, typename Deleter>
class ControlBlock : public SharedWeakCount {
public:
    explicit ControlBlock(T* value) : value_{value} {
        assert(value_ != nullptr);
        Add();
    }

    ControlBlock(ControlBlock&) = delete;
    void operator=(ControlBlock&) = delete;

    bool Remove() override {
        if (SharedCount::Remove()) {
            if (Get() == 0) {
                Deleter deleter;
                deleter(value_);
                value_ = nullptr;
            }
            return true;
        }
        return false;
    }

private:
    T* value_;
};