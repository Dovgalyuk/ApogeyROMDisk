// ��������, ����������, ASIS ������ ���������� VinLib. � �������� ���������
// (�) 5-12-2011 vinxru

#ifndef VINLIB_STRING_H
#define VINLIB_STRING_H

#include <stdlib.h>
#include <string>
#include <vector>
#include <tchar.h>

// ��� ��������
typedef TCHAR char_t;

#ifdef UNICODE
typedef const std::wstring& cstring;
typedef std::wstring string;
#else
typedef const std::string& cstring;
typedef std::string string;
#endif

// ������� ���-�� �������� � ������
int charsCount(const char* ptr, char chr);

// ���������� ������ �� ��������� (� ��������� ���� ������ std::string)
void explodeText(std::vector<string>& out, const char* str);
inline void explodeText(std::vector<string>& text, cstring str) { explodeText(text, str.c_str()); }

// ������ � ������� ������� (� ��������� ���� ������ std::string)
extern const unsigned char upperCaseTbl[];
string upperCase(const char* in);
inline string upperCase(cstring str) { return upperCase(str.c_str()); }

// ������ � ���������� �����
inline string i2s(int d) { char buf[256]; sprintf_s(buf, sizeof(buf), "%i", d); return buf; }

// ������ � ����������������� �����
inline string hex(int d) { char buf[256]; sprintf_s(buf, sizeof(buf), "%X", d); return buf; }

#endif