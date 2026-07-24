#pragma once 
#include <memory>  
#include <stdexcept>

template<typename T, typename Allocator = std::allocator<T>>
class deque {
public:
    class iterator {
        T** blocks_;
        size_t index_; 
        size_t block_size_;
    
    public:
        iterator(T** blocks, size_t index, size_t block_size) 
            : blocks_(blocks), index_(index), block_size_(block_size) {}
    
        T& operator*() { 
            return blocks_[index_ / block_size_][index_ % block_size_]; 
        }
        iterator& operator++() { ++index_; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++index_; return tmp; }
        bool operator==(const iterator& other) const { 
            return index_ == other.index_; 
        }
        bool operator!=(const iterator& other) const { 
            return !(*this == other); 
        }
    };

private:
    static constexpr size_t BLOCK_SIZE = 16;

    size_t capacity_;       
    size_t size_;          
    T** blocks_;           
    size_t block_count_;   
    size_t first_block_;  
    size_t first_element_;
    Allocator allocator_;  
    using BlockAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<T*>;


    BlockAllocator block_allocator_; 

    void resize() {
        size_t new_block_count = block_count_ * 2;
        T** new_blocks = nullptr;
        
        try {
            new_blocks = block_allocator_.allocate(new_block_count);
            size_t new_first_block = block_count_ / 2;  
            
            for (size_t i = 0; i < new_block_count; ++i) {
                if (i >= new_first_block && i < new_first_block + block_count_) {
                    size_t old_idx = (first_block_ + i - new_first_block) % block_count_;
                    new_blocks[i] = blocks_[old_idx];
                } else {
                    new_blocks[i] = allocator_.allocate(BLOCK_SIZE);

                    for (size_t j = 0; j < BLOCK_SIZE; ++j) {
                        std::allocator_traits<Allocator>::construct(
                            allocator_, 
                            &new_blocks[i][j]
                        );
                    }
                }
            }
            
       
            first_block_ = new_first_block;
            block_allocator_.deallocate(blocks_, block_count_);
            blocks_ = new_blocks;
            block_count_ = new_block_count;
            capacity_ = block_count_ * BLOCK_SIZE;
        } catch (...) {
         
            if (new_blocks) {
                for (size_t i = 0; i < new_block_count; ++i) {
                    if (new_blocks[i]) allocator_.deallocate(new_blocks[i], BLOCK_SIZE);
                }
                block_allocator_.deallocate(new_blocks, new_block_count);
            }
            throw;
        }
    }

public:
deque() : capacity_(BLOCK_SIZE), size_(0), block_count_(1), 
          first_block_(0), first_element_(0) {
    blocks_ = block_allocator_.allocate(1);
    blocks_[0] = allocator_.allocate(BLOCK_SIZE);
    
 
    for (size_t i = 0; i < BLOCK_SIZE; ++i) {
        std::allocator_traits<Allocator>::construct(
            allocator_, 
            &blocks_[0][i]
        );
    }
}

~deque() {
    clear();  
    
    for (size_t i = 0; i < block_count_; ++i) {
        if (blocks_[i]) {
            allocator_.deallocate(blocks_[i], BLOCK_SIZE);
        }
    }
    block_allocator_.deallocate(blocks_, block_count_);
}
    void push_back(const T& value) {
        if (size_ >= capacity_) {
            resize();
        }
        
        
        size_t absolute_pos = first_block_ * BLOCK_SIZE + first_element_ + size_; 
        size_t block_idx = (absolute_pos / BLOCK_SIZE) % block_count_;
        size_t element_idx = absolute_pos % BLOCK_SIZE;
        
        std::allocator_traits<Allocator>::construct(
            allocator_, 
            &blocks_[block_idx][element_idx], 
            value
        );
        ++size_;
    }

    void push_front(const T& value) {
        if (size_ >= capacity_) {
            resize();
        }
        

        
        if (first_element_ == 0) {
            first_element_ = BLOCK_SIZE - 1;
            first_block_ = (first_block_ - 1 + block_count_) % block_count_;  
        } else {
            --first_element_;
        }
        
        std::allocator_traits<Allocator>::construct(
            allocator_,
            &blocks_[first_block_][first_element_],
            value
        );
        ++size_;
    }

    void pop_back() {
        if (empty()) throw std::runtime_error("Deque is empty");
        
        
    size_t last_pos = first_block_ * BLOCK_SIZE + first_element_ + size_ - 1;
    size_t block_idx = (last_pos / BLOCK_SIZE) % block_count_;
    size_t elem_idx = last_pos % BLOCK_SIZE;
    

    std::allocator_traits<Allocator>::destroy(
        allocator_, &blocks_[block_idx][elem_idx]);
        --size_;
    }


    void pop_front() {
        if (empty()) {
            throw std::runtime_error("Deque is empty");
        }

        std::allocator_traits<Allocator>::destroy(
            allocator_, 
            &blocks_[first_block_][first_element_]
        );

        first_element_ = (first_element_ + 1) % BLOCK_SIZE;
        if (first_element_ == 0) {
            first_block_ = (first_block_ + 1) % block_count_;
        }
        --size_;

    }
    

    void clear() {
 
        for (size_t i = 0; i < size_; ++i) {
            size_t pos = first_block_ * BLOCK_SIZE + first_element_ + i;
            size_t block_idx = (pos / BLOCK_SIZE) % block_count_;
            size_t elem_idx = pos % BLOCK_SIZE;
            
            std::allocator_traits<Allocator>::destroy(
                allocator_, &blocks_[block_idx][elem_idx]);
        }
        

        size_ = 0;
        first_block_ = 0;
        first_element_ = 0;
    }

    size_t size() const { return size_; }

    bool empty() const { return size_ == 0; }


    iterator begin() { 
        return iterator(blocks_, first_block_ * BLOCK_SIZE + first_element_, BLOCK_SIZE);
    }

    iterator end() { 
        return iterator(blocks_, first_block_ * BLOCK_SIZE + first_element_ + size_, BLOCK_SIZE);
    }

    T& operator[](size_t index) {
        size_t pos = first_block_ * BLOCK_SIZE + first_element_ + index;
        size_t block_idx = (pos / BLOCK_SIZE) % block_count_;
        size_t elem_idx = pos % BLOCK_SIZE;
        return blocks_[block_idx][elem_idx];
    }

 
    const T& operator[](size_t index) const {
        size_t pos = first_block_ * BLOCK_SIZE + first_element_ + index;
        size_t block_idx = (pos / BLOCK_SIZE) % block_count_;
        size_t elem_idx = pos % BLOCK_SIZE;
        return blocks_[block_idx][elem_idx];
    }
};