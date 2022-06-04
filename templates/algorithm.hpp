//
// Created by tkondrac on 6/4/22.
//

#ifndef FT_CONTAINERS_ALGORITHM_HPP
#define FT_CONTAINERS_ALGORITHM_HPP

namespace ft {

	///equal()
	template< class InputIt1, class InputIt2 >
	bool equal( InputIt1 first1, InputIt1 last1,
				InputIt2 first2 ) {
		while (first1 != last1) {
			if (*first1 != *first2)
				return false;
			first1++;
			first2++;
		}
		return true;
	}

	///equal() with p closure
	template< class InputIt1,
		class InputIt2,
		class BinaryPredicate >
	bool equal( InputIt1 first1,
				InputIt1 last1,
				InputIt2 first2,
				BinaryPredicate p ){
		while (first1 != last1) {
			if (!p(*first1, *first2))
				return false;
			first1++;
			first2++;
		}
		return true;
	}

	template< class InputIt1, class InputIt2 >
	bool lexicographical_compare( InputIt1 first1, InputIt1 last1,
								  InputIt2 first2, InputIt2 last2 )
	{
		while (first1 != last1 && first2 != last2) {
			if (*first1 < *first2)
				return true;
			else if (*first1 > *first2)
				return false;
			first1++;
			first2++;
		}
		if (first1 == last1 && first2 != last2)
			return true;
		return false;
	}

	template< class InputIt1, class InputIt2, class Compare >
	bool lexicographical_compare( InputIt1 first1, InputIt1 last1,
								  InputIt2 first2, InputIt2 last2,
								  Compare comp )
	{
		while (first1 != last1 && first2 != last2) {
			if (comp(*first1, *first2))
				return true;
			else if (comp(*first2, *first1))
				return false;
			first1++;
			first2++;
		}
		if (first1 == last1 && first2 != last2)
			return true;
		return false;
	}
}

#endif //FT_CONTAINERS_ALGORITHM_HPP
