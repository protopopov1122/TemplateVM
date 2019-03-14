## Compile-time VM

It's one-evening project that aims to implement a simple stack-based virtual machine in C++ which runs completely in compile-time. Currently it implements common VM infrastructure, stack control, arithmetical and flow control commands. The main reason of this project - curiosity to write entirely compile-time virtual machine.

Example (see `main.cpp`):
```C++
#include <cstdlib>
#include "TemplateVM.h"

int main(int argc, const char **argv) {
	using I = Interpreter<Code<
		JumpCommand<16>,
// Procedure Factorial
		PushCommand<IntValue<1>>,
		SwapCommand,
		DupCommand,
		RotCommand,
		OperationCommand<CommandOpcode::Multiply>,
		SwapCommand,
		PushCommand<IntValue<1>>,
		OperationCommand<CommandOpcode::Subtract>,
		DupCommand,
		PushCommand<IntValue<1>>,
		CompareCommand,
		TestCommand<0b10>,
		JumpIfCommand<3>,
		DropCommand,
		RetCommand,
// Call Factorial(5)
		PushCommand<IntValue<5>>,
		CallCommand<1>,
		PrintCommand,
// Call Factorial(10)
		PushCommand<IntValue<10>>,
		CallCommand<1>,
		PrintCommand
	>, EmptyState>;
	using R = typename RunInterpreter<I>::Result;
	R::Print::flush(std::cout);
	return EXIT_SUCCESS;
}
```

#### Author & License
Author: Jevgenijs Protopopovs \
All project code is licensed under the terms of WTFPL
