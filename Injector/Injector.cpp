#include "stdafx.h"
#include <vector>
#include "Injector.h"
#include <vcclr.h>
#include "Bootstrap.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Reflection;

static unsigned int RandomName2 = ::RegisterWindowMessage(L"RandomText0");
static HHOOK _randomHookHandleName0;

__declspec(dllexport)
LRESULT __stdcall RandomName0(int nCode, WPARAM wparam, LPARAM lparam)
{
	if (!GetModuleHandleA("mscorlib.dll")) {
		if (!Bootstrapper::GetInstance()->Initialize())
		{
			Console::WriteLine("CLR could not be loaded.");
		}
	}
	if (nCode == HC_ACTION)
	{
		CWPSTRUCT* msg = (CWPSTRUCT*)lparam;
		if (msg != NULL && msg->message == RandomName2)
		{
			unsigned char* bytePointer = (unsigned char*)msg->wParam;
			int byteLength = (int)msg->lParam;
			if (byteLength != NULL && bytePointer != NULL)
			{
				Injector::RandomRefClassName::DLL = gcnew array<System::Byte>(byteLength);
				::Marshal::Copy(IntPtr((void*)bytePointer), Injector::RandomRefClassName::DLL, 0, byteLength);
				::Assembly^ assembly = ::Assembly::Load(Injector::RandomRefClassName::DLL);
				
				if (assembly != nullptr)
				{
					auto types = assembly->GetTypes();
					for (int i = 0; i < types->Length; i++)
					{
						auto methods = types[i]->GetMethods(::BindingFlags::Public | ::BindingFlags::Static);
						for (int ii = 0; ii < methods->Length; ii++)
						{
							for each(auto attribute in methods[ii]->CustomAttributes)
							{
								if (attribute->AttributeType->Name == "STAThreadAttribute")
								{
									System::Object^ result = methods[ii]->Invoke(nullptr, nullptr);
									if (result != nullptr)
										throw gcnew System::Exception("Unexpected Result:" + result->ToString());

									return CallNextHookEx(_randomHookHandleName0, nCode, wparam, lparam);
								}
							}
						}
					}
				}
			}
		}
	}
	return CallNextHookEx(_randomHookHandleName0, nCode, wparam, lparam);
}

int __stdcall RandomName1(int windowHandle, long address, int nSize)
{
	try
	{
		if (windowHandle <= 0 || address <= 0 || nSize <= 0)
			return 0;

		HINSTANCE hinstDLL;
		if (::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)&RandomName0, &hinstDLL))
		{
			DWORD processID = 0;
			DWORD threadID = ::GetWindowThreadProcessId((HWND)windowHandle, &processID);
			if (processID)
			{
				_randomHookHandleName0 = ::SetWindowsHookEx(WH_CALLWNDPROC, &RandomName0, hinstDLL, threadID);
				if (_randomHookHandleName0)
				{
					::SendMessage((HWND)windowHandle, RandomName2, (WPARAM)address, nSize);
					::UnhookWindowsHookEx(_randomHookHandleName0);
				}
			}
		}
		::FreeLibrary(hinstDLL);;
		return 1;
	}
	catch (const std::exception& e)
	{
		MessageBoxA(NULL, "Injection Failed.\r\n", "NULLPTR ERROR", NULL);
	}
}
