; ROM-���� ������ ��01 �� ������ 49LF040/49LF020/49LF010
; ��������� ���������
; (�) 22-11-2011 vinxru
; ������������ ���������� sjasm

		device zxspectrum48
		org 0h

;----------------------------------------------------------------------------


entryPoint:     ; ����� ��� (��������� ���������� �� 100h, � � ������ ����� ������� ���-�� ������� ��� ��������)
		ld bc, 0FFh

		; ������������� �����
		ld hl, 0EE03h		
		ld (hl), 90h 

		; R/C=1
		dec hl
		dec hl
		ld (hl), 1

		; ������� ���� � (R/C=0)
loop1:		ld de, 0F000h

loop:		; ������� �������: bank=0, de=0; bank=2; de=2; bank=4; de=4;
bank2:		ld a, 0 ; ��� ��������� ������� ���� *2
		cp e
		jp z, ignorebyte2			
noignorebyte:
		; ����
		inc hl
		ld (hl), d  ; hl=0EE02h
		dec hl
		ld (hl), e  ; hl=0EE01h (R/C=0)
		
		; ������
		inc hl
		ld (hl), 0 ; hl=0EE02h
		dec hl
bank:		ld (hl), 1 ; hl=0EE01h ; ��� ��������� ������� ���� *2+1 (R/C=1)

		; ����������� ������
		ld a, (0EE00h)
		ld (bc), a
		inc bc	
ignorebyte:     ; ����
		inc de
		inc de
		ld a, d
		or e
		jp nz, loop

		; � ����� ������ ����������� ����� �������, ������� ���� ��������� �������
		push bc
		ld a, (0FFh)
		ld b, a
		ld a, (bank+1)
		cp b
		jp z, 100h
		pop bc

		; ��������� ����
		ld a, (bank+1)
		inc a
		ld (bank2+1), a
		inc a
		ld (bank+1), a
		jp loop1

ignorebyte2:    ld a, 0F0h
		cp d
		jp z, ignorebyte
		jp noignorebyte

;----------------------------------------------------------------------------

		; ������ ����� 87 ����. � ��� �� ��� �������� ������������� ������� ����.
end1		DUP 87-(end1-entryPoint)
		db 0
		EDUP
end:                
		savebin "loader1.bin",entryPoint,end-entryPoint
