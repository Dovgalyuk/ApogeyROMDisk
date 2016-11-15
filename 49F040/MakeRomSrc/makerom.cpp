// ROM-���� ������ ��01 �� ������ ����������� ���
// (�) 6-12-2011 vinxru

#include "stdafx.h"
#include "vinlib/std.h"
#include "vinlib/file.h"
#include "vinlib/findfiles.h"
#include "vinlib/exec.h"
#include "vinlib/console.h"
#include "inifile.h"
#include "folders.h"
#include "romwriter.h"
#include "translateRk86.h"
#include <assert.h>

// �� ������� ���������������
void warning(cstring) {}
void fatal(Exception* e, const char* fn) { FatalAppExit(1, fn); }

//---------------------------------------------------------------------------
// ������ ������ � ��������                                                             

#pragma pack(push,1)
struct Map {
  char rombank;
  short romstart, ramstart;
};
#pragma pack(pop)

//---------------------------------------------------------------------------
// ��������� ��������

class MakeRomIni : public IniFile {
public:
  enum { clRed, clGreen, clBlue, clYellow, clMagenta, clCyan, colorCnt };

  std::vector<string> colors[colorCnt];
  int romsize;
  string input, output, loader0, menu, archivator;

  void setDefault() {
    romsize = 512;
    input = ".";
    output = "49lf040.rom";
    loader0 = "loader0.bin";
    menu = "menu.bin";
    archivator = "megalz.exe";
    for(int i=0; i<colorCnt; i++)
      colors[i].clear();
  }

  void loadParam(const char* n, const char* v) {
    #pragma warning(disable:4996)

    if(0==strcmpi(n,"red"       )) colors[clRed    ].push_back(v); else
    if(0==strcmpi(n,"green"     )) colors[clGreen  ].push_back(v); else
    if(0==strcmpi(n,"blue"      )) colors[clBlue   ].push_back(v); else
    if(0==strcmpi(n,"yellow"    )) colors[clYellow ].push_back(v); else
    if(0==strcmpi(n,"magenta"   )) colors[clMagenta].push_back(v); else
    if(0==strcmpi(n,"cyan"      )) colors[clCyan   ].push_back(v); else
    if(0==strcmpi(n,"romsize"   )) romsize=atoi(v); else
    if(0==strcmpi(n,"output"    )) output=v; else
    if(0==strcmpi(n,"input"     )) input=v; else
    if(0==strcmpi(n,"loader0"   )) loader0=v; else
    if(0==strcmpi(n,"archivator")) archivator=v; else
    if(0==strcmpi(n,"menu"      )) menu=v;
  }

  ~MakeRomIni() {}
};

//---------------------------------------------------------------------------
// ������������ ��� ����������� �����

const char* processColor(MakeRomIni& ini, cstring cname) {
  static const char* colorPrefix[6] = { "\x8C", "\x85", "\x89", "\x84", "\x88", "\x81" };
  assert(ini.colorCnt==6);

  for(uint j=0; j<ini.colorCnt; j++) {
    std::vector<string>& a = ini.colors[j];
    for(uint i=0; i<a.size(); i++)
      if(cname.find(a[i])!=-1)
        return colorPrefix[j];
  }

  return 0;
}

//---------------------------------------------------------------------------

