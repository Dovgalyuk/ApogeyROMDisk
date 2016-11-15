; ROM-���� ������ ��01 �� ������ 49LF040/49LF020/49LF010
; ����
; (�) 22-11-2011 vinxru
; ������������ ���������� sjasm

		device zxspectrum48
		org 100h

;----------------------------------------------------------------------------
; ��������� RKA �����. ��������������� ��� 4 ������ ��� �������� BIN �����

begin:
		;db entryPoint>>8
		;db entryPoint&0xFF
		;db crc>>8
		;db crc&0xFF

;----------------------------------------------------------------------------

entryPoint:	xor a
		ld (sel), a
		ld (scroll), a

		; ������� ������
		ld c, 1Fh
		call 0F809h

		; ���������
		ld hl, txt_start
		ld bc, 0
		call print

		; ����������� PCNT
		ld hl, (dirStart)
		ld a, (hl)
		ld (PCNT3+1), a
		ld (PCNT4+1), a

		inc hl
		ld a, (hl)
		ld (nameWidth1+1), a
		ld (nameWidth2+1), a
		ld (nameWidth3+1), a
		ld (nameWidth5+1), a

		; *** ����� ������������ �������� ***

		; ������
redraw:		ld a, (scroll)
		call findName

		ld c, 0
printLoop:	; ��������� ������� ������
		push bc

		; ���������� �� ������
		ld a, c
		call locate
		jp nc, mainLoopHB 

		;inc b
		;inc b
		call calcCoordsDE

nameWidth5:	ld b, 20

		; ����� �����
		ld a, (hl)
		or a
		jp z, printNo

		; ����� �����
		dec b
		inc hl
		ld (de),a
		inc de

		; ����� �������
		dec b
		xor a
		ld (de),a
		inc de

		; ����� �����
printL:		ld a, (hl)
		inc hl
		or a
		jp z, printSpc
		ld (de), a
		inc de
		dec b
		jp nz, printL
		jp printOk

		; ������� ����� �����
printSpc:       ld (de), a
		inc de
		dec b
		jp nz, printSpc

printOk:	; ����� �����
		ld a, 80h
		ld (de), a
		pop bc

		; ����
		inc c
		jp printLoop

printNo:	; ����� �����
		ld a, 80h
		ld (de),a
		inc de
		dec b
		; ����� ��������
		jp printSpc

		; *** ����������� ������� ***
mainLoopHB:	pop bc

mainLoop:	; ������ ������
		ld a, (sel)
		call locate
		call calcCoordsHL
		ld d, (hl)
		ld a, 0x91
		ld (hl), a

		; ���� ������� ������
		push hl
		push de
		call 0F803h ; readChar
		pop de
		pop hl

		; ������� ������
		ld (hl), d

		; ��������� ��������������� ���������
		cp 'A'
		jp z, showAuthor
		cp 19H
		jp z, up
		cp 1Ah
		jp z, down
		cp 08h
		jp z, left
		cp 18h
		jp z, right
		cp 0Dh
		jp z, load
		jp mainLoop

;----------------------------------------------------------------------------

showAuthor:	ld bc, 0
		ld hl, author
		call print
		jp mainLoop

;----------------------------------------------------------------------------

findName:       ld hl, (dirStart)
		inc hl
		inc hl
		ld e, (hl)
		inc hl
		ld d, (hl)
		ex de, hl

		or a
		ret z

		ld d, a

findName1:	ld a, (hl)
		inc hl
		or a
		jp nz, findName1
		dec d
		jp nz, findName1
		ret		

;----------------------------------------------------------------------------
; ���������� ������ � ������ �� �������� A

locate:         ; ��������� ���������� b=(a/22)*20, a%=22
		ld b, a
nameWidth1:	ld a, 10
		ld c, a
		ld a, b
locate0:	cp 22
		jp c, locate1
		ld b, a
		ld a, c
nameWidth2:	add 20
		cp 60	
		ret nc
		ld c, a
		ld a, b
		inc b
		add -22
		jp locate0		
locate1:	; ��������� ������ c=a+2
		add 2	
		ld b, c
		ld c, a
		ld a, b
nameWidth3:	sub 20
		ld b, a
		scf
		ret

;----------------------------------------------------------------------------
; ������ �������

up:             ld a, (sel)
		cp 0
		jp z, upScroll
		dec a
saveSel:	ld (sel), a		
		jp mainLoop

upScroll:	ld a, (scroll)
		or a
		jp z, mainLoop
		dec a
saveScroll:	ld (scroll), a
		jp redraw

;----------------------------------------------------------------------------

left:           ld a, (sel)
		cp 22
		jp c, leftScroll
		sub 22
		jp saveSel

