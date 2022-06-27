//
// Created by tkondrac on 6/5/22.
//

#ifndef FT_CONTAINERS_B_TREE_HPP
#define FT_CONTAINERS_B_TREE_HPP

#include <memory>
#include <cstdlib>
#include <stdexcept>
#include "iterator.hpp"

namespace ft {

	///simple binary tree, no fancy red black shenanigans
	template<typename T, class Allocator = std::allocator<T>, class Compare = std::less<T> >
	class BTree {
	public:
		//typedefs
		typedef bidirectional_iterator<BTree<T> >				iterator;
		typedef const bidirectional_iterator<BTree<T> >			const_iterator;
		typedef size_t											size_type;
		typedef long											difference_type;
		typedef typename Allocator::pointer						pointer;
		typedef typename Allocator::const_pointer				const_pointer;

	private:
		///node class
		class BTreeNode {
			friend class BTree;
			friend class bidirectional_iterator<BTree<T> >;
		private:
			//priv typedefs
			typedef typename Allocator::template rebind_alloc<BTreeNode>	Node_Alloc;
			typedef typename Node_Alloc::pointer	node_ptr;

			Node_Alloc		node_alloc;
			Allocator		alloc;
			BTree::pointer	elem;
			node_ptr	 	left;
			node_ptr	 	parent;
			node_ptr	 	right;
		public:
			BTreeNode(const Allocator& alloc, const Node_Alloc & nalloc): parent(NULL), left(NULL), right(NULL), alloc(alloc), node_alloc(nalloc) {  }
			BTreeNode(const T & new_elem, const Allocator& alloc, const Node_Alloc & nalloc): parent(NULL), left(NULL), right(NULL), alloc(alloc), node_alloc(nalloc) {
				elem = alloc.allocate(1);
				alloc.construct(elem, new_elem);
			}
			BTreeNode(const BTreeNode & node): parent(NULL), alloc(node.alloc), node_alloc(node.node_alloc) {
				elem = alloc.allocate(1);
				alloc.construct(elem, node.elem);
				if (node.right != NULL){
					right = node_alloc.allocate(1);
					node_alloc.construct(right, node.right);
					right->parent = this;
				}
				if (node.left != NULL){
					left = node_alloc.allocate(1);
					node_alloc.construct(left, node.left);
					left->parent = this;
				}
			}
			~BTreeNode() {
				alloc.destroy(elem);
				alloc.deallocate(elem, 1);
				if (right != NULL){
					node_alloc.destroy(right);
					node_alloc.deallocate(right, 1);
				}
				if (left != NULL){
					node_alloc.destroy(left);
					node_alloc.deallocate(left, 1);
				}
			}
		};
	private:
		//priv typedefs
		typedef typename Allocator::template rebind_alloc<BTreeNode>	Node_Alloc;
		typedef typename Node_Alloc::pointer	node_ptr;
	public:
		///Compare class
		typedef Compare elem_compare;
		class value_compare : std::binary_function< T, T, bool >
		{
			friend class BTree;

		protected:
			Compare comp;
			explicit value_compare(Compare c) : comp(c) {}

		public:
			typedef bool result_type;
			typedef T first_argument_type;
			typedef T second_argument_type;
			bool operator()(const T &x, const T &y) const
			{
				return comp(x, y);
			}
		};

	private:
		//Btree
		Node_Alloc							node_alloc;
		Allocator							alloc;
		node_ptr							head;
		size_t								_size;
		value_compare						_comparator;

	public:
		explicit BTree(const elem_compare &comp = elem_compare(), const Allocator& alloc = Allocator()): alloc(alloc), head(NULL), _size(0), _comparator(comp) {  }
		BTree(const BTree & inst): node_alloc(inst.node_alloc), alloc(inst.alloc), _size(inst._size), _comparator(inst._comparator) {
			if (inst.head == NULL){
				head = NULL;
				return;
			}
			head = node_alloc.allocate(1);
			node_alloc.construct(head, *(inst.head));
		}
		~BTree() {
			if (head != NULL) {
				node_alloc.destroy(head);
				node_alloc.deallocate(head, 1);
			}
		}

