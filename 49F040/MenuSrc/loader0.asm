; ��������� ��� ������ �������� �� ��������
; (�) 17-11-2011 vinxru
; ������������ ���������� sjasm

		device zxspectrum48
		org 0h

;----------------------------------------------------------------------------

entryPoint:     ld d, 1 ; ���������� ���������
		ld h, 0 ; ����� ��� = 0
		ld l, h
		ld e, h
		ld b, h
		ld c, h
		call 0FA68h ; ������� �������� �� ���

		; ����� �������� ����� ���
		ld hl, 0EE01h 
		xor a
		ld (hl), a
		inc hl ; hl = 0EE02h 
		ld (hl), a
		ld (hl), 80h
		ld (hl), a

		ld h, a ; ����� ��� = 0
		ld l, a
		ld b, a
		ld c, a
		push bc
		jp 0FA68h ; ������� �������� �� ���

end:
		savebin "loader0.bin",entryPoint,end
