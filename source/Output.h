#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <iostream>

template <typename ... T>
struct Print_Impl {
	static void print(std::ostream &) {}
};

template <typename A, typename ... B>
struct Print_Impl<A, B...> {
	static void print(std::ostream &os) {
		os << A::Value << std::endl;
		Print_Impl<B...>::print(os);
	};
};



template <typename ... T>
class Printer {
 public:
	static void flush(std::ostream &os) {
		Print_Impl<T...>::print(os);
	};

	template <typename V>
	using PrintLn = Printer<T..., V>;
};

using EmptyPrinter = Printer<>;

#endif
