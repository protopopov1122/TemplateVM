#ifndef VALUE_H_
#define VALUE_H_

#include <cinttypes>

template <typename T, T V>
class AbstractValue {
 public:
	static constexpr T Value = V;
};

template <int64_t V>
using IntValue = AbstractValue<int64_t, V>;

#endif
