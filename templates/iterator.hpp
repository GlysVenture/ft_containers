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
		class Pointer = T*,
		class Reference = T&>
	struct iterator {
	protected:
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
		typedef T*									pointer;
		typedef T&									reference;
		typedef random_access_iterator_tag			iterator_category;
	};

	template< class T > //todo understand
	struct iterator_traits< const T* > {
		typedef typename std::ptrdiff_t				difference_type;
		typedef	T									value_type;
		typedef const T*							pointer;
		typedef const T&							reference;
		typedef random_access_iterator_tag			iterator_category;
	};

	template<class T>
	class random_access_iterator: public iterator<random_access_iterator_tag, T> {
	private:
		typedef random_access_iterator<T> It;

		typename iterator<random_access_iterator_tag, T>::pointer current;
	public:
		typedef typename iterator<random_access_iterator_tag, T>::difference_type 	difference_type;
		typedef typename iterator<random_access_iterator_tag, T>::value_type 		value_type;
		typedef typename iterator<random_access_iterator_tag, T>::pointer			pointer;
		typedef typename iterator<random_access_iterator_tag, T>::reference			reference;
		typedef typename iterator<random_access_iterator_tag, T>::iterator_category	iterator_category;

		random_access_iterator(pointer c) {current = c; };
		random_access_iterator(const random_access_iterator<T> & inst) { *this = inst; };
		~random_access_iterator() {};

		// #################################################################################
		// operators
		It & operator=(const random_access_iterator<T> & rhs){
			current = rhs.current;
			return *this;
		}

		random_access_iterator<T>  operator++(int){
			random_access_iterator<T> temp = *this;
			current++;
			return temp;
		}

		random_access_iterator<T> &  operator++(){
			current++;
			return *this;
		}

		reference  operator*() const{
			return *current;
		}
		bool  operator==(const random_access_iterator<T> & rhs) const{
			return current == rhs.current;
		}
		bool  operator!=(const random_access_iterator<T> & rhs) const{
			return current != rhs.current;
		}

		random_access_iterator<T>  operator--(int){
			random_access_iterator<T> temp = *this;
			current--;
			return temp;
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

		random_access_iterator<T> operator+(difference_type n) const{
			random_access_iterator<T> temp = *this;
			return temp += n;
		}

		random_access_iterator<T> operator-(difference_type n) const{
			random_access_iterator<T> temp = *this;
			return temp -= n;
		}

		difference_type operator-(const random_access_iterator<T> & rhs) const{
			return current - rhs.current;
		}

		reference operator[](difference_type n){
			return *(current + n);
		}

		bool operator<(const random_access_iterator<T> & rhs){
			return current < rhs.current;
		}

		bool operator>(const random_access_iterator<T> & rhs){
			return current > rhs.current;
		}

		bool operator<=(const random_access_iterator<T> & rhs){
			return !(*this > rhs);
		}

		bool operator>=(const random_access_iterator<T> & rhs){
			return !(*this < rhs);
		}

	};

	//operator + other way around
	template<class T>
	random_access_iterator<T> operator+(const typename random_access_iterator<T>::difference_type n, const random_access_iterator<T> & it) {
		return it + n;
	}

	template< class Iter >
	class reverse_iterator:
		iterator<typename iterator_traits<Iter>::iterator_category,
		typename iterator_traits<Iter>::value_type ,
		typename iterator_traits<Iter>::difference_type ,
		typename iterator_traits<Iter>::pointer ,
		typename iterator_traits<Iter>::reference >
	{
	private:
		Iter	current;
	public:
		typedef Iter												iterator_type;
		typedef typename iterator_traits<Iter>::iterator_category	iterator_category;
		typedef typename iterator_traits<Iter>::value_type			value_type;
		typedef typename iterator_traits<Iter>::difference_type		difference_type;
		typedef typename iterator_traits<Iter>::pointer				pointer;
		typedef typename iterator_traits<Iter>::reference			reference;

		reverse_iterator(): current(iterator_type()) { }
		explicit reverse_iterator(iterator_type x): current(x) { }
		reverse_iterator( const reverse_iterator<Iter>& other ): current(other.current) { }
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

		reverse_iterator  operator++(int){
			reverse_iterator temp = *this;
			current--;
			return temp;
		}

		reverse_iterator &  operator++(){
			current--;
			return *this;
		}

		bool  operator==(const reverse_iterator & rhs) const{
			return current == rhs.current;
		}
		bool  operator!=(const reverse_iterator & rhs) const{
			return current != rhs.current;
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
			reverse_iterator temp = *this;
			return temp -= n;
		}

		reverse_iterator operator-(difference_type n) const{
			reverse_iterator temp = *this;
			return temp += n;
		}

		difference_type operator-(const reverse_iterator & rhs) const{
			return current - rhs.current;
		}

		reference operator[](difference_type n){
			return *(current - n);
		}

		bool operator<(const reverse_iterator & rhs){
			return current > rhs.current;
		}

		bool operator>(const reverse_iterator & rhs){
			return current < rhs.current;
		}

		bool operator<=(const reverse_iterator & rhs){
			return !(*this < rhs);
		}

		bool operator>=(const reverse_iterator & rhs){
			return !(*this > rhs);
		}

	};
}


#endif //FT_CONTAINERS_ITERATOR_HPP
