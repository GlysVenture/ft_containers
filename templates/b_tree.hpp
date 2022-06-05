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
	template<typename T>
	class BTree {
	public:
		//typedefs
		typedef random_access_iterator<BTree<T> >	iterator;
		typedef size_t								size_type;
	private:
		///node class
		class BTreeNode {
			friend class BTree;
			friend class random_access_iterator<BTree<T> >;
		private:
			T			elem;
			BTreeNode * left;
			BTreeNode * parent;
			BTreeNode * right;
		public:
			BTreeNode(): parent(NULL), left(NULL), right(NULL) {  }
			explicit BTreeNode(const T & new_elem): elem(new_elem), parent(NULL), left(NULL), right(NULL) {  }
			BTreeNode(const BTreeNode & node): elem(node.elem), parent(NULL) {
				if (node.right != NULL){
					right = alloc.allocate(1);
					alloc.construct(right, node.right);
				}
				if (node.left != NULL){
					left = alloc.allocate(1);
					alloc.construct(left, node.left);
				}
			}
			~BTreeNode() {
				if (right != NULL){
					alloc.destroy(right);
					alloc.deallocate(right, 1);
				}
				if (left != NULL){
					alloc.destroy(left);
					alloc.deallocate(left, 1);
				}
			}
		};


		//Btree
		static std::allocator<BTreeNode>	alloc;
		BTreeNode *							head;
		size_t								size;

		BTree(): head(NULL), size(0) {}
		BTree(const BTree & inst): size(inst.size) {
			if (inst.head == NULL){
				head = NULL;
				return;
			}
			head = alloc.allocate(1);
			alloc.construct(head, *(inst.head));
		}
		~BTree() {
			if (head != NULL) {
				alloc.destroy(head);
				alloc.deallocate(head, 1);
			}
		}

		//needed funcs todo add remove find begin last end
		iterator	add(const T & e) {

		}
		size_type	remove(const T & e) {

		}
		size_type	remove(iterator it) {

		}size_type	remove(iterator first, iterator last) {

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
				while (current->parent != NULL &&
				current == current->parent->right) {
					current = current->parent;
				}
				if (current->parent != NULL)
					current = current->parent;
				else
					_end = true;
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
				while (current->parent != NULL &&
					   current == current->parent->right) {
					current = current->parent;
				}
				if (current->parent != NULL)
					current = current->parent;
				else
					_end = true;
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
