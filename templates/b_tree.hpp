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
	template<typename T, class Compare = std::less<T>, class Allocator = std::allocator<T> >
	class BTree {
	public:
		//typedefs
		typedef random_access_iterator<BTree<T> >				iterator;
		typedef const random_access_iterator<BTree<T> >			const_iterator;
		typedef size_t											size_type;
		typedef long											difference_type;
		typedef typename Allocator::pointer						pointer;
		typedef typename Allocator::const_pointer				const_pointer;

	private:
		///node class
		class BTreeNode {
			friend class BTree;
			friend class random_access_iterator<BTree<T> >;
		private:
			//priv typedefs
			typedef typename Allocator::template rebind_alloc<BTreeNode>	Node_Alloc;
			typedef typename Node_Alloc::pointer	node_ptr;

			BTree::pointer	elem;
			node_ptr	 	left;
			node_ptr	 	parent;
			node_ptr	 	right;
		public:
			BTreeNode(): parent(NULL), left(NULL), right(NULL) {  }
			explicit BTreeNode(const T & new_elem): parent(NULL), left(NULL), right(NULL) {
				elem = alloc.allocate(1);
				alloc.construct(elem, new_elem);
			}
			BTreeNode(const BTreeNode & node): parent(NULL) {
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

		//priv typedefs
		typedef typename Allocator::template rebind_alloc<BTreeNode>	Node_Alloc;
		typedef typename Node_Alloc::pointer	node_ptr;

		//todo hmmm?
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


		//Btree
		static std::allocator<BTreeNode>	node_alloc;
		static Allocator					alloc;
		node_ptr							head;
		size_t								size;
		value_compare						_comparator;

		explicit BTree(const elem_compare &comp = elem_compare()): head(NULL), size(0), _comparator(comp) {  }
		BTree(const BTree & inst): size(inst.size) {
			_comparator = inst._comparator;
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
			node_alloc.construct(n, BTreeNode(e));
		}

		void	remove_node(node_ptr p) {
			node_ptr freespot;

			freespot = p->right;
			while (freespot->left != NULL)
				freespot = freespot->left;
			freespot->left = p->left;
			p->left->parent = freespot;

			p->right->parent = p->parent;
			if (p->parent == NULL)
				head = p->right;
			else if (p->parent->left == p)
				p->parent->left = p->right;
			else
				p->parent->right = p->right;

			node_alloc.destroy(p);
			node_alloc.deallocate(p, 1);
		}

		iterator	add(const T & e) {
			node_ptr spot = find_prev_or(e);
			if (spot == NULL) {
				head = create_new(e);
				return iterator(head);
			}
			else if (!_comparator(e, *(spot->elem))
					 && !_comparator(*(spot->elem), e))
				return (iterator(spot));
			else {
				if (_comparator(e, *(spot->elem))){
					spot->left = create_new(e);
					spot->left->parent = spot;
				}
				else {
					spot->right = create_new(e);
					spot->right->parent = spot;
				}
			}
			return (iterator(spot));
		}

		void	remove(const T & e) {
			node_ptr spot = find_prev_or(e);
			if (spot != NULL
				&& !_comparator(e, *(spot->elem))
				&& !_comparator(*(spot->elem), e))
				remove_node(spot);
		}

		void	remove(iterator it) {
			node_ptr temp = it._base();
			if (temp != NULL)
				remove_node(temp);
		}

		void	remove(iterator first, iterator last) {
			difference_type	len = last - first;
			node_ptr * arr = new node_ptr[len];
			long i = 0;

			while (first != last)
				arr[i++] = (first++)._base();
			i = 0;
			while (i < len)
				remove_node(arr[i++]);
			delete[] arr;
		}

	};

	// #################################################################################
	///BTree iterator specialization
	template<typename T>
	class random_access_iterator<BTree<T> >:  public iterator<random_access_iterator_tag, T > {
	private:
		typedef random_access_iterator<BTree<T> > It;

		typename BTree<T>::BTreeNode * current;
		bool	_end;
	public:
		typedef long	difference_type;
		typedef T 									value_type;
		typedef typename BTree<T>::BTreeNode::node_ptr 		pointer;
		typedef T&									reference;
		typedef random_access_iterator_tag			iterator_category;

		random_access_iterator(pointer c): _end(false) {current = c; };
		random_access_iterator(const It & inst) : _end(inst._end), current(inst.current) { };
		~random_access_iterator() {};

		// operators
		It & operator=(const It & rhs){
			_end = rhs._end;
			current = rhs.current;
			return *this;
		}

		pointer _base() {
			return current;
		}

		It  operator++(int){
			It temp = *this;
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
			if (current->left != NULL) {
				current = current->left;
				while (current->right != NULL)
					current = current->right;
			}
			else {
				while (current->parent != NULL &&
					   current == current->parent->left) {
					current = current->parent;
				}
				if (current->parent != NULL)
					current = current->parent;
				else
					throw std::runtime_error("undefined substract on begin iterator");
			}
			return temp;
		}

		It &  operator--(){
			if (_end){
				_end = false;
				return *this;
			}
			if (current->left != NULL) {
				current = current->left;
				while (current->right != NULL)
					current = current->right;
			}
			else {
				while (current->parent != NULL &&
					   current == current->parent->left) {
					current = current->parent;
				}
				if (current->parent != NULL)
					current = current->parent;
				else
					throw std::runtime_error("undefined substract on begin iterator");
			}
			return *this;
		}

		reference  operator*() const{
			return current->elem;
		}
		bool  operator==(const It & rhs) const{
			return current == rhs.current;
		}
		bool  operator!=(const It & rhs) const{
			return current != rhs.current;
		}

		It & operator+=(difference_type n) {
			while ( n != 0 ) {
				if (n > 0){
					(*this)++;
					n--;
				}
				else if (n < 0){
					(*this)--;
					n++;
				}
			}
			return *this;
		}

		It & operator-=(difference_type n) {
			while ( n != 0 ) {
				if (n > 0){
					(*this)--;
					n--;
				}
				else if (n < 0){
					(*this)++;
					n++;
				}
			}
			return *this;
		}

		It operator+(difference_type n) const{
			It temp = *this;
			return temp += n;
		}

		It operator-(difference_type n) const{
			It temp = *this;
			return temp -= n;
		}

		difference_type operator-(const random_access_iterator<T> & rhs) const{ //todo better, now dumb?
			difference_type count = 0;
			It temp = *this;
			while (temp._end == false) {
				if (temp == rhs)
					return count;
				count++;
				temp++;
			}
			count = 0;
			temp = *this;
			while (temp != rhs){
				count--;
				temp--;
			}
			return count;
		}

		reference operator[](difference_type n){
			return *(current + n);
		}

		bool operator<(const It & rhs){
			return current->elem < rhs.current->elem;
		}

		bool operator>(const It & rhs){
			return current->elem > rhs.current->elem;
		}

		bool operator<=(const It & rhs){
			return !(*this > rhs);
		}

		bool operator>=(const It & rhs){
			return !(*this < rhs);
		}

	};
}

#endif //FT_CONTAINERS_B_TREE_HPP
