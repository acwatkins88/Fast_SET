$ Calculate delay of gate

.param vdd = 1.0
.param rftime = 30p 
.param starttime = 300p
.param cap = 0f
.param rtime = 60p
.param ftime = 60p

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
$IUP2 gnd p10 PWL(time, cur2) 
$VINA a 0 1V

$ high low high pulse 2
$IUP2 p10 gnd PWL(time, cur2) 
$VINA a 0 0V

VINB b 0 1V
$VINH c 0 PULSE(1 0 starttime rftime)
$VINH c 0 PULSE(1 .25 starttime rtime ftime 0p)
$VINH c 0 PULSE(1 .25 starttime rtime ftime 40p)

$ Single Pulse
$xnand1 c b vdd p9 nand2inp
$xnand2 b p9 vdd p10 nand2inp
$CLOAD p10 0 cap

$ Extra 2 input NAND gate
$xnand3 b p10 vdd p11 nand2inp
$CLOAD p11 0 cap

$VINH p11 0 PULSE(0 1 327.58p 20.62p 27.86p 91.1p)
$xnand4 b p11 vdd p12 nand2inp
$CLOAD p12 0 cap

VINH p12 0 PULSE(1 0.49078 347.36p 100.367p 68.7447p 11.8p)
xnand5 b p12 vdd p13 nand2inp
CLOAD p12 0 cap

$xnand6 b p13 vdd p14 nand2inp
$xnand7 b p14 vdd p15 nand2inp

$ Single Pulse
$xand8 b p12 vdd p13 nand2inp
$ Cat 2 Convergence
$xand5 p16 p12 vdd p13 nand2inp
$ Cat 3 Convergence
$xand5 p16 p15 vdd p17 nand2inp 

$xand6 b p17 vdd p18 nand2inp
$xand7 b p18 vdd p19 nand2inp

.data setsrce MER
FILE='pwlpulse.dat' time=1 cur=2 cur2=3
.enddata

$ Options
.option ingold = 1
.OPTION MEASFILE=1
.option measform=3
.option post

$ Commands to Measure Voltage at Output Maximum or Minimum
.meas tran minval MIN V(p12) 
.meas tran maxval MAX V(p12)
.meas tran minthresh find V(p11) when V(p12)=minval
.meas tran maxthresh find V(p11) when V(p12)=maxval

.tran data=setsrce 
$.PRINT TRAN I(IUP) V(z) V(p) V(output) 
.LIB "IBM96w90nm.lib" CMOS_MODELS
.lib "./Subcircuits/subcircuits.lib" std_cells

$ Rerun the simulaiton varying the capacitance
.alter case 1:
.param cap = 1f

.alter case 2:
.param cap = 2f

.alter case 3:
.param cap = 3f

.alter case 4:
.param cap = 4f

.alter case 5:
.param cap = 5f

.alter case 6:
.param cap = 6f

.alter case 7:
.param cap = 7f

.alter case 8:
.param cap = 8f

.alter case 9:
.param cap = 9f

.alter case 10:
.param cap = 10f

.alter case 11:
.param cap = 11f

.alter case 12:
.param cap = 12f

.alter case 13:
.param cap = 13f

.alter case 14:
.param cap = 14f

.alter case 15:
.param cap = 15f

.END 
