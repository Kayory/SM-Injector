#pragma once

__declspec(dllexport) 
LRESULT __stdcall RandomName0(int nCode, WPARAM wparam, LPARAM lparam);
extern "C" { __declspec(dllexport) int __stdcall RandomName1(int windowHandle, int address, int nSize); }

using namespace System;

namespace Injector {

	public ref class RandomRefClassName : System::Object
	{
		internal:static array<System::Byte>^ DLL;
	};
}
