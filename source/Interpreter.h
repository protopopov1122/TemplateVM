#ifndef INTERPRETER_H_
#define INTERPRETER_H_

template <typename M, std::size_t I, typename E = void>
class NextCommand_Impl {
 public:
	using Command = void;
};

template <typename M, std::size_t I>
class NextCommand_Impl<M, I, typename std::enable_if_t<I < M::Length>> {
 public:
	using Command = typename M::template At<I>::Instruction;
};


template <typename I, typename E = void>
class InterpreterResult_Impl {
 public:
	using Result = typename I::State;
};

template <typename I>
class InterpreterResult_Impl<I, typename std::enable_if_t<!std::is_same<typename I::Command, void>::value>> {
 public:
	using NextPC = State<I::State::PC + 1, typename I::State::Stack, typename I::State::CallStack, typename I::State::Print>;
	using Result = typename Interpret<typename I::Command, NextPC>::ResultState;
};


template <typename M, typename S>
class Interpreter {
 public:
	using Module = M;
	using State = S;

	using Command = typename NextCommand_Impl<Module, State::PC>::Command;
	using Result = typename InterpreterResult_Impl<Interpreter<Module, State>>::Result;
	using Next = Interpreter<Module, Result>;
};


template <typename I, typename E = void>
class RunInterpreter {};

template <typename I>
class RunInterpreter<I, typename std::enable_if_t<std::is_same<typename I::Command, void>::value>> {
 public:
	using Result = typename I::State;
};

template <typename I>
class RunInterpreter<I, typename std::enable_if_t<!std::is_same<typename I::Command, void>::value>> : public RunInterpreter<typename I::Next> {};

#endif
