
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
$ low high low pulse 1
IUP gnd p9 PWL(time, cur)
VINC c 0 vdd

$ high low high pulse 1
$IUP p9 gnd PWL(time, cur)
$VINC c 0 0V

$ low high low pulse 2
IUP2 gnd p10 PWL(time, cur2) 
VINA a 0 vdd

$ high low high pulse 2
$IUP2 p10 gnd PWL(time, cur2) 
$VINA a 0 0V

VINB b 0 vdd

$ Double Pulse 
$ Pulse 1
xand1 c b vdd p9 nand2inp
xand2 b p9 vdd p11 nand2inp
$xand3 p11 b vdd p13 nand2inp
$xand4 b p13 vdd p15 nand2inp

$ Pulse 2
xand8 a b vdd p10 nand2inp
xand9 b p10 vdd p12 nand2inp
$xand10 b p12 vdd p14 nand2inp
$xand11 b p14 vdd p16 nand2inp

$ Single Pulse
$xand12 b p12 vdd p13 nand2inp

$ Cat 2 Convergence
$xand5 p12 p11 vdd p13 nand2inp
$xand5 p12 b p11 vdd p13 nand3inp

$ 32 nm 
$MP1 p13 p12 vdd vdd CMOSP W=80n L=32n 
$MP2 p13 p11 vdd vdd CMOSP W=80n L=32n 
$MN1 p13 p12 cn1 gnd CMOSN W=40n L=32n 
$MN2 cn1 p11 gnd gnd CMOSN W=40n L=32n

$ 65 nm
$MP1 p13 p12 vdd vdd CMOSP W=240n L=60n 
$MP2 p13 p11 vdd vdd CMOSP W=240n L=60n 
$MN1 p13 p12 cn1 gnd CMOSN W=120n L=60n 
$MN2 cn1 p11 gnd gnd CMOSN W=120n L=60n

$.print V(cn1) 
$xand5 b p13 vdd p14 nand2inp
$xand5 p13 b b vdd p14 nand3inp

$ 32nm 3 input
MP1 p13 p11 vdd vdd CMOSP W=80n L=32n 
MP2 p13 p12 vdd vdd CMOSP W=80n L=32n 
MP3 p13 b vdd vdd CMOSP W=80n L=32n 
MN1 p13 p11 cn1 0 CMOSN W=40n L=32n 
MN2 cn1 p12 cn2 0 CMOSN W=40n L=32n
MN3 cn2 b 0 0 CMOSN W=40n L=32n

.print V(cn1) V(cn2)

$ Cat 3 Convergence
$xand5 p16 p15 vdd p17 nand2inp 

$xand6 b p17 vdd p18 nand2inp
$xand7 b p18 vdd p19 nand2inp

.data setsrce MER
FILE='pwlpulse.dat' time=1 cur=2 cur2 = 3
.enddata

.tran data=setsrce
$.tran 2p 1.2n
$.PRINT TRAN I(IUP) V(z) V(p) V(output) 
$.LIB "IBM96w90nm.lib" CMOS_MODELS

$.LIB "IBM65nm.lib" CMOS_MODELS
$.LIB "./Subcircuits/subcircuits65nm.lib" std_cells

$.LIB "45nm_HP.lib" CMOS_MODELS
$.LIB "./Subcircuits/subcircuits45nm.lib" std_cells

.LIB "32nm_HP.lib" CMOS_MODELS
.lib "./Subcircuits/subcircuits32nm.lib" std_cells
.END 
