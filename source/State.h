#ifndef STATE_H_
#define STATE_H_

#include "Output.h"

template <template <typename ... Values> typename StackType, typename T, typename ... V>
struct Stack_Peek_Impl {
	using Value = T;
	using ResultStack = StackType<T, V...>;
};


template <template <typename ... Values> typename StackType, typename T, typename ... V>
struct Stack_Pop_Impl {
	using Value = T;
	using ResultStack = StackType<V...>;
};

template <std::size_t I, typename A, typename ... B>
struct Stack_Get_Impl : public Stack_Get_Impl<I - 1, B...> {};

template <typename A, typename ... B>
struct Stack_Get_Impl<0, A, B...> {
	using Value = A;
};

template <typename ... V>
struct Stack {
	static constexpr std::size_t Size = sizeof...(V);
	struct Peek : public Stack_Peek_Impl<Stack, V...> {};
	struct Pop : public Stack_Pop_Impl<Stack, V...> {};
	template <typename T>
	using Push = Stack<T, V...>;
	template <std::size_t I>
	struct Get : public Stack_Get_Impl<I, V...> {};
};

template <std::size_t I, typename S, typename C, typename P = EmptyPrinter>
struct State {
	static constexpr std::size_t PC = I;
	using Stack = S;
	using Print = P;
	using CallStack = C;
};

using EmptyState = State<0, Stack<>, Stack<>>;

#endif
