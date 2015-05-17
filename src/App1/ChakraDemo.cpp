#include "pch.h"
#include "ChakraDemo.h"
#include "HStringWrap.h"
#include <fstream>
#include <iostream>
#include <assert.h>
#include <wrl.h>

using namespace App1;
using namespace Platform;

JsValueRef CALLBACK CreateGreeter(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
{
	JsErrorCode err;

	Microsoft::WRL::ComPtr<IInspectable> obj;
	HStringWrap classId(L"WindowsRuntimeComponent1.Greeter");

	Microsoft::WRL::ComPtr<IActivationFactory> factory;
	HRESULT hr = RoGetActivationFactory(classId, IID_IActivationFactory, reinterpret_cast<void**>(factory.GetAddressOf()));
	assert(SUCCEEDED(hr));

	hr = factory->ActivateInstance(&obj);
	assert(SUCCEEDED(hr));

	JsValueRef ret = nullptr;
	err = JsInspectableToObject(obj.Get(), &ret);
	assert(JsNoError == err);

	return ret;
}

JsValueRef CALLBACK SayWorld(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
{
	JsErrorCode err;

	// The first argument is "this", in our case it is the global object
	JsValueRef *jsGreeter = arguments + 1;

	Microsoft::WRL::ComPtr<IInspectable> obj;
	err = JsObjectToInspectable(*jsGreeter, &obj);
	assert(JsNoError == err);

	WindowsRuntimeComponent1::Greeter^ greeter = reinterpret_cast<WindowsRuntimeComponent1::Greeter^>(obj.Get());
	String^ s = greeter->SayWorld();

	JsValueRef ret;
	err = JsPointerToString(s->Data(), s->Length(), &ret);
	assert(JsNoError == err);

	return ret;
}

ChakraDemo::ChakraDemo()
{
	JsErrorCode err;

	err = JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &m_runtime);
	assert(JsNoError == err);

	err = JsCreateContext(m_runtime, &m_context);
	assert(JsNoError == err);

	err = JsSetCurrentContext(m_context);
	assert(JsNoError == err);

	err = JsStartDebugging();
	assert(JsNoError == err);

	CreateGlobalFuncion(L"sayWorld", SayWorld);

	CreateGlobalFuncion(L"createGreeter", CreateGreeter);

	err = JsProjectWinRTNamespace(L"WindowsRuntimeComponent1");
	assert(JsNoError == err);
}

void ChakraDemo::CreateGlobalFuncion(const wchar_t *funcName, JsNativeFunction func)
{
	JsErrorCode err;
	JsValueRef global;

	err = JsGetGlobalObject(&global);
	assert(JsNoError == err);

	JsValueRef jsFunc;
	err = JsCreateFunction(func, nullptr, &jsFunc);
	assert(JsNoError == err);

	JsPropertyIdRef funcNameProp;
	err = JsGetPropertyIdFromName(funcName, &funcNameProp);
	assert(JsNoError == err);
	
	err = JsSetProperty(global, funcNameProp, jsFunc, true);
	assert(JsNoError == err);
}

String^ ChakraDemo::GetGreeting()
{
	JsErrorCode err;

	std::wifstream in(L"app.js");
	assert(in);

	std::wstring script((std::istreambuf_iterator<wchar_t>(in)), std::istreambuf_iterator<wchar_t>());
	
	JsValueRef result;
	JsSourceContext contextCookie = 0;
	err = JsRunScript(script.c_str(), contextCookie, L"app.js", &result);
	assert(JsNoError == err);

	const wchar_t *pStr = nullptr;
	size_t len = 0;
	err = JsStringToPointer(result, &pStr, &len);
	assert(JsNoError == err);

	return ref new String(pStr, len);
}

ChakraDemo::~ChakraDemo()
{
	JsErrorCode err;

	err = JsSetCurrentContext(JS_INVALID_REFERENCE);
	assert(JsNoError == err);

	err = JsDisposeRuntime(m_runtime);
	assert(JsNoError == err);
}