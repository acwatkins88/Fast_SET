$ Calculate delay of circuit

.param vdd = 1.0
.param cap = 1.0242f
.param starttime1 = 300p
.param rtime1 = 15p
.param ftime1 = 37.5p
.param width1 = 92.5p
.param peak1 = .75

.param starttime2 = 300p
.param rtime2 = 50p
.param ftime2 = 50p
.param width2 = 50p
.param peak2 = 1

$ Options
.option ingold = 1
.OPTION MEASFILE=1
.option post
.option captab

VDD vdd 0 vdd

VINA a 0 PULSE(0 peak1 starttime1 rtime1 ftime1 width1) 
VINC c 0 PULSE(0 peak2 starttime2 rtime2 ftime2 width2) 
VINS b 0 1V

xnand1 a b vdd p9 nand2inp
xnand2 p9 b vdd p10 nand2inp
xnand3 p10 b vdd out1 nand2inp

xnand4 c b vdd p01 nand2inp
xnand5 p01 b vdd p02 nand2inp
xnand6 p02 b vdd out2 nand2inp

.tran .05p 1n
 
.LIB "IBM96w90nm.lib" CMOS_MODELS
.lib "./Subcircuits/subcircuits.lib" std_cells

.END 
