
$ Calculate delay of gate

.param vdd = 1.05
.param rftime = 30p 
.param starttime = 300p
.param cap = 0.2f
.param rtime = 60p
.param ftime = 60p
$.param rtime = .001f
$.param ftime = .001f

$ Options
.option ingold = 1
.OPTION MEASFILE=1
.option post
.option captab

VDD vdd 0 vdd

$ high low high pulse 1
$IUP gnd p9 PWL(time, cur)
$VINC c 0 vdd

$ low high low pulse 1
IUP p9 gnd PWL(time, cur)
VINC c 0 0V

$ high low high pulse 2
$IUP2 gnd p10 PWL(time, cur2) 
$VINA a 0 vdd

$ low high low pulse 2
IUP2 p10 gnd PWL(time, cur2) 
VINA a 0 0V

VINB b 0 0V

$ Single Pulse
$xnor1 c b vdd p9 nor2inp
$xnor2 b p9 vdd p10 nor2inp

$ Extra 2 input NAND gate
$xnor3 b p10 vdd p11 nor2inp
$CLOAD p11 0 cap

$xnand10 p11 b b vdd p12 nand3inp

$xnor4 b p11 vdd p12 nand2inp

$xnor5 b p12 vdd p13 nor2inp
$xnor6 b p13 vdd p14 nor2inp
$xnor7 b p14 vdd p15 nor2inp


$ Double Pulse 
$ Pulse 1
xnor1 c b vdd p9 nor2inp
xnor2 b p9 vdd p11 nor2inp

$ Pulse 2
xnor8 a b vdd p10 nor2inp
xnor9 b p10 vdd p12 nor2inp

$ Single Pulse
$xand8 p11 p12 vdd p13 nand2inp

$ Cat 2 Convergence
$xnor5 p10 p9 vdd p13 nor2inp
$xnor5 p12 p11 vdd p13 nor2inp

MP1 cn1 p12 vdd vdd CMOSP W=80n L=32n 
MP2 p13 p11 cn1 vdd CMOSP W=80n L=32n 
MN1 p13 p12 gnd gnd CMOSN W=40n L=32n 
MN2 p13 p11 gnd gnd CMOSN W=40n L=32n

.print V(cn1) 

.data setsrce MER
FILE='pwlpulse.dat' time=1 cur=2 cur2 = 3
.enddata

.tran data=setsrce 
$.PRINT TRAN I(IUP) V(z) V(p) V(output) 
$.LIB "IBM96w90nm.lib" CMOS_MODELS
$.lib "./Subcircuits/subcircuits.lib" std_cells

.LIB "32nm_HP.lib" CMOS_MODELS
.lib "./Subcircuits/subcircuits32nm.lib" std_cells
.END 
