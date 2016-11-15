// ��������, ����������, ASIS ������ ���������� VinLib. � �������� ���������
// (�) 5-12-2011 vinxru

#ifndef VINLIB_EXCEPTION_H
#define VINLIB_EXCEPTION_H

#include <vinlib/string.h>

enum ExceptionClass { ecSilentException,ecException,ecUnknown,ecBreakException,ecLangException }; // ecAlt,

// ����������� ����� ���������� ������, � �� �� ������������ UNICODE.

class Exception {
protected:
  struct Exception1 {
    int len;           
    ExceptionClass cls;  // ����� ������
    char_t* what;        // ����� ������
    char_t* module;      // ������
    int line;            // ������ � ������
  };
  Exception1* e; // ���� ��� �� ���������� �������� ��� ����� ������ ������, ����� �������������, � ������� ������ ����������, ������� ���� ����. ��� ����� � �������� ���������.
public:
  Exception(const char_t* what=_T("����������� ������"), const char_t* module=_T(""), int line=0, ExceptionClass cls=ecException) throw();
  Exception(const Exception& src) throw();  
  void operator = (const Exception& src) throw();
  const char_t* what() const throw();
  const char_t* module() const throw();
  int line() const  throw();
  ExceptionClass cls() const  throw(); 
  void raise() const; // ������������ ����� ���������� ����������
  ~Exception() throw();
};

// ������� ���������� ������������ � ���������� ���������� ���������
#define BEGIN_PROCESS_EXCEPTION try {
#define END_PROCESS_EXCEPTION } catch(Exception& e) { ::error(&e,0); } catch(...) { ::error(0,0); }
#define END_PROCESS_EXCEPTION_ADDR(A) } catch(Exception& e) { ::error(&e,A); } catch(...) { ::error(0,A); }

// ����, � ������� �� ������ ���� ����������
#define BEGIN_NO_EXCEPTION try {
#define END_NO_EXCEPTION } catch(Exception& e) { fatal(&e,0); } catch(...) { fatal(0,0); }
#define END_NO_EXCEPTION_ADDR(A) } catch(Exception& e) { fatal(&e,_T(A)); } catch(...) { fatal(0,_T(A)); }

// �������� � ����� ���������� ������
#define BEGIN_EXT_EXCEPTION try {
#define END_EXT_EXCEPTION(text) \
  } catch(Exception& e) { \
    string tmp; /*! �������� �� ����������� �����*/ \
    cat(tmp, text, _T("\n"), e.what()); \
    raise(tmp.c_str(), e.module(), e.line(), e.cls()); \
    throw Exception(); \
  } catch(...) { \
    raise((string)text + _T("\n����������� ������"));  /*! �������� �� ����������� �����*/ \
    throw Exception(); \
  }

// ������ ������� ������ Break. ����� ���������� � ����������� ������ ���������� ���� except, ��� 
// �� ������������� ����������� ����.

class BreakException : public Exception {
public:
  inline BreakException(const char_t* what, const char_t* module=_T(""), int l=0) :  Exception(what,module,l,ecBreakException) {}
};

class LangException : public Exception {
public:
  inline LangException(const char_t* what, const char_t* module=_T(""), int l=0) :  Exception(what,module,l,ecLangException) {}
};

class SilentException : public Exception {
public:
  inline SilentException(const char_t* what, const char_t* module=_T(""), int l=0) :  Exception(what,module,l,ecSilentException) {}
};

// ��������� ����������
inline void raise(const char_t* str) { Exception e(str); throw e; }
inline void raise(cstring str) { raise(str.c_str()); }

// ��� ������� ������ ���� ������� � ���������
void warning(cstring errorText);

// ��� ������� ������ ���� ������� � ���������
void fatal(Exception* e, const char* fn);

#endif