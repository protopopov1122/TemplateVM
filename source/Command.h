#ifndef COMMAND_H_
#define COMMAND_H_

#include "Value.h"
#include "Output.h"

template <typename C, typename S>
class Interpret {};

template <typename V>
class PushCommand {
 public:
	using Value = V;
};

class DupCommand {};

class SwapCommand {};

class RotCommand {};

class DropCommand {};

class PrintCommand {};

template <int64_t M>
class TestCommand {
 public:
	static constexpr int64_t Mask = M;
};

template <std::size_t A>
class JumpCommand {
 public:
	static constexpr std::size_t Address = A;
};

class CompareCommand {};

template <std::size_t A>
class JumpIfCommand {
 public:
	static constexpr std::size_t Address = A;
};

template <std::size_t A>
class CallCommand {
 public:
	static constexpr std::size_t Address = A;
};

class RetCommand {};

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


template <typename S>
class BinaryCommand {
 protected:
	using Right = typename S::Stack::Pop;
	using Left = typename Right::ResultStack::Pop;
};

template <typename V, typename S>
class Interpret<PushCommand<V>, S> {
 public:
	using ResultState = State<S::PC, typename S::Stack::template Push<V>, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<DupCommand, S> {
 public:
	using ResultState = State<S::PC, typename S::Stack::template Push<typename S::Stack::Peek::Value>, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<SwapCommand, S> : public BinaryCommand<S> {
 private:
	using Result = typename BinaryCommand<S>::Left::ResultStack::template Push<typename BinaryCommand<S>::Right::Value>::template Push<typename BinaryCommand<S>::Left::Value>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<RotCommand, S> {
 private:
	using Top = typename S::Stack::Pop;
	using Middle = typename Top::ResultStack::Pop;
	using Bottom = typename Middle::ResultStack::Pop;
	using Result = typename Bottom::ResultStack::template Push<typename Middle::Value>::template Push<typename Top::Value>::template Push<typename Bottom::Value>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<DropCommand, S> {
 private:	
	using Argument = typename S::Stack::Pop;
	using Result = typename Argument::ResultStack;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Add>, S> : public BinaryCommand<S> {
 private:
	using Result = typename BinaryCommand<S>::Left::ResultStack::template Push<IntValue<BinaryCommand<S>::Left::Value::Value + BinaryCommand<S>::Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Subtract>, S> : public BinaryCommand<S> {
 private:
	using Result = typename BinaryCommand<S>::Left::ResultStack::template Push<IntValue<BinaryCommand<S>::Left::Value::Value - BinaryCommand<S>::Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Multiply>, S> : public BinaryCommand<S> {
 private:
	using Result = typename BinaryCommand<S>::Left::ResultStack::template Push<IntValue<BinaryCommand<S>::Left::Value::Value * BinaryCommand<S>::Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Divide>, S> : public BinaryCommand<S> {
 private:
	using Result = typename BinaryCommand<S>::Left::ResultStack::template Push<IntValue<BinaryCommand<S>::Left::Value::Value / BinaryCommand<S>::Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Modulo>, S> : public BinaryCommand<S> {
 private:
	using Result = typename BinaryCommand<S>::Left::ResultStack::template Push<IntValue<BinaryCommand<S>::Left::Value::Value % BinaryCommand<S>::Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<PrintCommand, S> {
 private:
	using Result = typename S::Stack::Pop;
	using Print = typename S::Print::template PrintLn<typename Result::Value>;
 public:
	using ResultState = State<S::PC, typename Result::ResultStack, typename S::CallStack, Print>;
};

template <std::size_t Address, typename S>
class Interpret<JumpCommand<Address>, S> {
 public:
	using ResultState = State<Address, typename S::Stack, typename S::CallStack, typename S::Print>;
};

template <typename A, typename B, typename E = void>
struct Compare_Impl {};

template <typename A, typename B>
struct Compare_Impl<A, B, typename std::enable_if_t<A::Value == B::Value>> {
	static constexpr int64_t Result = 0b1;
};

template <typename A, typename B>
struct Compare_Impl<A, B, typename std::enable_if_t<(A::Value > B::Value)>> {
	static constexpr int64_t Result = 0b10;
};

template <typename A, typename B>
struct Compare_Impl<A, B, typename std::enable_if_t<(A::Value < B::Value)>> {
	static constexpr int64_t Result = 0b100;
};

template <typename S>
class Interpret<CompareCommand, S> : public BinaryCommand<S> {
	using Result = typename BinaryCommand<S>::Left::ResultStack::template Push<IntValue<Compare_Impl<typename BinaryCommand<S>::Left::Value, typename BinaryCommand<S>::Right::Value>::Result>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <int64_t Mask, typename S>
class Interpret<TestCommand<Mask>, S> {
 private:	
	using Argument = typename S::Stack::Pop;
	using Result = typename Argument::ResultStack::template Push<IntValue<(Argument::Value::Value & Mask) != 0>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <std::size_t Address, typename S, typename A, typename E = void>
class JumpIf_Impl {
 public:
	using ResultState = S;
};

template <std::size_t Address, typename S, typename A>
class JumpIf_Impl<Address, S, A, typename std::enable_if_t<A::Value != 0>> {
 public:
	using ResultState = State<Address, typename S::Stack, typename S::CallStack, typename S::Print>;
};

template <std::size_t Address, typename S>
class Interpret<JumpIfCommand<Address>, S> {
 private:
	using Argument = typename S::Stack::Pop;
	using Result = State<S::PC, typename Argument::ResultStack, typename S::CallStack, typename S::Print>;
 public:
	using ResultState = typename JumpIf_Impl<Address, Result, typename Argument::Value>::ResultState;
};

template <std::size_t Address, typename S>
class Interpret<CallCommand<Address>, S> {
	using Result = typename S::CallStack::template Push<IntValue<S::PC>>;
 public:
	using ResultState = State<Address, typename S::Stack, Result, typename S::Print>;
};

template <typename S>
class Interpret<RetCommand, S> {
	using Result = typename S::CallStack::Pop;
 public:
	using ResultState = State<Result::Value::Value, typename S::Stack, typename Result::ResultStack, typename S::Print>;
};

#endif
