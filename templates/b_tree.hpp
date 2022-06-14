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
		typedef random_access_iterator<BTree<T> >							iterator;
		typedef const random_access_iterator<BTree<T> >						const_iterator;
		typedef size_t														size_type;
		typedef long														difference_type;
		typedef typename std::allocator_traits<Allocator>::pointer			pointer;
		typedef typename std::allocator_traits<Allocator>::const_pointer	const_pointer;

	private:
		///node class
		class BTreeNode {
			friend class BTree;
			friend class random_access_iterator<BTree<T> >;
		private:
			T			* elem;
			BTreeNode	* left;
			BTreeNode	* parent;
			BTreeNode	* right;
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
				}
				if (node.left != NULL){
					left = node_alloc.allocate(1);
					node_alloc.construct(left, node.left);
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
		typedef std::allocator<BTreeNode>		Node_Alloc;

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
		BTreeNode *							head;
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
			BTreeNode * current = head;
			while (current->left != NULL)
				current = current->left;
			return iterator(current);
		}

		iterator	last() {
			BTreeNode * current = head;
			while (current->right != NULL)
				current = current->right;
			return iterator(current);
		}

		iterator	end() {
			BTreeNode * current = head;
			while (current->right != NULL)
				current = current->right;
			return (iterator(current)++);
		}

		iterator	find(const T & e){
			BTreeNode * current = head;
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

		///attention trouve des pointeurs a modifier!
		BTreeNode **	find_ptr(const T & e){
			BTreeNode ** current = &head;
			while (*current != NULL) {
				if (_comparator(*(*current->elem), e))
				{
					*current = *current->right;
				}
				else if (_comparator(e, *(*current->elem)))
				{
					current = *current->left;
				}
				else {
					return current;
				}
			}
			return current;
		}

		typename Node_Alloc::pointer create_new(const T & e) {
			typename Node_Alloc::pointer n = node_alloc.allocate(1);
			node_alloc.construct(n, BTreeNode(e));
		}

		void	remove_node(typename Node_Alloc::pointer p) {
			node_alloc.destroy(p);
			node_alloc.deallocate(p, 1);
		}

		iterator	add(const T & e) {
			BTreeNode ** spot = find_ptr(e);
			if (*spot == NULL) {
				*spot = create_new(e);
			}
			return (iterator(*spot));
		}

		//todo much work
		void	remove(const T & e) {
			BTreeNode ** spot = find_ptr(e);
			if (*spot != NULL) {
				remove_node(*spot);
				*spot = NULL;
			}
		}

		void	remove(iterator it) {
			BTreeNode * temp = it._base();
			if (temp->parent != NULL){
				if (temp->parent->left == temp)
					temp->parent->left = NULL;
				else
					temp->parent->right = NULL;
			}
			remove_node(temp);
		}

		void	remove(iterator first, iterator last) {

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
		typedef typename BTree<T>::BTreeNode *		pointer;
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
