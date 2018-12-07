#     Minesweeper
#
#  Your Name:	haozheng luo
#  Date:Oct 7th 2018
.data
maps:    .word   0:99
A:       .word   0:7
# your data allocation/initialization goes here

.text

	   	MineSweep: swi   567	   	   # Bury mines (returns # buried in $1)
start:     	addi $5, $0, 0   #total
	   	addi $6, $0, -1   #Flag
	   	addi $7, $0, 0   #zero
           	addi $8, $0, -2   #unknown
           	addi $10,$0, -1    #tempi
	   	addi $21,$0,9
           	addi $22,$0,7
           	addi $15,$0,63
Loop1:     	beq  $10,$21,Loop3
           	addi $10,$10, 1
	   	addi $11,$0, -1      #tempj
Loop2:     	beq  $11,$21,Loop1
           	addi $11, $11, 1
           	addi $26,$0,10
           	mult $10,$26
           	mflo $13
           	add  $13,$13,$11
           	sll  $13,$13,2           	
	   	beq  $10,$0, case1
           	beq  $10,$21,case1
           	beq  $11,$0, case1
           	beq  $11,$21, case1
	   	sw   $8,maps($13)
	   	j    Loop2
case1:     	sw   $7,maps($13)
           	j    Loop2	   
Loop3:		beq  $5,$15,End
		addi $10,$0, -1    #tempi
Loop4:        	beq  $10,$22,Loop3
               	addi $10, $10,1
	       	addi $11,$0, -1     #tempj
Loop5:         	beq  $11,$22,Loop4
               	addi $11, $11,1
test:           addi $1,$0, -1     #i
Loop6:          beq  $1,$22,c10
               	addi $1, $1,1
	       	addi $9,$0, -1     #j
Loop7:          beq  $9,$22,Loop6
		addi $9, $9,1
openOrFlag:   	addi $1,$0,0
	      	addi $13,$1,1
                sll  $13,$13,3
		add  $13,$13,$9
                addi $13,$13,1
                sll  $13,$13,2
                lw   $12,maps($13)
		addi $17,$0,0
                addi $29,$0,1
		addi $14,$0,0
		addi $14,$0,100
		sll  $14,$14,2
                addi $25,$0,0
                addi $27,$0,3     #flag
Loop8:          beq  $25,$27,r1
		addi $25,$25,1
		addi $28,$0,0		
Loop9:          beq  $28,$27,Loop8
		beq  $25,$29,case2
t2:		sll  $13,$25,3
		add  $13,$13,$9
	        sll  $13,$13,2
      		lw   $17,maps($13)
		sw   $17,A($14)
		addi $14,$14,4
		addi $28,$28,1
		j    Loop9
case2:          bne  $28,$27,t2
		beq  $28,$27,Loop9
r1:             addi $28,$0,0
                addi $24,$0,0     #flag
                addi $13,$0,0      #unknown
Loop10:         beq  $28,$22,r2
		sll  $29,$28,2
		lw   $25,A($29)
                beq  $25,$6,c1
                beq  $25,$8,c2
		addi $28,$28,1 
		j    Loop10
c1:             addi $24,$24,1
                addi $28,$28,1 
		j    Loop10
c2:             addi $13,$13,1
                addi $28,$28,1 
		j    Loop10
r2:             addi  $14, $0, 0
		addi  $16, $0, 0
		beq  $24, $12, c3
                add  $24,$24, $13
                beq  $24, $12, c4    #flag
		j    Loop7

c3:             beq  $14, $22,Loop7
		sll  $16, $14,2
		lw   $16, A($16)
row:       	addi $18,$0,0
	   	slti $19, $14,3
           	beq   $19,$0, case8
	   	addi  $18,$18,-1
		j     c12
case8:     	addi  $18,$18,1
		j     c12
col:       	addi  $20,$0, 0
           	addi  $23,$0,3
           	addi  $24,$0,5
	   	beq   $14, $0, case3
           	beq   $14, $23, case3
           	beq   $14, $24, case3
           	addi  $23, $0, 2
           	addi  $24, $0, 4
           	beq   $14, $23, case4
           	beq   $14, $24, case4
           	beq   $14, $22, case4   
		j     c12 
case3:     	addi  $20,$20,-1
		j     c12
case4:     	addi  $20,$20,1   
                j     c12
c12:		beq  $16, $8, c5
		addi $14,$14,1
		j    c3
c4:             beq  $14, $22,Loop7
		sll  $16, $14,2
		lw   $16, A($16)       #flag
row1:       	addi $18,$0,0
	   	slti $19, $14,3
           	beq   $19,$0, case8
	   	addi  $18,$18,-1
		j     c13
case81:     	addi  $18,$18,1
		j     c13
col1:       	addi  $20,$0, 0
           	addi  $23,$0,3
           	addi  $24,$0,5
	   	beq   $14, $0, case3
           	beq   $14, $23, case3
           	beq   $14, $24, case3
           	addi  $23, $0, 2
           	addi  $24, $0, 4
           	beq   $14, $23, case4
           	beq   $14, $24, case4
           	beq   $14, $22, case4   
		j     c13 
case31:     	addi  $20,$20,-1
		j     c13
case41:     	addi  $20,$20,1   
                j     c13
c13:		beq  $16, $8, c6
		addi $14,$14,1
		j    c4
c5:             add   $2, $18,$0
		add   $2, $2, $1
		sll   $2, $2, 3
		add   $3, $20,$0
                add   $3, $3,$9
		addi  $13, $13, 1
		sll   $13,$13, 3
		add   $13,$13,$3
		addi  $13,$13,1
		sll   $13,$13,2
		add   $2,$2,$3
           	addi  $3, $0, 0      # Open
          	swi   568            # returns result in $4 (-1: mine; 0-8: count) 
		addi  $5,$5,1
		sw    $4, maps($13)
		j     Loop5
c6:		add   $2, $18, $0
		add   $2, $2, $1
		add   $13,$2,$0
		sll   $2, $2, 3
		add   $3,$20,$0
                add   $3,$3,$9
		addi  $13, $13, 1
		sll   $13,$13, 3
		add   $13,$13,$3
		addi  $13,$13,1
		sll   $13,$13,2
		add   $2,$2,$3
		addi  $3, $0, 1      # Flag
          	swi   568            # returns result in $4 (9)
		sw    $4, maps($13)
		addi  $5,$5,1   
		j     Loop5
c10:            addi  $10,$10,1
		addi  $13,$0,10
		mult  $10,$13
		addi  $10, $10,-1
		mflo  $13
		add   $13,$13,$11
		addi  $13,$13,1
		sll   $13,$13,2
		lw    $14, maps($13)
		beq   $14,$8, c11
                j     Loop5
c11:            add   $2, $10,$0
		sll   $2, $2, 3
		add   $2, $2, $11
		addi  $3, $0, -1     # Guess
           	swi   568            # returns result in $4 (-1: mine; 0-8: count)
		sw    $4, maps($13)
		addi  $5,$5,1
		j     Loop5
End:          	jr  $31  	  	   # return to OS














	  