// FlashHelper.cpp : Defines the entry point for the application.
//

#include "FlashHelper.h"
#include <ostream>
#include <windowsx.h>
CComPtr<IShockwaveFlash> pFlash;
FlashSink    flashSink;
CComModule _Module;
HMODULE	m_hModule;


HRESULT CreateInstanceFromFile(const TCHAR* pcszPath,
	REFCLSID rclsid,
	REFIID riid,
	IUnknown* pUnkOuter,
	LPVOID* ppv)
{
	m_hModule = ::LoadLibraryEx(pcszPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (m_hModule == NULL)
		return TYPE_E_CANTLOADLIBRARY;

	typedef HRESULT(STDAPICALLTYPE* _pfnDllGetClassObject)(REFCLSID, REFIID, LPVOID*);
	_pfnDllGetClassObject pfnDllGetClassObject = (_pfnDllGetClassObject)GetProcAddress(m_hModule, "DllGetClassObject");
	if (pfnDllGetClassObject == NULL)
		return CO_E_ERRORINDLL;

	CComPtr<IClassFactory> spCF;
	HRESULT hr = (*pfnDllGetClassObject)(rclsid, IID_IClassFactory, (LPVOID*)&spCF);
	if (FAILED(hr))
		return hr;

	hr = spCF->CreateInstance(pUnkOuter, riid, ppv);
	if (FAILED(hr))
		return hr;

	return S_OK;
}
void FlashLib::FlashWraper::FlashMsg(String^ msg)
{
	FlashMessage(msg);
}
FlashLib::FlashWraper::~FlashWraper()
{
	Release();
}

void FlashLib::FlashWraper::Init(System::IntPtr hwnd)
{
	HRESULT hr;
	TCHAR curDirPath[MAX_PATH];
	//_pAtlModule = &_Module;
	::GetModuleFileName(NULL, curDirPath, MAX_PATH);
	::PathRemoveFileSpec(curDirPath);
	TCHAR szPath[MAX_PATH];
	wsprintf(szPath, _T("%s\\Flash32_24_0_0_221.ocx"), curDirPath);
	hr = CreateInstanceFromFile(szPath, __uuidof(ShockwaveFlash), __uuidof(IShockwaveFlash), NULL, (void**)&pFlash);
	if (SUCCEEDED(hr))
	{
		hr = AtlAxAttachControl(pFlash, static_cast<HWND>(hwnd.ToPointer()), NULL);
		flashSink.Init(pFlash);
		mdelegate = gcnew FlashMessageHandler(this, &FlashWraper::FlashMsg);
		gchandle = GCHandle::Alloc(mdelegate);
		IntPtr pFunc = Marshal::GetFunctionPointerForDelegate(mdelegate);
		flashSink.SetCallBack(static_cast<FlashCallHandler>(pFunc.ToPointer()));
	}
}

void FlashLib::FlashWraper::CallFunction(String^ cmd)
{
	if (pFlash)
	{		
		String^ str = "<invoke name=\"addText\" returntype=\"xml\"><arguments><string>";
		str += cmd;
		str += "</string></arguments></invoke>";
		IntPtr pStrPtr = Marshal::StringToHGlobalUni(str);
		TCHAR* pStr = static_cast<TCHAR*>(pStrPtr.ToPointer());
		pFlash->CallFunction(pStr);
		Marshal::FreeHGlobal(pStrPtr);
	}
}

void FlashLib::FlashWraper::Release()
{
	if (pFlash)
	{
		pFlash->Stop();
		flashSink.Shutdown();
		pFlash.Release();
		pFlash = NULL;
	}
	AtlAxWinTerm();
	::CoUninitialize();
	if (m_hModule)
	{
		try
		{
			::FreeLibraryAndExitThread(m_hModule, 0);
			m_hModule = NULL;
		}
		catch (...)
		{
		}
	}
}

void FlashLib::FlashWraper::LoadFlash(String^ swf)
{
	IntPtr pStrPtr = Marshal::StringToHGlobalUni(swf);
	TCHAR* pStr = static_cast<TCHAR*>(pStrPtr.ToPointer());
	auto hr = pFlash->LoadMovie(0, pStr);
	pFlash->AllowFullScreen = _bstr_t("true");
	pFlash->ScaleMode = 2;
	pFlash->BackgroundColor = pFlash->BackgroundColor;
	Marshal::FreeHGlobal(pStrPtr);	
}
