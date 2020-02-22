	;; test comment
	.INCLUDE 	"other_prog.s"
	

	.SEGMENT	"zero_page"
	.ORIGIN		$0000
	.const_dec	= #10
	.const_hex	= $10
	.const_bin	= %10

	
	.SEGMENT	"MY_PROG"
	.ORIGIN		$F000
start:
	cld				;this little flag caused me undue pain and suffering
	;; 
	;; clear zero-page ram
	;; 
	lda	#0			;set value to #0
	ldx	#$FF			;point x at $FF
	txs				;set the stack pointer to $FF
loop_clear_ram:				;will clear ram from $FF to $00
 	sta 	0,x 			;store #0 at $x
        dex 				;pointer--
        bne 	loop_clear_ram		;loop if x != 0
	lda	#0			;
	ldx	#0			;
	ldy	#0			;set all registers to #0


	.ALIGN 	256			;align ROM graphics data on page boundry
gfx_player_sprite_base_addr:
	.byte	%01010000
        .byte	%01010000
        .byte	%00100000
        .byte	%10101000
        .byte	%01110000
        .byte	%00100000
        .byte	%01110000
        .byte	%01110000
gfx_player_sprite_frame1:	
        .byte	%01100000
        .byte	%01100000
        .byte	%00100000
        .byte	%10110000
        .byte	%01110000
        .byte	%00100000
        .byte	%01110000
        .byte	%01110000


data:
	.byte	31	;1
	.byte	13
	.byte	26
	.byte	6
	.byte	0  	;2
	.byte	0
	.byte	24
	.byte	6	
	.byte	28	;3
	.byte	13
	.byte	26
	.byte	6
	.byte	0	;4
	.byte	0
	.byte	24
	.byte	6

	
	.SEGMENT	"interrupt_vectors" 	;comment
	.ORIGIN		$FFFA		    	;comment with more than one space
	.WORD		start		   	;
	.WORD		start			;^that comment doesn't have anything
	.WORD		start			;$1234
	;; 1231
	;; !@#$%^&*()_{}:">?<,./;'[]-=+_`~\|
