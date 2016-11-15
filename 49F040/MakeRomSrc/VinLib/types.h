// ��������, ����������, ASIS ������ ���������� VinLib. � �������� ���������
// (�) 5-12-2011 vinxru

#ifndef VINLIB_TYPES_H
#define VINLIB_TYPES_H

#include <vector>
#include <list>

typedef char byte_t;
typedef unsigned int uint;

// �������� ������� � ����� ������� � ������� ������ �� ����
template<class T>
inline T& add(std::vector<T>& a) { int n=a.size(); a.resize(n+1); return a[n]; }

// �������� ������� � ����� ������ � ������� ������ �� ����
template<class T>
inline T& add(std::list<T>& a) { a.push_back(T()); return a.back(); }

#endif