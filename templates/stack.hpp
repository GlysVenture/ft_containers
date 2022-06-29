//
// Created by Tadeusz Kondracki on 6/29/22.
//

#ifndef FT_CONTAINERS_STACK_HPP
# define FT_CONTAINERS_STACK_HPP

#include "vector.hpp"

namespace ft {


	template<typename T, class Container = ft::vector<T> >
	class stack {
	public:
		typedef Container								container_type;
		typedef typename Container::value_type			value_type;
		typedef typename Container::size_type			size_type;
		typedef	typename Container::reference			reference;
		typedef typename Container::const_reference		const_reference;
	private:
		template< class V, class C >
		friend bool operator==(const stack< V, C > &lhs,
							   const stack< V, C > &rhs);

		template< class V, class C >
		friend bool operator<(const stack< V, C > &lhs,
							  const stack< V, C > &rhs);
	protected:
		container_type	c;
	public:
		explicit stack( const container_type& cont = Container() ): c(cont) {  }
		stack( const stack& other ): c(other.c) {  }
		~stack() {  }

		bool empty() const { return c.empty(); }

		size_type size() const { return c.size(); }

		reference top() { return c.back(); }

		const_reference top() const { return c.back(); }

		void push(const value_type &val) { c.push_back(val); }

		void pop() { c.pop_back(); }

	};

	template< class T, class Container >
	bool operator==(const stack< T, Container > &lhs, const stack< T, Container > &rhs)
	{
		return (lhs.c == rhs.c);
	}


	template< class T, class Container >
	bool operator!=(const stack< T, Container > &lhs, const stack< T, Container > &rhs)
	{
		return (!(lhs == rhs));
	}


	template< class T, class Container >
	bool operator<(const stack< T, Container > &lhs, const stack< T, Container > &rhs)
	{
		return (lhs.c < rhs.c);
	}


	template< class T, class Container >
	bool operator<=(const stack< T, Container > &lhs, const stack< T, Container > &rhs)
	{
		return (!(rhs < lhs));
	}


	template< class T, class Container >
	bool operator>(const stack< T, Container > &lhs, const stack< T, Container > &rhs)
	{
		return (rhs < lhs);
	}


	template< class T, class Container >
	bool operator>=(const stack< T, Container > &lhs, const stack< T, Container > &rhs)
	{
		return !(lhs < rhs);
	}
}

#endif //FT_CONTAINERS_STACK_HPP
