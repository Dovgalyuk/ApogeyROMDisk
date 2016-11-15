// ��������, ����������, ASIS ������ ���������� VinLib. � �������� ���������
// (�) 5-12-2011 vinxru

#ifndef VINLIB_WINAPI_EXCEPTION_H
#define VINLIB_WINAPI_EXCEPTION_H

#include "vinlib/string.h"

// ��������� ������ ������ Windows 
void getOsErrorMessage(const char* prefix, string& error);

// ����� ��������������
void warning_os(const char* preFix);

// ��������� ���������� � ����������� ������ ������ Windows
void raise_os(const char* preFix);
inline void raise_os(cstring text) { raise_os(text.c_str()); }

// �������� ������� �������
#ifndef _CONSOLE
int main(const char*);
#else
int cmain(int argc, const char** argv);
#endif

#endif