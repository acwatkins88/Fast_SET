
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

.print I(IUP) I(IUP2)

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

$ 32 nm
$MP1 p13 p11 vdd vdd CMOSP W=80n L=32n 
$MP2 p13 p12 vdd vdd CMOSP W=80n L=32n 
$MP3 p13 b vdd vdd CMOSP W=80n L=32n 
$MN1 p13 p11 cn1 0 CMOSN W=40n L=32n 
$MN2 cn1 p12 cn2 0 CMOSN W=40n L=32n
$MN3 cn2 b 0 0 CMOSN W=40n L=32n

$ 65 nm
MP1 p13 p11 vdd vdd CMOSP W=240n L=60n 
MP2 p13 p12 vdd vdd CMOSP W=240n L=60n 
MP3 p13 b vdd vdd CMOSP W=240n L=60n 
MN1 p13 p11 cn1 0 CMOSN W=120n L=60n 
MN2 cn1 p12 cn2 0 CMOSN W=120n L=60n
MN3 cn2 b 0 0 CMOSN W=120n L=60n

.$print V(cn1) V(cn2)

$ Cat 3 Convergence
$xand5 p16 p15 vdd p17 nand2inp 

$xand6 b p17 vdd p18 nand2inp
$xand7 b p18 vdd p19 nand2inp

.data setsrce MER
FILE='pwlpulse1.dat' time=1 cur=2 cur2 = 3
.enddata

.tran data=setsrce
$.tran 2p 1.2n
$.PRINT TRAN I(IUP) V(z) V(p) V(output) 
$.LIB "IBM96w90nm.lib" CMOS_MODELS

.LIB "IBM65nm.lib" CMOS_MODELS
.LIB "./Subcircuits/subcircuits65nm.lib" std_cells

$.LIB "45nm_HP.lib" CMOS_MODELS
$.LIB "./Subcircuits/subcircuits45nm.lib" std_cells

$.LIB "32nm_HP.lib" CMOS_MODELS
$.lib "./Subcircuits/subcircuits32nm.lib" std_cells


.alter case 2:
.data setsrce MER
FILE='pwlpulse2.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 3:
.data setsrce MER
FILE='pwlpulse3.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 4:
.data setsrce MER
FILE='pwlpulse4.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 5:
.data setsrce MER
FILE='pwlpulse5.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 6:
.data setsrce MER
FILE='pwlpulse6.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 7:
.data setsrce MER
FILE='pwlpulse7.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 8:
.data setsrce MER
FILE='pwlpulse8.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 9:
.data setsrce MER
FILE='pwlpulse9.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 10:
.data setsrce MER
FILE='pwlpulse10.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 11:
.data setsrce MER
FILE='pwlpulse11.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 12:
.data setsrce MER
FILE='pwlpulse12.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 13:
.data setsrce MER
FILE='pwlpulse13.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 14:
.data setsrce MER
FILE='pwlpulse14.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 15:
.data setsrce MER
FILE='pwlpulse15.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 16:
.data setsrce MER
FILE='pwlpulse16.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 17:
.data setsrce MER
FILE='pwlpulse17.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 18:
.data setsrce MER
FILE='pwlpulse18.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 19:
.data setsrce MER
FILE='pwlpulse19.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 20:
.data setsrce MER
FILE='pwlpulse20.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 21:
.data setsrce MER
FILE='pwlpulse21.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 22:
.data setsrce MER
FILE='pwlpulse22.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 23:
.data setsrce MER
FILE='pwlpulse23.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 24:
.data setsrce MER
FILE='pwlpulse24.dat' time=1 cur=2 cur2 = 3
.enddata

.alter case 25:
.data setsrce MER
FILE='pwlpulse25.dat' time=1 cur=2 cur2 = 3
.enddata

$.alter case 26:
$.data setsrce MER
$FILE='pwlpulse26.dat' time=1 cur=2 cur2 = 3
$.enddata

$.alter case 27:
$.data setsrce MER
$FILE='pwlpulse27.dat' time=1 cur=2 cur2 = 3
$.enddata

$.alter case 28:
$.data setsrce MER
$FILE='pwlpulse28.dat' time=1 cur=2 cur2 = 3
$.enddata

$.alter case 29:
$.data setsrce MER
$FILE='pwlpulse29.dat' time=1 cur=2 cur2 = 3
$.enddata

$.alter case 30:
$.data setsrce MER
$FILE='pwlpulse30.dat' time=1 cur=2 cur2 = 3
$.enddata

.END 
