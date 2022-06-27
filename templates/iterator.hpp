//
// Created by Tadeusz Kondracki on 5/13/22.
//

#ifndef FT_CONTAINERS_ITERATOR_HPP
# define FT_CONTAINERS_ITERATOR_HPP
namespace ft {

	struct input_iterator_tag { };
	struct output_iterator_tag { };
	struct forward_iterator_tag : public input_iterator_tag { };
	struct bidirectional_iterator_tag : public forward_iterator_tag { };
	struct random_access_iterator_tag : public bidirectional_iterator_tag { };

	template<class Category,
		class T,
		class Distance = std::ptrdiff_t,
		class Pointer = T *,
		class Reference = T&>
	struct iterator {
		typedef Distance			difference_type;
		typedef T					value_type;
		typedef Pointer				pointer;
		typedef Reference			reference;
		typedef Category			iterator_category;
	};


	template< class Iter >
	struct iterator_traits{
		typedef typename Iter::difference_type		difference_type;
		typedef typename Iter::value_type			value_type;
		typedef typename Iter::pointer				pointer;
		typedef typename Iter::reference			reference;
		typedef typename Iter::iterator_category	iterator_category;
	};

	template< class T > //todo understand
	struct iterator_traits< T* > {
		typedef typename std::ptrdiff_t				difference_type;
		typedef	T									value_type;
		typedef T *									pointer;
		typedef T &									reference;
		typedef random_access_iterator_tag			iterator_category;
	};

	template< class T > //todo understand
	struct iterator_traits< const T * > {
		typedef typename std::ptrdiff_t				difference_type;
		typedef	T									value_type;
		typedef const T *							pointer;
		typedef const T &							reference;
		typedef random_access_iterator_tag			iterator_category;
	};

	template< class InputIt >
	typename ft::iterator_traits< InputIt >::difference_type distance(InputIt first, InputIt last)
	{
		typename ft::iterator_traits< InputIt >::difference_type dist = 0;
		while (first != last)
		{
			first++;
			dist++;
		}
		return (dist);
	}

	///random access iterator
	template<class T>
	class random_access_iterator: public iterator<random_access_iterator_tag, T> {
	private:
		typedef random_access_iterator<T> 	It;
		typedef iterator_traits< T * >			traits;


	public:
		typedef typename traits::difference_type 	difference_type;
		typedef typename traits::value_type 		value_type;
		typedef typename traits::pointer			pointer;
		typedef typename traits::reference			reference;
		typedef typename traits::iterator_category	iterator_category;
	private:
		pointer current;
	public:

		random_access_iterator(pointer c): current(c) {  };
		random_access_iterator(): current(NULL) {  };
		template<class V>
		random_access_iterator(const random_access_iterator<V> & inst): current(inst._base()) { };
		~random_access_iterator() {};

		// #################################################################################
		// operators

		pointer _base() const { return current; };

		It & operator=(const It & rhs){
			current = rhs.current;
			return *this;
		}

		It  operator++(int){
			return It(current++);
		}

		It & operator++(){
			++current;
			return *this;
		}

		reference  operator*() const{
			return *current;
		}

		pointer operator->() const { return &(operator*()); }

		template<class V>
		bool  operator==(const random_access_iterator<V> & rhs) const{
			return current == rhs._base();
		}
		template<class V>
		bool  operator!=(const random_access_iterator<V> & rhs) const{
			return current != rhs._base();
		}

		It  operator--(int){
			return It(current--);
		}

		random_access_iterator<T> &  operator--(){
			current--;
			return *this;
		}

		random_access_iterator<T> & operator+=(difference_type n) {
			current += n;
			return *this;
		}

		random_access_iterator<T> & operator-=(difference_type n) {
			current -= n;
			return *this;
		}

		It operator+(difference_type n) const{
			return It(current + n);
		}

		It operator-(difference_type n) const{
			return It(current - n);
		}

		template<class V>
		difference_type operator-(const random_access_iterator<V> & rhs) const{
			return current - rhs._base();
		}

		reference operator[](difference_type n){
			return *(current + n);
		}

		template<class V>
		bool operator<(const random_access_iterator<V> & rhs) const{
			return current < rhs._base();
		}

		template<class V>
		bool operator>(const random_access_iterator<V> & rhs) const{
			return current > rhs._base();
		}

		template<class V>
		bool operator<=(const random_access_iterator<V> & rhs) const{
			return !(*this > rhs);
		}

		template<class V>
		bool operator>=(const random_access_iterator<V> & rhs) const{
			return !(*this < rhs);
		}

	};

	//operator + other way around
	template<class T>
	random_access_iterator<T> operator+(const typename ft::iterator_traits< random_access_iterator<T> >::difference_type n, const random_access_iterator<T> & it) {
		return it + n;
	}

	///Bidirectional iterator
	template<class T>
	class bidirectional_iterator: public iterator<bidirectional_iterator_tag, T> {
	private:
		typedef bidirectional_iterator<T> It;
		typedef iterator_traits< T * >	traits;

