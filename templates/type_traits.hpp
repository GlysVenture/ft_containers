//
// Created by tkondrac on 6/4/22.
//

#ifndef FT_CONTAINERS_TYPE_TRAITS_HPP
#define FT_CONTAINERS_TYPE_TRAITS_HPP

namespace ft {
	///is_integral<T>::value default
	template< class T >
	struct is_integral {
		static const bool value = false;
		bool operator()(){
			return value;
		}

		typedef bool	value_type;
		//todo type typedef?
	};

	///is_integral<bool>::value
	template<>
	struct is_integral<bool> {
		static const bool value = true;
		bool operator()(){
			return value;
		}

		typedef bool	value_type;
	};

	///is_integral<char>::value
	template<>
	struct is_integral<char> {
		static const bool value = true;
		bool operator()(){
			return value;
		}

		typedef bool	value_type;
	};

	///is_integral<wchar_t>::value
	template<>
	struct is_integral<wchar_t> {
		static const bool value = true;
		bool operator()(){
			return value;
		}

		typedef bool	value_type;
	};

	///is_integral<short>::value
	template<>
	struct is_integral<short> {
		static const bool value = true;
		bool operator()(){
			return value;
		}

		typedef bool	value_type;
	};

	///is_integral<int>::value
	template<>
	struct is_integral<int> {
		static const bool value = true;
		bool operator()(){
			return value;
		}

		typedef bool	value_type;
	};

	///is_integral<long>::value
	template<>
	struct is_integral<long> {
		static const bool value = true;
		bool operator()(){
			return value;
		}

		typedef bool	value_type;
	};

	///is_integral<long long>::value
	template<>
	struct is_integral<long long> {
		static const bool value = true;
		bool operator()(){
			return value;
		}

		typedef bool	value_type;
	};
}

#endif //FT_CONTAINERS_TYPE_TRAITS_HPP
