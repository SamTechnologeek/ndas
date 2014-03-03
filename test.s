JSR setup

; main setup
:setup
	HWN I             ; Store number of connected devices in I
	JSR map_hardware  ; Setup the hardware
	JSR print_message ; Print our message
	SET PC, end       ; End the program

; setup the hardware
; iterate over all connected devices
:map_hardware
	SUB I, 0x1               ; Subtract 1 from I each time the loop runs
	HWQ I                    ; Harware query stores first part of the HW ID in
	                         ; B and the second part in A
	IFE B, 0x7349            ; Check for LEM1802 (ID=7349F615)
		IFE A, 0xF615
			;IFE C, 0x1802    ; Verify version
			JSR setup_screen ; Setup the screen
	IFE I, 0x0               ; If I is 0, all devices have been checked/setup
		SET PC, POP          ; Return
	SET PC, map_hardware     ; Otherwise loop

; setup the LEM1802
:setup_screen
	SET [screen], I ; Store screen ID at address of screen
	SET A, 0        ; MEM_MAP_SCREEN map video memory to address starting at B
	SET B, 0x8000   ; video memory starting address
	HWI [screen]    ; Send interrupt to device
	SET PC, POP     ; return

:print_message
	SET PUSH, I   ; I will be our counter
	SET I, 0x0    ; Set it to 0
	:print_message_loop
		SET J, [msg+I]                 ; Store current character in J
		IFE J, 0                       ; Check if J is 0
			SET PC, print_message_end  ; End the loop
		BOR J, 0xA000                  ; Set color to light green
		SET [0x8000+I], J              ; Print the character
		ADD I, 0x1                     ; Add 1 to the counter
		SET PC, print_message_loop     ; Loop
	:print_message_end
	SET I, POP  ; POP I
	SET PC, POP ; Return

:end
	SUB PC, end

; message we want to print
:msg 
	dat "Hello, World!", 0x0
	dat "FICK JA!", 0x0

:screen 
dat 0x0
