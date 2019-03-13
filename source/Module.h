#ifndef MODULE_H_
#define MODULE_H_

#include <cstdlib>
#include <cinttypes>

template <std::size_t Index, typename ... I>
struct Code_At_Impl {};

template <typename T, typename ... I>
struct Code_At_Impl<0, T, I...> {
	using Instruction = T;
};

template <std::size_t Index, typename T, typename ... I>
struct Code_At_Impl<Index, T, I...> : public Code_At_Impl<Index - 1, I...> {};

template <typename ... I>
class Code {
 public:
	static constexpr std::size_t Length = sizeof...(I);
	
	template <std::size_t Index>
	struct At : public Code_At_Impl<Index, I...> {};
};

#endif
