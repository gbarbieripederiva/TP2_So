GLOBAL cpuVendor

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret



;--------------------------------Sounds----------------------------------------------------
GLOBAL turnOffBeep
GLOBAL turnOnBeep
turnOnBeep:
	push ax
	mov     al, 182         ; Prepare the speaker for the
	out     43h, al         ;  note.
							; this is done by putting 182 on port 43h
	mov ax,di				; same but get parameter from di
    ;mov     ax, 4560       ; Frequency number (in decimal)
	out     42h, al         ; Output low byte.
	mov     al, ah          ; Output high byte.
	out     42h, al 
	in      al, 61h         ; Turn on note (get value from
							;  port 61h).
	or      al, 00000011b   ; Set bits 1 and 0.
	out     61h, al         ; Send new value.
	pop ax
	ret
turnOffBeep:	
	push ax
	in      al, 61h         ; Turn off note (get value from
							;  port 61h).
	and     al, 11111100b   ; Reset bits 1 and 0.
	out     61h, al         ; Send new value.
 	pop ax
	ret
;Nota sobre la frecuencia
; code from http://muruganad.com/8086/8086-assembly-language-program-to-play-sound-using-pc-speaker.html
; Note 	Frequency 	Frequency #
; C 	130.81 	9121
; C# 	138.59 	8609
; D 	146.83 	8126
; D# 	155.56 	7670
; E 	164.81 	7239
; F 	174.61 	6833
; F# 	185.00 	6449
; G 	196.00 	6087
; G# 	207.65 	5746
; A 	220.00 	5423
; A# 	233.08 	5119
; B 	246.94 	4831
; Middle C 	261.63 	4560
; C# 	277.18 	4304
; D 	293.66 	4063
; D# 	311.13 	3834
; E 	329.63 	3619
; F 	349.23 	3416
; F# 	369.99 	3224
; G 	391.00 	3043
; G# 	415.30 	2873
; A 	440.00 	2711
; A# 	466.16 	2559
; B 	493.88 	2415
; C 	523.25 	2280
; C# 	554.37 	2152
; D 	587.33 	2031
; D# 	622.25 	1917
; E 	659.26 	1809
; F 	698.46 	1715
; F# 	739.99 	1612
; G 	783.99 	1521
; G# 	830.61 	1436
; A 	880.00 	1355
; A# 	923.33 	1292
; B 	987.77 	1207
; C 	1046.50



;Function to access the clock
GLOBAL readCMOS
readCMOS:
	cli
	xor rax,rax
	mov al,dil
	out 70h,al
	in al,71h
	sti
	ret

GLOBAL rebootASM
rebootASM:
	;send reboot to cpu via 8042 controller
	mov al,0xf0
	out 64h,al
	;if it fails then halt machine
	cli
infiniteLoop:
	hlt
	jmp infiniteLoop
	ret