		BTree & operator=(const BTree & other) {
			node_alloc = other.node_alloc;
			alloc = other.alloc;
			_size = other._size;
			_comparator = other._comparator;
			if (other.head == NULL){
				head = NULL;
				return *this;
			}
			head = node_alloc.allocate(1);
			node_alloc.construct(head, *(other.head));
			return *this;
		}

		size_type size() const{
			return _size;
		}

		Allocator get_alloc() const{
			return alloc;
		}

		//needed funcs todo add remove find begin last end
		iterator	begin() {
			node_ptr current = head;
			while (current->left != NULL)
				current = current->left;
			return iterator(current);
		}

		iterator	last() {
			node_ptr current = head;
			while (current->right != NULL)
				current = current->right;
			return iterator(current);
		}

		iterator	end() {
			node_ptr current = head;
			while (current->right != NULL)
				current = current->right;
			return (iterator(current)++);
		}

		iterator	find(const T & e){
			node_ptr current = head;
			while (current != NULL) {
				if (_comparator(*(current->elem), e))
				{
					current = current->right;
				}
				else if (_comparator(e, *(current->elem)))
				{
					current = current->left;
				}
				else {
					return iterator(current);
				}
			}
			return this->end();
		}

		iterator	find_not_less(const T & e){
			node_ptr current = head;
			while (current != NULL) {
				if (_comparator(*(current->elem), e))
				{
					if (current->right == NULL){
						return ++iterator(current);
					}
					current = current->right;
				}
				else if (_comparator(e, *(current->elem)))
				{
					if (current->left == NULL){
						return iterator(current);
					}
					current = current->left;
				}
				else {
					return iterator(current);
				}
			}
			return this->end();
		}

		///attention trouve possible le precedent ou pas
		node_ptr	find_prev_or(const T & e){
			node_ptr prev = head;
			node_ptr current = head;
			while (current != NULL) {
				prev = current;
				if (_comparator(*(current->elem), e))
				{
					current = current->right;
				}
				else if (_comparator(e, *(current->elem)))
				{
					current = current->left;
				}
				else {
					return current;
				}
			}
			return prev;
		}

		node_ptr create_new(const T & e) {
			node_ptr n = node_alloc.allocate(1);
			node_alloc.construct(n, BTreeNode(e, alloc, node_alloc));
		}

		void set_parent_reloc(node_ptr p, node_ptr n){
			if (p->parent == NULL)
				head = n;
			else if (p->parent->left == p)
				p->parent->left = n;
			else
				p->parent->right = n;
		}

		void	remove_node(node_ptr p) {
			node_ptr freespot;

			if (p->left != NULL && p->right != NULL){
				freespot = p->right;
				while (freespot->left != NULL)
					freespot = freespot->left;
				freespot->left = p->left;
				p->left->parent = freespot;

				p->right->parent = p->parent;
				set_parent_reloc(p, p->right);
			}
			else if (p->left != NULL){
				p->left->parent = p->parent;
				set_parent_reloc(p, p->left);
			}
			else if (p->right != NULL){
				p->right->parent = p->parent;
				set_parent_reloc(p, p->right);
			}
			else {
				set_parent_reloc(p, NULL);
			}

			node_alloc.destroy(p);
			node_alloc.deallocate(p, 1);
			_size--;
		}

		iterator	add(const T & e) {
			node_ptr spot = find_prev_or(e);
			if (spot == NULL) {
				head = create_new(e);
				_size++;
				return iterator(head);
			}
			else if (!_comparator(e, *(spot->elem))
					 && !_comparator(*(spot->elem), e))
				return (iterator(spot));
			else {
				if (_comparator(e, *(spot->elem))){
					spot->left = create_new(e);
					_size++;
					spot->left->parent = spot;
					spot = spot->left;
				}
				else {
					spot->right = create_new(e);
					_size++;
					spot->right->parent = spot;
					spot = spot->right;
				}
			}
			return (iterator(spot));
		}

		size_type 	remove(const T & e) {
			node_ptr spot = find_prev_or(e);
			if (spot != NULL
				&& !_comparator(e, *(spot->elem))
				&& !_comparator(*(spot->elem), e)){
				remove_node(spot);
				return 1;
			}
			return 0;
		}

		void	remove(iterator it) {
			node_ptr temp = it._base();
			if (temp != NULL){
				remove_node(temp);
			}
		}

