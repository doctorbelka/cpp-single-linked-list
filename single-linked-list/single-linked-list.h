#pragma once
#include <cstddef>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
    
    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node) {
        node_=node;
        }
        Node* node_ = nullptr;
        public:
         using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;


        BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_) {}

    
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return (rhs.node_==this->node_);
        }


        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(rhs==*this);
        }


        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return (rhs.node_==this->node_);
        }


        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
           return !(rhs==*this);
        }

        BasicIterator& operator++() noexcept {
           assert(node_!=nullptr);
            node_=node_->next_node;
            return *this;
        }


        BasicIterator operator++(int) noexcept {
            auto old=(*this);
            ++(*this);
            return old;
        }


        [[nodiscard]] reference operator*() const noexcept {
        assert(node_!=nullptr);
           return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
        assert(node_!=nullptr);
            return &node_->value;
        }

    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;
    
    SingleLinkedList(std::initializer_list<Type> values) {
        for (auto it=std::rbegin(values);it!=std::rend(values);++it){
        PushFront(*it);
        }  
        size_=values.size();
    }

    SingleLinkedList(const SingleLinkedList& other) {
   
        SingleLinkedList tmp;           //Не совсем понял как исправить, нужно через InsertAfter делать? И как найти элемент перед концом?
        for (auto value : other)    {
        tmp.PushFront(value);
        }
        SingleLinkedList tmp2;
        for (auto value : tmp)    {
        tmp2.PushFront(value);
        }
        swap(tmp2);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this==&rhs)
        return *this;
        SingleLinkedList tmp(rhs);
        swap(tmp);
        return *this;
    }


    void swap(SingleLinkedList& other) noexcept {
       std::swap(size_,other.size_);
       std::swap(head_.next_node,other.head_.next_node);
    }

    [[nodiscard]] Iterator begin() noexcept {

        return Iterator{head_.next_node};
    }

     [[nodiscard]] Iterator end() noexcept {
        
        return Iterator{nullptr};
    }


    [[nodiscard]] ConstIterator begin() const noexcept {
      
        return Iterator{head_.next_node};
    }
    

    [[nodiscard]] ConstIterator end() const noexcept {
        return Iterator{nullptr};
    }
    

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{head_.next_node};
    }
    

    [[nodiscard]] ConstIterator cend() const noexcept {
         return ConstIterator{nullptr};
    }
    
    [[nodiscard]] Iterator before_begin() noexcept{
    return Iterator{&head_};
    }
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept{
    return ConstIterator{const_cast<Node*>(&head_)};
    }
    [[nodiscard]] ConstIterator before_begin() const noexcept{
    return ConstIterator{const_cast<Node*>(&head_)};
    }
    
SingleLinkedList() : head_(), size_(0) {}

~SingleLinkedList(){
Clear();
}

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }


    [[nodiscard]] bool IsEmpty() const noexcept {
        return (!head_.next_node);
    }
    
    void PushFront(const Type& value){
    head_.next_node=new Node(value,head_.next_node);
    ++size_;
    }
    
    void Clear() noexcept{
    while (head_.next_node){
    PopFront();
    }
    size_=0;
    
    }
    
    void PopFront() noexcept {
    assert(size_>0);
    --size_;
    delete std::exchange(head_.next_node,head_.next_node->next_node);
    
    }
    
    
    
    ConstIterator InsertAfter(ConstIterator pos,const Type& value) {
    assert(pos.node_);
    auto new_node=new Node(value, pos.node_->next_node);
    pos.node_->next_node=new_node;
    ++size_;
    return ConstIterator{new_node};
    }
    
    Iterator EraseAfter (ConstIterator pos) {
    assert(pos.node_->next_node);
    delete std::exchange(pos.node_->next_node,pos.node_->next_node->next_node);
    --size_;
    return Iterator{pos.node_->next_node};
    }

private:
    Node head_;
    size_t size_=0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (lhs.GetSize()!=rhs.GetSize()) return false;
    return std::equal(lhs.begin(),lhs.end(),rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs==rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs>rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (!(lhs<rhs)&&(lhs!=rhs));
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs<rhs);
} 
