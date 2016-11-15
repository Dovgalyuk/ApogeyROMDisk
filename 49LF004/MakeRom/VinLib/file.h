// ��������, ����������, ASIS ������ ���������� VinLib. � �������� ���������
// (�) 5-12-2011 vinxru

#ifndef VINLIB_FILE_H
#define VINLIB_FILE_H

#include <windows.h>
#include "vinlib/string.h"

// ������ �������� �����
enum FileCreateMode {
  fcmCreateNew     = 1, // ���� ���� ��� ����������, �� ���������� ������.
  fcmCreateAlways  = 2, // ���� ���� ��� ����������, �� �� ����� ������.
  fcmOpenExisting  = 3, // ���� ���� �� ����������, �� ���������� ������. ������������ ���� ����� ������.
};

class File {
public:
  HANDLE handle;

  File();
  ~File();

  // ����������, ���� ���� �� ������
  void raiseIfNotOpened();

  // ������� ����
  //   access = ����� ���������� GENERIC_READ, GENERIC_WRITE
  //   share  = ����� ���������� FILE_SHARE_DELETE, FILE_SHARE_READ, FILE_SHARE_WRITE	
  //   distr  = ���� �� CREATE_NEW, CREATE_ALWAYS, OPEN_EXISTING, OPEN_ALWAYS, TRUNCATE_EXISTING
  void open(const char* fileName, int access, int share, int distr);

  // ������� ���� ��� ������
  void openR(const char* fileName); 

  // ������� ���� ��� ������/������
  void openW(const char* fileName);

  // ������� ����� ����
  void openC(const char* fileName, FileCreateMode mode=fcmCreateAlways);

  // ������ �� �����
  void read(void* buf, int len);

  // �������� � ����
  void write(const void* buf, int len);

  // ������ ������ �����
  __int64 size64();

  // ������ ������ �����. ���� ������ ������ 2^31 ����, �� ���������� ������.
  int size32();

  // ��������� ����� �����
  bool eof();

  // ���������� ������ �����
  void setSize(__int64 size);

  // ���������� ������� ������/������ �����
  void setPosition(__int64 pos);

  // �������� ������� ������/������ �����
  __int64 getPosition64();

  // �������� ������� ������/������ �����. ���� ������� ������ 2^31 ����, �� ���������� ������.
  int getPosition32();

  // ������� ����
  void close();

  // ���� ������?
  bool opened();

  // ��������� ���� ������ std::string
  inline void open(cstring fileName, int access, int share, int distr) { open(fileName.c_str(), access, share, distr); }
  inline void openR(cstring fileName) { openR(fileName.c_str()); }
  inline void openC(cstring fileName, FileCreateMode createAlways=fcmCreateAlways) { openC(fileName.c_str(), createAlways); }
  inline void openW(cstring fileName) { openW(fileName.c_str()); }
};

// ��������� ������� ������ � ���� (� ��������� ���� ������ std::string)
void saveFile(const char* fileName, FileCreateMode mode, const void* buf, int len);
inline void saveFile(cstring     fileName, FileCreateMode mode, const void* d, int l    ) { saveFile(fileName.c_str(), mode, d, l);                }

// ��������� ������ � ���� (� ��������� ���� ������ std::string)
inline void saveFile(const char* fileName, FileCreateMode mode, const std::vector<char>& d) { saveFile(fileName,         mode, &d[0], d.size()); }
inline void saveFile(cstring     fileName, FileCreateMode mode, const std::vector<char>& d) { saveFile(fileName.c_str(), mode, &d[0], d.size()); }

// ��������� ������ � ����
inline void saveFile(const char* fileName, FileCreateMode mode, cstring     d) { saveFile(fileName,         mode, d.c_str(), d.size()); }
inline void saveFile(cstring     fileName, FileCreateMode mode, cstring     d) { saveFile(fileName.c_str(), mode, d.c_str(), d.size()); }
inline void saveFile(const char* fileName, FileCreateMode mode, const char* d) { saveFile(fileName,         mode, d, strlen(d));        } 
inline void saveFile(cstring     fileName, FileCreateMode mode, const char* d) { saveFile(fileName.c_str(), mode, d, strlen(d));        }

// ��������� ���� � ������
void loadFile(std::vector<char>& out, cstring fileName);

// ��������� ���� � ������
void loadStringFromFile(std::string& out, const std::string& fileName);

// ��������� ������ �� �����.
// ����� ��������� (��� ��� ������ ����������� � ����������), �� ������� �������.
inline std::string loadStringFromFile(cstring fileName) {
  std::string str;
  loadStringFromFile(str, fileName);
  return str;
}

#endif