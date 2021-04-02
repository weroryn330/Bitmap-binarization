;------ KOD --------;
.code

Main proc buf: QWORD, part: QWORD, start: SDWORD, finish: SDWORD, intensity: SDWORD

	push r13		; iterator iZero
	push r10		; intensity
	push r9			; finish
	push r8			; start
	push rcx		; bufor
	push rdx		; part

	xor r13, r13	; zerowanie iZero

	mov r10, rax	; przeniesienie intensity do r10
	imul r10, 3		; wartosc intensity przemnozona *3 
	

;------ PETLA FOR --------;
	forLoop:

		;------- warunek konca pracy asm --------;
		cmp r8, r9		; porownanie start z finish
		jge endLoop		; jestli start >= finish, to skocz do endLoop

		;------- warunek if (((buf[i] + buf[i + 1] + buf[i + 2])) > intensity) --------;
		; XMM:
		pmovzxbd xmm0, [rcx + r8]	; cztery wartosci do xmm0
		
		shufps xmm1, xmm0, 93h		; przesuniecie w lewo o jeden (druga skladowa)
		pshufd xmm2, xmm0, 4eh		; przesuniecie w lewo o dwa (trzecia skladowa)

		addps xmm0, xmm1			; dodanie dwoch skladowych
		addps xmm0, xmm2			; dodanie trzeciej skladowej

		shufps xmm0, xmm0, 39h		; przesuniecie w prawo
		shufps xmm0, xmm0, 39h		; przesuniecie w prawo

		vcvtdq2ps xmm3, xmm0		; konwersja
		cvttss2si eax, xmm3			; przemieszczenie gotowej liczby do rax

		add r8, 3				; iterator += 3

		;------- porownanie jasnosci piksela z zadanym progiem --------;
		cmp r10, rax
		jge elseCondition ; jesli intensity >= R + G + B, to skocz do else

		; part[iZero] = 255;
		mov byte ptr [rdx + r13], 0ffh
		; part[iZero + 1] = 255;
		mov byte ptr [rdx + r13 + 1], 0ffh
		; part[iZero + 2] = 255;
		mov byte ptr [rdx + r13 + 2], 0ffh

		; iZero += 3;
		add r13, 3

		;------- koniec ifa - powrot do petli for --------;
		jmp forLoop


		;------- warunek else --------;
		elseCondition:
			; part[iZero] = 255;
			mov byte ptr [rdx + r13], 00h
			; part[iZero + 1] = 255;
			mov byte ptr [rdx + r13 + 1], 00h
			; part[iZero + 2] = 255;
			mov byte ptr [rdx + r13 + 2], 00h
			; iZero += 3;
			add r13, 3
			
			;------- koniec else - powrot do petli for --------;
			jmp forLoop



;------- petla zamykajaca kod --------;
endLoop:
	pop r13
	pop r10
	pop r9
	pop r8
	pop rcx
	pop rdx
	ret
	Main endp
    end