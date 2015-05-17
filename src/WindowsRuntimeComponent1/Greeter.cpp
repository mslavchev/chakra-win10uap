#include "pch.h"

namespace WindowsRuntimeComponent1
{
	public ref class Greeter sealed
	{
	public:
		Platform::String^ SayHello()
		{
			return ref new Platform::String(L"Hello ");
		}
		Platform::String^ SayWorld()
		{
			return ref new Platform::String(L"World");
		}
	};
}