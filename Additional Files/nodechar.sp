$ Calculate delay of gate

.param vdd = 1.0
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
VINC c 0 1V

$ high low high pulse 1
$IUP p9 gnd PWL(time, cur)
$VINC c 0 0V

$ low high low pulse 2
IUP2 gnd p10 PWL(time, cur2) 
VINA b 0 1V

$ high low high pulse 2
$IUP2 p10 gnd PWL(time, cur2) 
$VINA a 0 0V

$VINB b 0 1V

$ Single Pulse
xnand1 c b vdd p9 nand2inp
xnand2 b p9 vdd p10 nand2inp

MP1 p11 b vdd vdd CMOSP W=0.24U L=0.08U 
MP2 p11 p10 vdd vdd CMOSP W=0.24U L=0.08U 
MN1 p11 b cn1 gnd CMOSN W=0.12U L=0.08U 
MN2 cn1 p10 gnd gnd CMOSN W=0.12U L=0.08U

$ Extra 2 input NAND gate
$xnand3 b p10 vdd p11 nand2inp
CLOAD p11 0 cap

$ NAND transistor gate
$MP1 p12 b vdd vdd CMOSP W=0.24U L=0.08U 
$MP2 p12 p11 vdd vdd CMOSP W=0.24U L=0.08U 
$MN1 p12 b cn1 gnd CMOSN W=0.12U L=0.08U 
$MN2 cn1 p11 gnd gnd CMOSN W=0.12U L=0.08U

.probe TRAN equivres1 = PAR('abs(V(vdd)-V(p12))/abs(I(MP1))')
.probe TRAN equivres2 = PAR('abs(V(vdd)-V(p12))/abs(I(MP2))')
.probe TRAN equivresn3 = PAR('abs(V(p12)-V(cn1))/abs(I(MN1))')
.probe TRAN equivresn4 = PAR('abs(V(cn1)-V(gnd))/abs(I(MN2))')

.print V(cn1)

$xnand4 b p11 vdd p12 nand2inp

$xnand5 b p12 vdd p13 nand2inp
$xnand6 b p13 vdd p14 nand2inp
$xnand7 b p14 vdd p15 nand2inp
 

$ Double Pulse 
$ Pulse 1
$xand1 c b vdd p9 nand2inp
$xand2 b p9 vdd p11 nand2inp
$xand3 b p11 vdd p13 nand2inp
$xand4 b p13 vdd p15 nand2inp

$ Pulse 2
$xand8 a b vdd p10 nand2inp
$xand9 b p10 vdd p12 nand2inp
$xand10 b p12 vdd p14 nand2inp
$xand11 b p14 vdd p16 nand2inp

$ Single Pulse
$xand8 b p12 vdd p13 nand2inp
$ Cat 2 Convergence
$xand5 p16 p12 vdd p13 nand2inp
$ Cat 3 Convergence
$xand5 p16 p15 vdd p17 nand2inp 

$xand6 b p17 vdd p18 nand2inp
$xand7 b p18 vdd p19 nand2inp

.data setsrce MER
FILE='pwlpulse.dat' time=1 cur=2 cur2 = 3
.enddata

.tran data=setsrce 
$.PRINT TRAN I(IUP) V(z) V(p) V(output) 
.LIB "IBM96w90nm.lib" CMOS_MODELS
.lib "./Subcircuits/subcircuits.lib" std_cells
.END 