int cmain(int,const char**) {
  writeConsole("�������� �������� ROM ����� ��� ������ ��01� �� ������ ����������� ���\r\n"
	           "4-12-2011 (c) vinxru, b2m, titus, esl");

  MakeRomIni ini;
  ini.load("makerom.ini");

  // ������� ������ ��������
  DeleteFile(ini.output.c_str());

  // �������� ������ ������
  Folders folders;
  folders.find(ini.input);

  if(folders.folders.size()==1 && folders.folders.front().items.size()==0) 
    raise("����� �� �������.");

  // ��������
  RomWriter rom;
  rom.alloc(ini.romsize*1024);

  // �������� ��������� �� ��� ��������
  std::vector<byte_t> data;
  loadFile(data, ini.loader0);
  rom.loader0size = data.size();
  if(data.size()>32768) raise("��������� �� ����� ���� ������ 32 ��!");
  for(uint i=32768; i+data.size()<=rom.buf.size(); i+=32768)
    rom.randomPut(i, data);

  // �������� ����
  loadFile(data, ini.menu);
  if(data.size()>32768) raise("��������� �� ����� ���� ������ 32 ��!");
  rom.put(data);

  for(std::list<Folders::Folder>::iterator f=folders.folders.begin(); f!=folders.folders.end(); f++) {
    Folders::Folder& folder = *f;

    uint mnl=0;
    for(uint i=0; i<folder.items.size(); i++) {
      Folders::FolderItem& it = folder.items[i];

      // ������ �����
      const char* prefix = processColor(ini, it.shortName);
      if(prefix==0) prefix = it.link!=-1 ? "\x81" : "\x80";

      // ����������� ��������� � ��86, ��������� ����, ������������ �����
      it.shortName = prefix+translateRk86(it.shortName);
      if(it.shortName.size()>60) it.shortName.resize(60);

      // ������ ������������ ����� �����
      if(mnl<it.shortName.size()) mnl=it.shortName.size();
    }
    mnl+=2;

    // ���������� ����� ����� � ������
    folder.intPtr = rom.ptr;

    // ��������� ���-�� ������ � �����
    int c = folder.items.size();
	if(c>=256) raise("� ����� �� ����� ���� ������ 255 ������ ("+folder.items[0].name+")");
    rom.put(&c, 1);  

    // ��������� ������ �������
    rom.put(&mnl, 1);  

    // ��������� ����� ������� ����
    int namesPtr = rom.ptr + 2 + 5*folder.items.size();
    rom.put(&namesPtr, 2);  

    // ����������, ��� ��� ����� ��������� �����
    folder.mapPtr = rom.ptr;

    // ��������� ����� ��� �����
    std::vector<Map> map;
    map.resize(folder.items.size());
    rom.put(&map, map.size()*sizeof(Map));  

    // ���������� ������������
    for(uint i=0; i<folder.items.size(); i++)
      rom.put(folder.items[i].shortName.c_str(), folder.items[i].shortName.size()+1);

    // ��������� ��� ������� ����� � ����� ������ ����
    rom.put("", 1);
  }

  // ��������� ������
  if(rom.ptr>=32768) raise("������ ���� �� ����� ��������� 32 ��");
  int s = (rom.ptr+255)/256;
  rom.randomPut(1, &s, 1);

  // �������� ����� � �������� � ���������� �����
  string err;
  for(std::list<Folders::Folder>::iterator f=folders.folders.begin(); f!=folders.folders.end(); f++) {
    Folders::Folder& folder = *f;

    std::vector<Map> map;
    map.resize(folder.items.size());

    for(uint i=0; i<folder.items.size(); i++) {
      cstring name = folder.items[i].name;

	  // ��� �����
      if(folder.items[i].link!=-1) {
        // ������������ �����
        map[i].rombank  = 0;
        map[i].romstart = folders.getFolder(folder.items[i].link).intPtr;
        map[i].ramstart = (short)0xF800; // ��� ������� �����
        continue;
      }

      // �������� �����
      data.resize(0);
      loadFile(data, name);

      // ������ � �������� �������. ������ ����� �������.
      int startAddr = (((int)(unsigned char)data[0])<<8) | ((int)(unsigned char)data[1]);
      int stopAddr  = (((int)(unsigned char)data[2])<<8) | ((int)(unsigned char)data[3]);
      uint len1 = stopAddr-startAddr+1;
      if(len1+4>data.size()) raise("������� �������� ���� "+name);
      if(len1>32768) raise("������� ������� ���� "+name);

      // ���������
      if(!folder.items[i].pak) { 
        saveFile("archivator.tmp", fcmCreateAlways, &data[4], len1);
        int r = exec(ini.archivator, "archivator.tmp \""+name+".pak\"");
        DeleteFile("archivator.tmp");
        if(r!=0) raise(ini.archivator+" archivator.tmp \""+name+".pak\" ������ "+i2s(r));  
      }

      // ������������ �����
      map[i].rombank  = rom.ptr>>15;
      map[i].romstart = rom.ptr&0x7FFF;
      map[i].ramstart = startAddr;

      // �������� ���� � ��������
      std::vector<char> packedData;
      loadFile(packedData, name+".pak");
      if(!rom.put_ne(&packedData[0], packedData.size()))
        err += "��� ����� ��� ����� "+name+"\r\n";
    }

    // ��������� �����
    if(map.size()>0) rom.randomPut(folder.mapPtr, &map[0], map.size()*sizeof(Map));
  }

  // ���� ����� �� ����� �� �����������, ������� ������ � �������
  if(!err.empty()) {
    MessageBox(0,err.c_str(),0,0);
    return 1;
  }
  
  // ��������� �������� 
  saveFile(ini.output, fcmCreateAlways, rom.buf);

  // ������� ���-�� ��������� ����
  MessageBox(0,("�������� �������� "+i2s(rom.buf.size()-rom.ptr)+" ����").c_str(),"",0);
  return 0;
}          