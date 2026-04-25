#include <cstddef>
#include <utility>
#include <stdexcept>
#include <cstring>

namespace sjtu {

template<typename T>
class vector {
private:
    T* data_;
    size_t size_;
    size_t capacity_;
    
public:
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;
    
    vector() noexcept : data_(nullptr), size_(0), capacity_(0) {}
    
    explicit vector(size_t n) : data_(nullptr), size_(0), capacity_(0) {
        if (n > 0) {
            data_ = static_cast<T*>(::operator new(n * sizeof(T)));
            capacity_ = n;
            for (size_t i = 0; i < n; ++i) {
                ::new (&data_[i]) T();
            }
            size_ = n;
        }
    }
    
    vector(size_t n, const T& val) : data_(nullptr), size_(0), capacity_(0) {
        if (n > 0) {
            data_ = static_cast<T*>(::operator new(n * sizeof(T)));
            capacity_ = n;
            for (size_t i = 0; i < n; ++i) {
                ::new (&data_[i]) T(val);
            }
            size_ = n;
        }
    }
    
    vector(const vector& other) : data_(nullptr), size_(0), capacity_(0) {
        if (other.size_ > 0) {
            data_ = static_cast<T*>(::operator new(other.capacity_ * sizeof(T)));
            capacity_ = other.capacity_;
            for (size_t i = 0; i < other.size_; ++i) {
                ::new (&data_[i]) T(other.data_[i]);
            }
            size_ = other.size_;
        }
    }
    
    vector(vector&& other) noexcept 
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    
    ~vector() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        ::operator delete(data_);
    }
    
    vector& operator=(const vector& other) {
        if (this != &other) {
            for (size_t i = 0; i < size_; ++i) {
                data_[i].~T();
            }
            
            if (other.capacity_ > capacity_) {
                ::operator delete(data_);
                data_ = static_cast<T*>(::operator new(other.capacity_ * sizeof(T)));
                capacity_ = other.capacity_;
            }
            
            for (size_t i = 0; i < other.size_; ++i) {
                ::new (&data_[i]) T(other.data_[i]);
            }
            size_ = other.size_;
        }
        return *this;
    }
    
    vector& operator=(vector&& other) noexcept {
        if (this != &other) {
            for (size_t i = 0; i < size_; ++i) {
                data_[i].~T();
            }
            ::operator delete(data_);
            
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }
    
    reference operator[](size_t pos) { return data_[pos]; }
    const_reference operator[](size_t pos) const { return data_[pos]; }
    
    reference at(size_t pos) {
        if (pos >= size_) throw std::out_of_range("vector::at");
        return data_[pos];
    }
    
    const_reference at(size_t pos) const {
        if (pos >= size_) throw std::out_of_range("vector::at");
        return data_[pos];
    }
    
    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }
    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }
    
    T* data() noexcept { return data_; }
    const T* data() const noexcept { return data_; }
    
    iterator begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }
    const_iterator cbegin() const noexcept { return data_; }
    
    iterator end() noexcept { return data_ + size_; }
    const_iterator end() const noexcept { return data_ + size_; }
    const_iterator cend() const noexcept { return data_ + size_; }
    
    bool empty() const noexcept { return size_ == 0; }
    size_t size() const noexcept { return size_; }
    size_t capacity() const noexcept { return capacity_; }
    
    void reserve(size_t new_cap) {
        if (new_cap > capacity_) {
            T* new_data = static_cast<T*>(::operator new(new_cap * sizeof(T)));
            for (size_t i = 0; i < size_; ++i) {
                ::new (&new_data[i]) T(std::move(data_[i]));
                data_[i].~T();
            }
            ::operator delete(data_);
            data_ = new_data;
            capacity_ = new_cap;
        }
    }
    
    void clear() noexcept {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }
    
    void push_back(const T& value) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        ::new (&data_[size_++]) T(value);
    }
    
    void push_back(T&& value) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        ::new (&data_[size_++]) T(std::move(value));
    }
    
    template<typename... Args>
    void emplace_back(Args&&... args) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        ::new (&data_[size_++]) T(std::forward<Args>(args)...);
    }
    
    void pop_back() {
        if (size_ > 0) {
            data_[--size_].~T();
        }
    }
    
    void resize(size_t new_size) {
        if (new_size > capacity_) {
            reserve(new_size);
        }
        if (new_size > size_) {
            for (size_t i = size_; i < new_size; ++i) {
                ::new (&data_[i]) T();
            }
        } else {
            for (size_t i = new_size; i < size_; ++i) {
                data_[i].~T();
            }
        }
        size_ = new_size;
    }
    
    void resize(size_t new_size, const T& value) {
        if (new_size > capacity_) {
            reserve(new_size);
        }
        if (new_size > size_) {
            for (size_t i = size_; i < new_size; ++i) {
                ::new (&data_[i]) T(value);
            }
        } else {
            for (size_t i = new_size; i < size_; ++i) {
                data_[i].~T();
            }
        }
        size_ = new_size;
    }
    
    iterator insert(iterator pos, const T& value) {
        size_t index = pos - data_;
        if (size_ == capacity_) {
            size_t new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
            T* new_data = static_cast<T*>(::operator new(new_cap * sizeof(T)));
            
            for (size_t i = 0; i < index; ++i) {
                ::new (&new_data[i]) T(std::move(data_[i]));
                data_[i].~T();
            }
            ::new (&new_data[index]) T(value);
            for (size_t i = index; i < size_; ++i) {
                ::new (&new_data[i + 1]) T(std::move(data_[i]));
                data_[i].~T();
            }
            
            ::operator delete(data_);
            data_ = new_data;
            capacity_ = new_cap;
        } else {
            ::new (&data_[size_]) T(std::move(data_[size_ - 1]));
            for (size_t i = size_ - 1; i > index; --i) {
                data_[i] = std::move(data_[i - 1]);
            }
            data_[index] = value;
        }
        ++size_;
        return data_ + index;
    }
    
    iterator erase(iterator pos) {
        size_t index = pos - data_;
        data_[index].~T();
        for (size_t i = index; i < size_ - 1; ++i) {
            ::new (&data_[i]) T(std::move(data_[i + 1]));
            data_[i + 1].~T();
        }
        --size_;
        return data_ + index;
    }
    
    iterator erase(iterator first, iterator last) {
        size_t start = first - data_;
        size_t end = last - data_;
        size_t count = end - start;
        
        for (size_t i = start; i < end; ++i) {
            data_[i].~T();
        }
        for (size_t i = end; i < size_; ++i) {
            ::new (&data_[i - count]) T(std::move(data_[i]));
            data_[i].~T();
        }
        size_ -= count;
        return data_ + start;
    }
    
    void swap(vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
};

} // namespace sjtu
