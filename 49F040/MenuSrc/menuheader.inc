		org 0h
begin:
		ld d, (end+255)/256 ; ���������� ���������
		ld h, 0 ; ����� ��� = 0
		ld l, h
		ld e, h
		ld b, h
		ld c, h
		call 0FA68h ; ������� �������� �� ���
