#pragma once

#include <functional> // std::less
#include <iostream>
#include <queue> // std::queue
#include <utility> // std::pair

template <typename K, typename V, typename Comparator = std::less<K>>
class BinarySearchTree
{
  public:
    using key_type        = K;
    using value_type      = V;
    using key_compare     = Comparator;
    using pair            = std::pair<key_type, value_type>;
    using pointer         = pair*;
    using const_pointer   = const pair*;
    using reference       = pair&;
    using const_reference = const pair&;
    using difference_type = ptrdiff_t;
    using size_type       = size_t;

  private:
    struct BinaryNode
    {
        pair element;
        BinaryNode *left;
        BinaryNode *right;

        BinaryNode( const_reference theElement, BinaryNode *lt, BinaryNode *rt )
          : element{ theElement }, left{ lt }, right{ rt } { }
        
        BinaryNode( pair && theElement, BinaryNode *lt, BinaryNode *rt )
          : element{ std::move( theElement ) }, left{ lt }, right{ rt } { }
    };

    using node           = BinaryNode;
    using node_ptr       = node*;
    using const_node_ptr = const node*;

    node_ptr _root;
    size_type _size;
    key_compare comp;

  public:
    BinarySearchTree(): _root(nullptr), _size(0) {}
    BinarySearchTree( const BinarySearchTree & rhs ): _size(rhs._size) {
        _root = clone(rhs._root);
    }
    BinarySearchTree( BinarySearchTree && rhs ): _size(rhs._size) {
        _root = rhs._root;
        rhs._root = nullptr;
        rhs._size = 0;
    }
    ~BinarySearchTree() {
        clear();
    }

    const_reference min() const { return min( _root )->element; }
    const_reference max() const { return max( _root )->element; }
    const_reference root() const {
        const_reference root = _root->element;
        return root;
    }
    bool contains( const key_type & x ) const { return contains( x, _root ); }
    value_type & find( const key_type & key ) { return find( key, _root )->element.second; }
    const value_type & find( const key_type & key ) const { return find( key, _root )->element.second; }
    bool empty() const {
        return _size == 0;
    }
    size_type size() const {
        return _size;
    }

    void clear() {
        clear( _root );
        _size = 0;
        _root = nullptr;
    }
    void insert( const_reference x ) { insert( x, _root ); }
    void insert( pair && x ) { insert( std::move( x ), _root ); }
    void erase( const key_type & x ) { erase(x, _root); }

    BinarySearchTree & operator=( const BinarySearchTree & rhs ) {
        if(this != &rhs)
        {
            this->~BinarySearchTree();
            _root = clone(rhs._root);
            _size = rhs._size;
        }
        return *this;
    }
    BinarySearchTree & operator=( BinarySearchTree && rhs ) {
        if(this != &rhs)
        {
            this->~BinarySearchTree();
            _root = rhs._root;
            _size = rhs._size;
            rhs._root = nullptr;
            rhs._size = 0;
        }
        return *this;
    }

  private:
    void insert( const_reference x, node_ptr & t ) {
        if(t == nullptr)
        {
            t = new BinaryNode(x, nullptr,nullptr);
            _size++;
        }
        else if(comp(x.first, t->element.first))
        {
            insert(x, t->left);
        }
        else if(comp(t->element.first,x.first))
        {
            insert(x, t->right);
        }
        else
        {
            t->element = x;
        }
    }
    void insert( pair && x, node_ptr & t ) {
        if(t == nullptr)
        {
            t = new BinaryNode(std::move(x),nullptr,nullptr);
            _size++;
        }
        else if(comp(x.first, t->element.first))
        {
            insert(std::move(x), t->left);
        }
        else if(comp(t->element.first,x.first))
        {
            insert(std::move(x), t->right);
            
        }
        else
        {
            t->element = std::move(x);
        }
    }

    void erase( const key_type & x, node_ptr & t ) {
        if(t == nullptr)
        {
            return;
        }
        if(comp(x, t->element.first))
        {
            erase(x, t->left);
        }
        else if(comp(t->element.first, x))
        {
            erase(x, t->right);
        }
        else
        {
            //target node found
            if(t == nullptr)
            {
                return;
            }
            else if(t->left == nullptr && t->right == nullptr)
            {
                //When node has no children
                delete t;
                t = nullptr;
                _size--;
            }
            else if(t->left != nullptr && t->right != nullptr)
            {
                //When node has 2 children
                t->element = min(t->right)->element;
                erase(min(t->right)->element.first, t->right);

            }
            else
            {
                //When node has 1 child
                node_ptr temp = t;
                if(t->left != nullptr)
                {
                    t = t->left;
                }
                else
                {
                    t = t->right;
                }

                delete temp;
                _size--;
            }
            
        }

    }

    const_node_ptr min( const_node_ptr t ) const {
        while (t->left != nullptr)
        {
            t = t->left;
        }
        return t;
        
    }
    const_node_ptr max( const_node_ptr t ) const {
        while (t->right != nullptr)
        {
            t = t->right;
        }
        return t;
    }

    bool contains( const key_type & x, const_node_ptr t ) const {
        if(find(x,t) == nullptr)
        {
            return false;
        }
        return true;
    }
    node_ptr find( const key_type & key, node_ptr t ) {
        if(t == nullptr)
        {
            return nullptr;
        }
        else if(comp(key, t->element.first))
        {
            find(key, t->left);
        }
        else if(comp(t->element.first, key))
        {
            find(key, t->right);
        }
        else
        {
            return t;
        }
    }
    const_node_ptr find( const key_type & key, const_node_ptr t ) const {
        if(t == nullptr)
        {
            return nullptr;
        }
        else if(comp(key, t->element.first))
        {
            find(key, t->left);
        }
        else if(comp(t->element.first, key))
        {
            find(key, t->right);
        }
        else
        {
            return t;
        }
    }

    void clear( node_ptr & t ) {
        if(t == nullptr)
        {
            return;
        }
        if(t->left != nullptr)
        {
            clear(t->left);
        }
        if(t->right != nullptr)
        {
            clear(t->right);
        }
        delete t;
    }
    
    node_ptr clone ( const_node_ptr t ) const {
        if(t == nullptr)
        {
            return nullptr;
        }

        node_ptr sub = new BinaryNode(t->element, nullptr, nullptr);
        if(t->left != nullptr)
        {
            sub->left = clone(t->left);
        }
        if(t->right != nullptr)
        {
            sub->right = clone(t->right);
        }
        return sub;
    }

  public:
    template <typename KK, typename VV, typename CC>
    friend void printLevelByLevel( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out );

    template <typename KK, typename VV, typename CC>
    friend std::ostream& printNode(std::ostream& o, const typename BinarySearchTree<KK, VV, CC>::node& bn);

    template <typename KK, typename VV, typename CC>
    friend void printTree( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out );

    template <typename KK, typename VV, typename CC>
    friend void printTree(typename BinarySearchTree<KK, VV, CC>::const_node_ptr t, std::ostream & out, unsigned depth );

    template <typename KK, typename VV, typename CC>
    friend void vizTree(
        typename BinarySearchTree<KK, VV, CC>::const_node_ptr node, 
        std::ostream & out,
        typename BinarySearchTree<KK, VV, CC>::const_node_ptr prev
    );

    template <typename KK, typename VV, typename CC>
    friend void vizTree(
        const BinarySearchTree<KK, VV, CC> & bst, 
        std::ostream & out
    );
};

template <typename KK, typename VV, typename CC>
std::ostream& printNode(std::ostream & o, const typename BinarySearchTree<KK, VV, CC>::node & bn) {
    return o << '(' << bn.element.first << ", " << bn.element.second << ')';
}

template <typename KK, typename VV, typename CC>
void printLevelByLevel( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out = std::cout ) {
    using node = typename BinarySearchTree<KK, VV, CC>::node;
    using node_ptr = typename BinarySearchTree<KK, VV, CC>::node_ptr;
    using const_node_ptr = typename BinarySearchTree<KK, VV, CC>::const_node_ptr;
    
    // TODO -- Guide in Instructions
    std::queue <node_ptr> nodes;
    node_ptr curr;
    
    int elementsInLevel = 1;
    bool nonNullChild = false;

    if(bst._root != nullptr)
    {
        nodes.push(bst._root);
        while(elementsInLevel > 0)
        {
            curr = nodes.front();
            nodes.pop();
            elementsInLevel--;

            if(curr != nullptr)
            {
                printNode<KK, VV, CC>(out, *curr);
                nodes.push(curr->left);
                nodes.push(curr->right);
                if(curr->left != nullptr || curr->right != nullptr)
                {
                    nonNullChild = true;
                }
            }
            else
            {
                out << "null";
                nodes.push(nullptr);
                nodes.push(nullptr);
            }

            if(elementsInLevel == 0)
            {
                out << "\n";
                if(nonNullChild)
                {
                    nonNullChild = false;
                    elementsInLevel = nodes.size();
                }
            }

        }
    }
    
}

template <typename KK, typename VV, typename CC>
void printTree( const BinarySearchTree<KK, VV, CC> & bst, std::ostream & out = std::cout ) { printTree<KK, VV, CC>(bst._root, out ); }

template <typename KK, typename VV, typename CC>
void printTree(typename BinarySearchTree<KK, VV, CC>::const_node_ptr t, std::ostream & out, unsigned depth = 0 ) {
    if (t != nullptr) {
        printTree<KK, VV, CC>(t->right, out, depth + 1);
        for (unsigned i = 0; i < depth; ++i)
            out << '\t';
        printNode<KK, VV, CC>(out, *t) << '\n';
        printTree<KK, VV, CC>(t->left, out, depth + 1);
    }
}

template <typename KK, typename VV, typename CC>
void vizTree(
    typename BinarySearchTree<KK, VV, CC>::const_node_ptr node, 
    std::ostream & out,
    typename BinarySearchTree<KK, VV, CC>::const_node_ptr prev = nullptr
) {
    if(node) {
        std::hash<KK> khash{};

        out << "\t" "node_" << (uint32_t) khash(node->element.first)
            << "[label=\"" << node->element.first 
            << " [" << node->element.second << "]\"];" << std::endl;
        
        if(prev)
            out << "\tnode_" << (uint32_t) khash(prev->element.first) <<  " -> ";
        else
            out << "\t";
        
        out << "node_" << (uint32_t) khash(node->element.first) << ";" << std::endl;
    
        vizTree<KK, VV, CC>(node->left, out, node);
        vizTree<KK, VV, CC>(node->right, out, node);
    }
}

template <typename KK, typename VV, typename CC>
void vizTree(
    const BinarySearchTree<KK, VV, CC> & bst, 
    std::ostream & out = std::cout
) {
    out << "digraph Tree {" << std::endl;
    vizTree<KK, VV, CC>(bst._root, out);
    out << "}" << std::endl;
}
