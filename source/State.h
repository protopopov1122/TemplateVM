#ifndef STATE_H_
#define STATE_H_

#include "Output.h"

template <template <typename ... Values> typename StackType, typename T, typename ... V>
class Stack_Peek_Impl {
 public:
	using Value = T;
	using ResultStack = StackType<T, V...>;
};


template <template <typename ... Values> typename StackType, typename T, typename ... V>
class Stack_Pop_Impl {
 public:
	using Value = T;
	using ResultStack = StackType<V...>;
};

template <typename ... V>
class Stack {
 public:
	static constexpr std::size_t Size = sizeof...(V);
	struct Peek : public Stack_Peek_Impl<Stack, V...> {};
	struct Pop : public Stack_Pop_Impl<Stack, V...> {};
	template <typename T>
	using Push = Stack<T, V...>;
};

template <std::size_t I, typename S, typename C, typename P = EmptyPrinter>
class State {
 public:
	static constexpr std::size_t PC = I;
	using Stack = S;
	using Print = P;
	using CallStack = C;
};

using EmptyState = State<0, Stack<>, Stack<>>;

#endif