leftScroll:	ld a, (scroll)
		or a
		jp z, saveSel
		sub 22
		jp nc, saveScroll
		xor a
		ld (sel), a
		jp saveScroll		

;----------------------------------------------------------------------------

right:          ; ��� ��������� �����?
		call getN
		inc a
PCNT4:		ld b, 100
		cp b
		jp z, mainLoop

		; ����� �� ��������� �� 22? 
		add 22-1
		cp b
		jp nc, rightMax
		
		; ���������� ������
		ld a, (sel)
		add 22
right2:		ld (sel), a
		
		; ������ �� ����� �������?
		call locate
		jp c, mainLoop

		; ������� ������
		ld hl, sel
		ld a, (hl)
		sub 22
		ld (hl), a
		ld a, (scroll)
		add 22
		jp saveScroll

		; ������������� ������ �� ��������� �����
rightMax:	ld hl, scroll
		ld a, b
		dec a
		sub (hl)
		jp right2

;----------------------------------------------------------------------------

down:		call getN
		inc a
PCNT3:		cp 2
		jp z, mainLoop
		ld a, (sel)
		inc a
		call locate
		jp nc, downScroll
		ld a, (sel)
		inc a
		jp saveSel
downScroll:	ld a, (scroll)
		inc a
		jp saveScroll

;----------------------------------------------------------------------------

getN:		ld a, (scroll)
		ld b, a
		ld a, (sel)
		add b
		ret

;----------------------------------------------------------------------------
; ������������ ����� ENTER. �������� �� ��� � ������ ���������/����� � �������

load:           ; ������ ������ HL = map + 2 + 5 * sel
		call getN
		ld l, a
		ld h, 0
		ld bc, hl
		add hl, hl
		add hl, hl
		add hl, bc
		ex hl, de
		ld hl, (dirStart)
		add hl, de
		ld bc, 4+4
		add hl, bc
		   
		ld a, (hl)
		dec hl
		cp 0F8h
		jp z, directory

		push hl

		; ���� �����
		ld hl, txt
		call 0F818h

		; ����� �����
		ld a, (sel)
		ld b, a
		ld a, (scroll)
		add b
		call findName

		; ����� �����
		inc hl
		call 0F818h

		; ����������� ������������
		ld de, 0E000h-(UnmlzEnd-UnmlzStart)
		ld hl, UnmlzStart
		ld b, (UnmlzEnd-UnmlzStart)>>8
		ld c, (UnmlzEnd-UnmlzStart)&0xFF
load_1:		ld a, (hl)
		ld (de), a
		inc hl
		inc de
		dec bc
		ld a, b
		or a, c
		jp nz, load_1

		pop hl
		dec hl
		dec hl
		dec hl

		;ld bc, 0F875h
		;push bc

		; ������������
		ld a, (hl) ; ���� ���
		inc hl
		ld e, (hl) ; ����� ���
		inc hl 
		ld d, (hl)
		inc hl 
		ld c, (hl) ; ����� ���
		inc hl 
		ld b, (hl)
		push bc
		jp 0E000h-(UnmlzEnd-UnmlzStart)

;----------------------------------------------------------------------------

directory:      dec hl
		ld d, (hl)
		dec hl
		ld e, (hl)
		ex hl, de
		ld (dirStart), hl

		ld a, (sel)
		ld hl, 4000h 
		ld (hl), a
		jp entryPoint
		
;----------------------------------------------------------------------------

calcCoordsDE:   push hl
		call calcCoordsHL
		ex hl, de
		pop hl
		ret

calcCoordsHL:	ld hl, 0E1D0h + 78*3 + 8
		ld de, 78
		inc c
print1:		add hl, de
		dec c
		jp nz, print1
		ld e, b
		add hl, de
		ret

;----------------------------------------------------------------------------

print:          call calcCoordsDE
print2:		ld a, (hl)
		inc hl
		or a
		ret z
		ld (de), a
		inc de
		jp print2

;----------------------------------------------------------------------------
; ������ ����������

txt_start:      db 0x84,'apogej bk01c ROM disk',0x80,0
txt		db 01Fh, 01Bh, 059h, 038h, 020h, 0
sel:		db 0
dirStart:	dw rootDir
scroll:		db 0
author:         db 0x84,'ROMDISK (C) 4-12-2011 VINXRU, B2M, TITUS, ESL',0x80,0

UnmlzStart:	incbin "unmlz.bin"
UnmlzEnd:

rootDir:
		; ��� RKA �����  include "dir.inc"
		; ��� ����� ������������� ��������� �������� ��������
		
;----------------------------------------------------------------------------

crc:            dw 0

end:
;		savebin "c:\loader.rka",begin,end
		savebin "menu.bin",begin,crc-begin
