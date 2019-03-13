#include <cstdlib>
#include <cinttypes>
#include <type_traits>
#include <iostream>
#include <typeinfo>
#include "Module.h"
#include "Value.h"
#include "State.h"
#include "Command.h"
#include "Interpreter.h"

int main(int argc, const char **argv) {
	using I = Interpreter<Code<
		PushCommand<IntValue<2>>,
		PushCommand<IntValue<2>>,
		OperationCommand<CommandOpcode::Multiply>,
		PushCommand<IntValue<2>>,
		OperationCommand<CommandOpcode::Add>
	>, EmptyState>;
	using R = typename RunInterpreter<I>::Result;
	std::cout << R::Stack::Pop::Value::Value << std::endl;
	return EXIT_SUCCESS;
}
