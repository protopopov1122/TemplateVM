#ifndef COMMAND_H_
#define COMMAND_H_

#include "Value.h"
#include "Output.h"

template <int64_t V>
struct PushCommand {
	static constexpr int64_t Value = V;
};

struct DupCommand {};

struct SwapCommand {};

struct RotCommand {};

struct DropCommand {};

template <std::size_t I>
struct CopyCommand {
	static constexpr std::size_t Index = I;
};

struct PrintCommand {};

template <int64_t M>
struct TestCommand {
	static constexpr int64_t Mask = M;
};

template <std::size_t A>
struct JumpCommand {
	static constexpr std::size_t Address = A;
};

struct CompareCommand {};

template <std::size_t A>
struct JumpIfCommand {
	static constexpr std::size_t Address = A;
};

template <std::size_t A>
struct CallCommand {
	static constexpr std::size_t Address = A;
};

struct RetCommand {};

enum class CommandOpcode {
	Add,
	Subtract,
	Multiply,
	Divide,
	Modulo,
	And,
	Or,
	Xor,
	Not,
	Negate,
	LogicalNot
};

template <CommandOpcode O>
struct OperationCommand {
	static constexpr CommandOpcode Opcode = O;
};

#endif
