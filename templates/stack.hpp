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

		container_type	_cont;
	public:
		explicit stack( const container_type& cont = Container() ): _cont(cont) {  }
		stack( const stack& other ): _cont(other._cont) {  }
		~stack() {  }

		bool empty() const { return _cont.empty(); }

		size_type size() const { return _cont.size(); }

		reference top() { return _cont.back(); }

		const_reference top() const { return _cont.back(); }

		void push(const value_type &val) { _cont.push_back(val); }

		void pop() { _cont.pop_back(); }

	};

	template< class T, class Container >
	bool operator==(const stack< T, Container > &lhs, const stack< T, Container > &rhs)
	{
		return (lhs._cont == rhs._cont);
	}


	template< class T, class Container >
	bool operator!=(const stack< T, Container > &lhs, const stack< T, Container > &rhs)
	{
		return (!(lhs == rhs));
	}


	template< class T, class Container >
	bool operator<(const stack< T, Container > &lhs, const stack< T, Container > &rhs)
	{
		return (lhs._cont < rhs._cont);
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
