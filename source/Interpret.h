#ifndef INTERPRET_H_
#define INTERPRET_H_

#include "Command.h"

template <typename C, typename S>
struct Interpret {};

template <typename S>
class BinaryInterpret {
 protected:
	using Right = typename S::Stack::Pop;
	using Left = typename Right::ResultStack::Pop;
};

template <int64_t V, typename S>
struct Interpret<PushCommand<V>, S> {
	using ResultState = State<S::PC, typename S::Stack::template Push<IntValue<V>>, typename S::CallStack, typename S::Print>;
};

template <typename S>
struct Interpret<DupCommand, S> {
	using ResultState = State<S::PC, typename S::Stack::template Push<typename S::Stack::Peek::Value>, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<SwapCommand, S> : public BinaryInterpret<S> {
 private:
	using Result = typename BinaryInterpret<S>::Left::ResultStack::template Push<typename BinaryInterpret<S>::Right::Value>::template Push<typename BinaryInterpret<S>::Left::Value>;
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
class Interpret<OperationCommand<CommandOpcode::Add>, S> : public BinaryInterpret<S> {
 private:
	using Result = typename BinaryInterpret<S>::Left::ResultStack::template Push<IntValue<BinaryInterpret<S>::Left::Value::Value + BinaryInterpret<S>::Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Subtract>, S> : public BinaryInterpret<S> {
 private:
	using Result = typename BinaryInterpret<S>::Left::ResultStack::template Push<IntValue<BinaryInterpret<S>::Left::Value::Value - BinaryInterpret<S>::Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Multiply>, S> : public BinaryInterpret<S> {
 private:
	using Result = typename BinaryInterpret<S>::Left::ResultStack::template Push<IntValue<BinaryInterpret<S>::Left::Value::Value * BinaryInterpret<S>::Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Divide>, S> : public BinaryInterpret<S> {
 private:
	using Result = typename BinaryInterpret<S>::Left::ResultStack::template Push<IntValue<BinaryInterpret<S>::Left::Value::Value / BinaryInterpret<S>::Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Modulo>, S> : public BinaryInterpret<S> {
 private:
	using Result = typename BinaryInterpret<S>::Left::ResultStack::template Push<IntValue<BinaryInterpret<S>::Left::Value::Value % BinaryInterpret<S>::Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::And>, S> : public BinaryInterpret<S> {
 private:
	using Result = typename BinaryInterpret<S>::Left::ResultStack::template Push<IntValue<BinaryInterpret<S>::Left::Value::Value & BinaryInterpret<S>::Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Or>, S> : public BinaryInterpret<S> {
 private:
	using Result = typename BinaryInterpret<S>::Left::ResultStack::template Push<IntValue<BinaryInterpret<S>::Left::Value::Value | BinaryInterpret<S>::Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Xor>, S> : public BinaryInterpret<S> {
 private:
	using Result = typename BinaryInterpret<S>::Left::ResultStack::template Push<IntValue<BinaryInterpret<S>::Left::Value::Value ^ BinaryInterpret<S>::Right::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Not>, S> {
 private:	
	using Argument = typename S::Stack::Pop;
	using Result = typename Argument::ResultStack::template Push<IntValue<~Argument::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::LogicalNot>, S> {
 private:	
	using Argument = typename S::Stack::Pop;
	using Result = typename Argument::ResultStack::template Push<IntValue<!Argument::Value::Value>>;
 public:
	using ResultState = State<S::PC, Result, typename S::CallStack, typename S::Print>;
};

template <typename S>
class Interpret<OperationCommand<CommandOpcode::Negate>, S> {
 private:	
	using Argument = typename S::Stack::Pop;
	using Result = typename Argument::ResultStack::template Push<IntValue<-Argument::Value::Value>>;
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

template <std::size_t I, typename S>
class Interpret<CopyCommand<I>, S> {
 private:
	using Result = typename S::Stack::template Get<I>::Value;
 public:
	using ResultState = State<S::PC, typename S::Stack::template Push<Result>, typename S::CallStack, typename S::Print>;
};

template <std::size_t Address, typename S>
struct Interpret<JumpCommand<Address>, S> {
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
class Interpret<CompareCommand, S> : public BinaryInterpret<S> {
 private:
	using Result = typename BinaryInterpret<S>::Left::ResultStack::template Push<IntValue<Compare_Impl<typename BinaryInterpret<S>::Left::Value, typename BinaryInterpret<S>::Right::Value>::Result>>;
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
struct JumpIf_Impl {
	using ResultState = S;
};

template <std::size_t Address, typename S, typename A>
struct JumpIf_Impl<Address, S, A, typename std::enable_if_t<A::Value != 0>> {
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
 private:
	using Result = typename S::CallStack::template Push<IntValue<S::PC>>;
 public:
	using ResultState = State<Address, typename S::Stack, Result, typename S::Print>;
};

template <typename S>
class Interpret<RetCommand, S> {
 private:
	using Result = typename S::CallStack::Pop;
 public:
	using ResultState = State<Result::Value::Value, typename S::Stack, typename Result::ResultStack, typename S::Print>;
};

#endif