		void	remove(iterator first, iterator last) {
			difference_type	len = 0;
			iterator temp = first;
			while (temp != last){
				temp++;
				len++;
			}
			node_ptr * arr = new node_ptr[len];
			long i = 0;

			while (first != last)
				arr[i++] = (first++)._base();
			i = 0;
			while (i < len){
				remove_node(arr[i++]);
			}
			delete[] arr;
		}

		void clear(node_ptr node){
			if (node->right != NULL)
				clear(node->right);
			if (node->left != NULL)
				clear(node->left);
			node_alloc.destroy(node);
			node_alloc.deallocate(node, 1);
			_size--;
		}

	};

	// #################################################################################
	///BTree iterator specialization
	template<typename T>
	class bidirectional_iterator<BTree<T> >:  public iterator<bidirectional_iterator_tag, T > {
	private:
		typedef bidirectional_iterator<BTree<T> > It;

		typename BTree<T>::BTreeNode * current;
		bool	_end;
		bool	_start;
	public:
		typedef long	difference_type;
		typedef T 									value_type;
		typedef typename BTree<T>::BTreeNode::node_ptr 		pointer;
		typedef T&									reference;
		typedef bidirectional_iterator_tag			iterator_category;

		bidirectional_iterator(pointer c): _end(false), _start(false) {current = c; };
		bidirectional_iterator(const It & inst) : _end(inst._end), _start(inst._start), current(inst.current) { };
		~bidirectional_iterator() {};

		// operators
		It & operator=(const It & rhs){
			_end = rhs._end;
			_start = rhs._start;
			current = rhs.current;
			return *this;
		}

		pointer _base() {
			return current;
		}

		It  operator++(int){
			It temp = *this;
			if (_start){
				_start = false;
				return *this;
			}
			if (_end) //undefined but still dealt with
				return temp;
			if (current->right != NULL) {
				current = current->right;
				while (current->left != NULL)
					current = current->left;
			}
			else {
				It dangle = *this;
				while (current->parent != NULL &&
				current == current->parent->right) {
					current = current->parent;
				}
				if (current->parent != NULL)
					current = current->parent;
				else{
					*this = dangle;
					_end = true;
				}
			}
			return temp;
		}

		It &  operator++(){
			if (_start){
				_start = false;
				return *this;
			}
			if (_end) //undefined but still dealt with
				return *this;
			if (current->right != NULL) {
				current = current->right;
				while (current->left != NULL)
					current = current->left;
			}
			else {
				It dangle = *this;
				while (current->parent != NULL &&
					   current == current->parent->right) {
					current = current->parent;
				}
				if (current->parent != NULL)
					current = current->parent;
				else {
					*this = dangle;
					_end = true;
				}
			}
			return *this;
		}

		It operator--(int){
			It temp = *this;
			if (_end){
				_end = false;
				return temp;
			}
			if (_start) //undefined but still dealt with
				return temp;
			if (current->left != NULL) {
				current = current->left;
				while (current->right != NULL)
					current = current->right;
			}
			else {
				It dangle = *this;
				while (current->parent != NULL &&
					   current == current->parent->left) {
					current = current->parent;
				}
				if (current->parent != NULL)
					current = current->parent;
				else{
					*this = dangle;
					_start = true;
				}
			}
			return temp;
		}

		It &  operator--(){
			if (_end){
				_end = false;
				return *this;
			}
			if (_start) //undefined but still dealt with
				return *this;
			if (current->left != NULL) {
				current = current->left;
				while (current->right != NULL)
					current = current->right;
			}
			else {
				It dangle = *this;
				while (current->parent != NULL &&
					   current == current->parent->left) {
					current = current->parent;
				}
				if (current->parent != NULL)
					current = current->parent;
				else{
					*this = dangle;
					_start = true;
				}
			}
			return *this;
		}

		reference  operator*() const{
			return current->elem;
		}
		bool  operator==(const It & rhs) const{
			return current == rhs.current && _end == rhs._end && _start == rhs._start;
		}
		bool  operator!=(const It & rhs) const{
			return !(*this == rhs);
		}

	};
}

#endif //FT_CONTAINERS_B_TREE_HPP
