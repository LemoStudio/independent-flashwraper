#include "UtilHelper.h"
std::wstring Utf8ToUnicode(const std::string& str) {
	// Ԥ��-�������п��ֽڵĳ���    
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	// ��ָ�򻺳�����ָ����������ڴ�    
	wchar_t* pUnicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
	// ��ʼ�򻺳���ת���ֽ�    
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pUnicode, unicodeLen);
	std::wstring ret_str = pUnicode;
	free(pUnicode);
	return ret_str;
}

std::string UnicodeToAscii(const std::wstring& wstr) {
	// Ԥ��-�������ж��ֽڵĳ���    
	int ansiiLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	// ��ָ�򻺳�����ָ����������ڴ�    
	char* pAssii = (char*)malloc(sizeof(char) * ansiiLen);
	// ��ʼ�򻺳���ת���ֽ�    
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
	std::string ret_str = pAssii;
	free(pAssii);
	return ret_str;
}
std::string Utf8ToAscii(const std::string& str) {
	return UnicodeToAscii(Utf8ToUnicode(str));
}