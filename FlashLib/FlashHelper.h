#pragma once

#include <atlbase.h>
//#include <atlwin.h>
//#include <comdef.h>
#include "oaidl.h"
#include <CommCtrl.h>
#include "atlhost.h"
#include "lib/flash32_24_0_0_221.tlh"
#include <string>
#pragma comment(lib,"atls.lib")

using namespace ShockwaveFlashObjects;

typedef void (*FlashCallHandler)(const char*);
class FlashSink : public ShockwaveFlashObjects::_IShockwaveFlashEvents
{
public:
	LPCONNECTIONPOINT       m_ConnectionPoint;
	DWORD                   m_dwCookie;
	int                     m_nRefCount;
	FlashCallHandler _handler;

public:
	FlashSink()
	{
		m_dwCookie = 0;
		m_ConnectionPoint = NULL;
		m_nRefCount = 0;
	}

	virtual ~FlashSink()
	{

	}
	void SetCallBack(FlashCallHandler _handler)
	{
		this->_handler = _handler;
	}
	HRESULT Init(CComPtr<IShockwaveFlash> ptrFlash)
	{
		HRESULT aResult = NOERROR;
		LPCONNECTIONPOINTCONTAINER aConnectionPoint = NULL;
		if ((ptrFlash->QueryInterface(IID_IConnectionPointContainer, (void**)&aConnectionPoint) == S_OK) &&
			(aConnectionPoint->FindConnectionPoint(__uuidof(ShockwaveFlashObjects::_IShockwaveFlashEvents), &m_ConnectionPoint) == S_OK))
		{
			IDispatch* aDispatch = NULL;
			QueryInterface(__uuidof(IDispatch), (void**)&aDispatch);
			if (aDispatch != NULL)
			{
				aResult = m_ConnectionPoint->Advise((LPUNKNOWN)aDispatch, &m_dwCookie);
				//aDispatch->Release();
			}
		}

		/*if (aConnectionPoint != NULL)
			aConnectionPoint->Release();*/

		return aResult;
	}

	HRESULT Shutdown()
	{
		HRESULT aResult = S_OK;

		if (m_ConnectionPoint)
		{
			if (m_dwCookie)
			{
				aResult = m_ConnectionPoint->Unadvise(m_dwCookie);
				m_dwCookie = 0;
			}

			//m_ConnectionPoint->Release();
			//m_ConnectionPoint = NULL;
		}

		return aResult;
	}

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID* ppv)
	{
		*ppv = NULL;

		if (riid == IID_IUnknown)
		{
			*ppv = (LPUNKNOWN)this;
			AddRef();
			return S_OK;
		}
		else if (riid == IID_IDispatch)
		{
			*ppv = (IDispatch*)this;
			AddRef();
			return S_OK;
		}
		else if (riid == __uuidof(ShockwaveFlashObjects::_IShockwaveFlashEvents))
		{
			*ppv = (ShockwaveFlashObjects::_IShockwaveFlashEvents*)this;
			AddRef();
			return S_OK;
		}
		else
		{
			return E_NOTIMPL;
		}
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{
		return ++m_nRefCount;
	}

	ULONG STDMETHODCALLTYPE Release()
	{
		int aRefCount = --m_nRefCount;
		/*if (aRefCount == 0)
		{
			if (m_dwCookie)
			{
				m_ConnectionPoint->Unadvise(m_dwCookie);
				m_dwCookie = 0;
			}
		}*/
		return aRefCount;
	}

	// IDispatch method   
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* pctinfo)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames,
		UINT cNames, LCID lcid, DISPID* rgDispId)
	{
		return E_NOTIMPL;
	}

	//_CRT_SECURE_NO_WARNINGS

	virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
		WORD wFlags, ::DISPPARAMS __RPC_FAR* pDispParams, VARIANT __RPC_FAR* pVarResult,
		::EXCEPINFO __RPC_FAR* pExcepInfo, UINT __RPC_FAR* puArgErr)
	{
		/*TCHAR dispid[10] = { 0 };
		_stprintf(dispid, _T("%ld"), dispIdMember);
		OutputDebugString(dispid);*/
		switch (dispIdMember)
		{
		case 0x7a6:
			break;
		case 0x96:
			if ((pDispParams->cArgs == 2) &&
				(pDispParams->rgvarg[0].vt == VT_BSTR) &&
				(pDispParams->rgvarg[1].vt == VT_BSTR))
			{
				FSCommand(pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].bstrVal);
			}
			break;
		case DISPID_READYSTATECHANGE:
			break;
		case  197:
		{
			auto ret= pDispParams;
			auto val = ret->rgvarg->bstrVal;
			_bstr_t bs1(val, false);
			const char* str = static_cast <const char*>(bs1);
			std::string raw(str);
			int lpos= raw.find("string");
			int rpos = raw.rfind("string");
			std::string msg=raw.substr(lpos + 6+1, rpos - lpos - 6-3);
			
			//int unicodeLen = MultiByteToWideChar(CP_ACP, 0, msg.c_str(), -1, nullptr, 0);
			//wchar_t* pUnicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
			//// 开始向缓冲区转换字节    
			//MultiByteToWideChar(CP_UTF8, 0, msg.c_str(), -1, pUnicode, unicodeLen);
			//std::wstring ret_str = pUnicode;
			//free(pUnicode);

			if (_handler)
			{
				_handler(msg.c_str());
			}
		}
			break;
		default:
			return DISP_E_MEMBERNOTFOUND;
		}

		return NOERROR;
	}

	HRESULT OnReadyStateChange(long newState)
	{
		return S_OK;
	}

	HRESULT OnProgress(long percentDone)
	{
		return S_OK;
	}

	HRESULT FSCommand(_bstr_t command, _bstr_t args)
	{
		//         if (m_pFlashWidget != NULL)   
		//             m_pFlashWidget->FlashCommand((char*) command, (char*) args);   
		return S_OK;
	}
};

using namespace System;
using namespace System::Runtime::InteropServices;
namespace FlashLib
{
	[UnmanagedFunctionPointer(CallingConvention::StdCall)]
	public delegate void FlashMessageHandler(String^ msg);
	public ref class FlashWraper
	{
	public:
		~FlashWraper();
		void Init(System::IntPtr hwnd);
		void CallFunction(String^ cmd);
		void Release();
		void LoadFlash(String^ swf);

		event FlashMessageHandler^ FlashMessage;
		void FlashMsg(String^ msg);
	private:
		IntPtr g_hwnd;
		GCHandle gchandle;
		FlashMessageHandler^ mdelegate;
	};
}