	public:
		typedef typename traits::difference_type 	difference_type;
		typedef typename traits::value_type 		value_type;
		typedef typename traits::pointer			pointer;
		typedef typename traits::reference			reference;
		typedef typename traits::iterator_category	iterator_category;
	private:
		pointer current;
	public:

		bidirectional_iterator(): current(NULL) {  };
		bidirectional_iterator(pointer c): current(c) {  };

		template<class V>
		bidirectional_iterator(const bidirectional_iterator<V> & inst): current(inst._base()) {  };
		~bidirectional_iterator() {};

		// #################################################################################
		// operators

		pointer _base() const { return current; };

		It & operator=(const It & rhs){
			current = rhs.current;
			return *this;
		}

		It  operator++(int){
			random_access_iterator<T> temp = *this;
			current++;
			return temp;
		}

		It &  operator++(){
			current++;
			return *this;
		}

		reference  operator*() const{
			return *current;
		}

		pointer operator->() const { return &(operator*()); }

		bool  operator==(const It & rhs) const{
			return current == rhs.current;
		}
		bool  operator!=(const It & rhs) const{
			return current != rhs.current;
		}

		It  operator--(int){
			It temp = *this;
			current--;
			return temp;
		}

		It &  operator--(){
			current--;
			return *this;
		}

	};

	//operator + other way around
	template<class T>
	bidirectional_iterator<T> operator+(const typename ft::iterator_traits< bidirectional_iterator<T> >::difference_type n, const bidirectional_iterator<T> & it) {
		return it + n;
	}

	///Reverse iterator
	template< class Iter >
	class reverse_iterator: public iterator<
	    typename iterator_traits<Iter>::iterator_category,
		typename iterator_traits<Iter>::value_type ,
		typename iterator_traits<Iter>::difference_type ,
		typename iterator_traits<Iter>::pointer ,
		typename iterator_traits<Iter>::reference >
	{
	private:
		typedef iterator_traits<Iter> traits;
	public:
		typedef Iter								iterator_type;
		typedef typename traits::iterator_category	iterator_category;
		typedef typename traits::value_type			value_type;
		typedef typename traits::difference_type	difference_type;
		typedef typename traits::pointer			pointer;
		typedef typename traits::reference			reference;
	private:
		iterator_type 	current;
	public:

		reverse_iterator(): current(iterator_type()) { }
		explicit reverse_iterator(iterator_type x): current(x) { }

		template<class ItO>
		reverse_iterator( const reverse_iterator<ItO>& other ): current(other.base()) { }
		~reverse_iterator() { };

		reverse_iterator& operator=( const reverse_iterator<Iter>& other ) {
			current = other.current;
			return *this;
		}

		iterator_type base() const{
			return current;
		}

		reference operator*() const{
			return *(current - 1);
		}

		pointer operator->() const { return &(operator*()); }

		reverse_iterator  operator++(int){
			reverse_iterator temp = *this;
			current--;
			return temp;
		}

		reverse_iterator &  operator++(){
			current--;
			return *this;
		}

		template<class ItO>
		bool  operator==(const reverse_iterator<ItO> & rhs) const{
			return current == rhs.base();
		}
		template<class ItO>
		bool  operator!=(const reverse_iterator<ItO> & rhs) const{
			return current != rhs.base();
		}

		reverse_iterator  operator--(int){
			reverse_iterator temp = *this;
			current++;
			return temp;
		}

		reverse_iterator &  operator--(){
			current++;
			return *this;
		}

		reverse_iterator & operator+=(difference_type n) {
			current -= n;
			return *this;
		}

		reverse_iterator & operator-=(difference_type n) {
			current += n;
			return *this;
		}

		reverse_iterator operator+(difference_type n) const{
			return reverse_iterator(current - n);
		}

		reverse_iterator operator-(difference_type n) const{
			return reverse_iterator(current + n);
		}

		template<class ItO>
		difference_type operator-(const reverse_iterator<ItO> & rhs) const{
			return rhs.base() - current;
		}

		reference operator[](difference_type n){
			return *(current - n - 1);
		}

		template<class ItO>
		bool operator<(const reverse_iterator<ItO> & rhs) const{
			return current > rhs.base();
		}

		template<class ItO>
		bool operator>(const reverse_iterator<ItO> & rhs) const{
			return current < rhs.base();
		}

		template<class ItO>
		bool operator<=(const reverse_iterator<ItO> & rhs) const{
			return !(*this > rhs);
		}

		template<class ItO>
		bool operator>=(const reverse_iterator<ItO> & rhs) const{
			return !(*this < rhs);
		}

	};

	//operator + other way around
	template<class T>
	reverse_iterator<T> operator+(const typename ft::iterator_traits< reverse_iterator<T> >::difference_type n, const reverse_iterator<T> & it) {
		return (it + n);
	}

}


#endif //FT_CONTAINERS_ITERATOR_HPP
