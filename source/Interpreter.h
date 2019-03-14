#ifndef INTERPRETER_H_
#define INTERPRETER_H_

template <typename M, std::size_t I, typename E = void>
struct NextCommand_Impl {
	using Command = void;
};

template <typename M, std::size_t I>
struct NextCommand_Impl<M, I, typename std::enable_if_t<I < M::Length>> {
	using Command = typename M::template At<I>::Instruction;
};


template <typename I, typename E = void>
struct InterpreterResult_Impl {
	using Result = typename I::State;
};

template <typename I>
class InterpreterResult_Impl<I, typename std::enable_if_t<!std::is_same<typename I::Command, void>::value>> {
 private:
	using NextPC = State<I::State::PC + 1, typename I::State::Stack, typename I::State::CallStack, typename I::State::Print>;
 public:
	using Result = typename Interpret<typename I::Command, NextPC>::ResultState;
};


template <typename M, typename S>
struct Interpreter {
	using Module = M;
	using State = S;

	using Command = typename NextCommand_Impl<Module, State::PC>::Command;
	using Result = typename InterpreterResult_Impl<Interpreter<Module, State>>::Result;
	using Next = Interpreter<Module, Result>;
};


template <typename I, typename E = void>
struct RunInterpreter {};

template <typename I>
struct RunInterpreter<I, typename std::enable_if_t<std::is_same<typename I::Command, void>::value>> {
	using Result = typename I::State;
};

template <typename I>
struct RunInterpreter<I, typename std::enable_if_t<!std::is_same<typename I::Command, void>::value>> : public RunInterpreter<typename I::Next> {};

#endif
