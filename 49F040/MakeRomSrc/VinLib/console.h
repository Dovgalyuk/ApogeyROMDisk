// ��������, ����������, ASIS ������ ���������� VinLib. � �������� ���������
// (�) 5-12-2011 vinxru

#ifndef VINLIB_CONSOLE_H
#define VINLIB_CONSOLE_H

#include "vinlib/string.h"

void writeConsole(const char* src);
inline void writeConsole(cstring src) { writeConsole(src.c_str()); }

#endif