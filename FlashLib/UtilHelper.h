#pragma once
#include <string>
#include <windows.h>
std::wstring Utf8ToUnicode(const std::string& str);

std::string UnicodeToAscii(const std::wstring& wstr);
std::string Utf8ToAscii(const std::string& str);

