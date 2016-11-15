// ��������, ����������, ASIS ������ ���������� VinLib. � �������� ���������
// (�) 5-12-2011 vinxru

#include <stdafx.h>
#include "vinlib/findfiles.h"
#include "vinlib/types.h"
#include <windows.h>

class FindHandle {
public:
  HANDLE handle;

  inline FindHandle() { 
    handle = INVALID_HANDLE_VALUE;
  }

  // �������� ������ ����
  bool findFirst(const char* mask, WIN32_FIND_DATA& fd);

  // �������� ��������� ����
  bool findNext(WIN32_FIND_DATA& fd);

  // ������� ����� ������ (������������� ���������� � findFirst � �����������)
  void close();

  inline ~FindHandle() { 
    close();
  }
};

bool FindHandle::findFirst(const char* mask, WIN32_FIND_DATA& fd) {
  close();
  handle = FindFirstFile(mask, &fd);
  return handle!=INVALID_HANDLE_VALUE;
}

bool FindHandle::findNext(WIN32_FIND_DATA& fd) {
  if(handle==INVALID_HANDLE_VALUE) return false;
  return FindNextFile(handle, &fd)!=0;
}

void FindHandle::close() {
  if(handle!=INVALID_HANDLE_VALUE)
    FindClose(handle);
  handle=INVALID_HANDLE_VALUE;
}

void findFiles(std::vector<FindData>& out, const char* mask) {
  WIN32_FIND_DATA fd;
  FindHandle h;
  if(h.findFirst(mask, fd))
    do {
      if(0!=strcmp(fd.cFileName,".") && 0!=strcmp(fd.cFileName,"..")) {
        FindData& x = add(out);
        x.name         = fd.cFileName;
        x.attrib       = fd.dwFileAttributes;
        x.accessTime   = *(__int64*)&fd.ftLastAccessTime;
        x.writeTime    = *(__int64*)&fd.ftLastWriteTime;
        x.creationTime = *(__int64*)&fd.ftCreationTime;
        x.size         = ((unsigned __int64)fd.nFileSizeHigh<<32) | (unsigned __int64)fd.nFileSizeLow;
        if(x.size<0) x.size=0x7FFFFFFFFFFFFFFF; //!! ����������� ���������
      }
    } while(h.findNext(fd));
};
