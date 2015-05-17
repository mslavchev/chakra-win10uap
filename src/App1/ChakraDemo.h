#pragma once
#define USE_EDGEMODE_JSRT
#include <jsrt.h>

namespace App1
{
	class ChakraDemo
	{
	public:
		ChakraDemo();

		Platform::String^ GetGreeting();

		~ChakraDemo();

	private:
		void CreateGlobalFuncion(const wchar_t *funcName, JsNativeFunction func);

		JsRuntimeHandle m_runtime;
		JsContextRef m_context;
	};
}
