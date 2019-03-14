#include <cstdlib>
#include "TemplateVM.h"

int main(int argc, const char **argv) {
	using I = Interpreter<Code<
		JumpCommand<16>,
// Procedure Factorial
		PushCommand<1>,
		SwapCommand,
		DupCommand,
		RotCommand,
		OperationCommand<CommandOpcode::Multiply>,
		SwapCommand,
		PushCommand<1>,
		OperationCommand<CommandOpcode::Subtract>,
		DupCommand,
		PushCommand<1>,
		CompareCommand,
		TestCommand<0b10>,
		JumpIfCommand<3>,
		DropCommand,
		RetCommand,
// Call Factorial(5)
		PushCommand<5>,
		CallCommand<1>,
		PrintCommand,
// Call Factorial(10)
		PushCommand<10>,
		CallCommand<1>,
		PrintCommand
	>, EmptyState>;
	using R = typename RunInterpreter<I>::Result;
	R::Print::flush(std::cout);
	return EXIT_SUCCESS;
}
