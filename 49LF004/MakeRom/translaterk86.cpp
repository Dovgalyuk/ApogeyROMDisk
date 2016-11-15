// ROM-���� ������ ��01 �� ������ 49LF040/49LF020/49LF010
// (�) 5-12-2011 vinxru

#include <stdafx.h>
#include "translateRk86.h"
#include "vinlib/string.h"
#include "vinlib/exception.h"

char translateRk86c(char c) {
  switch((char)upperCaseTbl[(unsigned char)c]) {
    case ' ': return 0x20;
    case '!': return 0x21;
    case '"': return 0x22;
    case '#': return 0x23;
    case '$': return 0x24;
    case '%': return 0x25;
    case '&': return 0x26;
    case '\'':return 0x27;
    case '(': return 0x28;
    case ')': return 0x29;
    case '*': return 0x2A;
    case '+': return 0x2B;
    case ',': return 0x2C;
    case '-': return 0x2D;
    case '.': return 0x2E;
    case '/': return 0x2F;
    case '~': return 0x2F;
    case '0': return 0x30;
    case '1': return 0x31;
    case '2': return 0x32;
    case '3': return 0x33;
    case '4': return 0x34;
    case '5': return 0x35;
    case '6': return 0x36;
    case '7': return 0x37;
    case '8': return 0x38;
    case '9': return 0x39;
    case ':': return 0x3A;
    case ';': return 0x3B;
    case '<': return 0x3C;
    case '=': return 0x3D;
    case '>': return 0x3E;
    case '?': return 0x3F;
    case '@': return 0x40;
    case 'A': return 0x41;
    case 'B': return 0x42;
    case 'C': return 0x43;
    case 'D': return 0x44;
    case 'E': return 0x45;
    case 'F': return 0x46;
    case 'G': return 0x47;
    case 'H': return 0x48;
    case 'I': return 0x49;
    case 'J': return 0x4A;
    case 'K': return 0x4B;
    case 'L': return 0x4C;
    case 'M': return 0x4D;
    case 'N': return 0x4E;
    case 'O': return 0x4F;
    case 'P': return 0x50;
    case 'Q': return 0x51;
    case 'R': return 0x52;
    case 'S': return 0x53;
    case 'T': return 0x54;
    case 'U': return 0x55;
    case 'V': return 0x56;
    case 'W': return 0x57;
    case 'X': return 0x58;
    case 'Y': return 0x59;
    case 'Z': return 0x5A;
    case '[': return 0x5B;
    case '\\':return 0x5C;
    case ']': return 0x5D;
    case '^': return 0x5E;
    case '_': return 0x5F;
    case '�': return 0x60;
    case '�': return 0x61;
    case '�': return 0x62;
    case '�': return 0x63;
    case '�': return 0x64;
    case '�': return 0x65;
    case '�': return 0x66;
    case '�': return 0x67;
    case '�': return 0x68;
    case '�': return 0x69;
    case '�': return 0x6A;
    case '�': return 0x6B;
    case '�': return 0x6C;
    case '�': return 0x6D;
    case '�': return 0x6E;
    case '�': return 0x6F;
    case '�': return 0x70;
    case '�': return 0x71;
    case '�': return 0x72;
    case '�': return 0x73;
    case '�': return 0x74;
    case '�': return 0x75;
    case '�': return 0x76;
    case '�': return 0x77;
    case '�': return 0x78;
    case '�': return 0x79;
    case '�': return 0x7A;
    case '�': return 0x7B;
    case '�': return 0x7C;
    case '�': return 0x7D;
    case '�': return 0x7E;
  }
  return 0;
}

string translateRk86(cstring in) {
  string out;
  out.resize(in.size());
  const char* inp = in.c_str();
  char* o = (char*)out.c_str();
  while(*inp) {
    char c = translateRk86c(*inp++);
    if(c==0) raise("���� "+in+" �������� ���������������� ��86 ������.");
    *o++ = c;
  }
  return out;
}
