#ifndef COMMAND_H_
#define COMMAND_H_

#include "Value.h"

template <typename C, typename S>
class Interpret {};

template <typename V>
class PushCommand {
 public:
	using Value = V;
};

enum class CommandOpcode {
	Add,
	Subtract,
	Multiply,
	Divide,
	Modulo
};

template <CommandOpcode O>
class OperationCommand {
 public:
	static constexpr CommandOpcode Opcode = O;
};

template <typename V, typename S>
class Interpret<PushCommand<V>, S> {
 public:
	using ResultState = State<S::PC, typename S::Stack::template Push<V>>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Add>, S> {
 private:
	using Right = typename S::Stack::Pop;
	using Left = typename Right::ResultStack::Pop;
	using Result = typename Left::ResultStack::template Push<IntValue<Left::Value::Value + Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Subtract>, S> {
 private:
	using Right = typename S::Stack::Pop;
	using Left = typename Right::ResultStack::Pop;
	using Result = typename Left::ResultStack::template Push<IntValue<Left::Value::Value - Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Multiply>, S> {
 private:
	using Right = typename S::Stack::Pop;
	using Left = typename Right::ResultStack::Pop;
	using Result = typename Left::ResultStack::template Push<IntValue<Left::Value::Value * Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Divide>, S> {
 private:
	using Right = typename S::Stack::Pop;
	using Left = typename Right::ResultStack::Pop;
	using Result = typename Left::ResultStack::template Push<IntValue<Left::Value::Value / Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Modulo>, S> {
 private:
	using Right = typename S::Stack::Pop;
	using Left = typename Right::ResultStack::Pop;
	using Result = typename Left::ResultStack::template Push<IntValue<Left::Value::Value % Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result>;
};

#endif
