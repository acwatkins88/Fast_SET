$ Standard Cell Gate Characterization

$ Parameter Definitions (Similar to variable definitions in C)
.param vdd = 1.0
.param cap = 0
.param rftime = 36p
.param rftime2 = 49.3p
.param cap2 = 1f

$ Gate Definitions
$ Refer to subcircuits.lib for gate definitions

$ A NAND gate loaded with another NAND gate
$ A capacitance is attached to the output load
$ Gate Definition:  xnand# input input power output (gate as defined in subcircuits.lib)

$xnand2 a b vdd out nand2inp
$xnand2 b a vdd out nand2inp
$xnand5 out b vdd aout nand2inp

MP1 p11 a vdd vdd CMOSP W=0.24U L=0.08U 
MP2 p11 b vdd vdd CMOSP W=0.24U L=0.08U 
MN1 p11 a cn1 0 CMOSN W=0.12U L=0.08U 
MN2 cn1 b 0 0 CMOSN W=0.12U L=0.08U
$CLOADB out 0 cap

$ A NAND gate loaded with another NAND GAME
xnand4 b a vdd znt nand2inp
xnand3 znt b vdd ldout nand2inp
$CLOADA znt 0 cap2

$xnor1 a c vdd zno nor2inp
$CLOADV zno 0 cap

$xinv1 a vdd zi inv
$CLOADI zi 0 cap 

$ Voltage Source Definitions
VINA a 0 PULSE(0 1 400ps rftime)
VINB v 0 PULSE(1 0 500ps rftime2)
VINand b 0 1V
vinor c 0 0V
vdd vdd 0 vdd

$ Tell HSPICE to conduct a transient analysis in .5 picosecond steps for 1 nanosecond

.tran .5p 1n 
$.TRAN .5p 2n SWEEP cap lin 61 0f 3f

$ Simulation Options (refer to the manual or each option) 
.OPTION MEASFILE=1
.option measform=3
.option MEASDGT = 8
.option ingold=0
.option post

$ Commands to measure delay
$ trig v(a) - measure the voltage at input a
$ td=0ns - delay the measurement by 0ns
$ val='vdd/2' cross=1 - start calculating the gate delay the first time the simulator crosses vdd/2
$ targ v(out) - measure the voltage to the output out

$.meas tran delay_nand trig v(a)  td=0ns val='vdd/2' cross=1
$+		      targ v(out)  td=0ns val='vdd/2' cross=1

$.meas tran delay_arrnand trig v(a)  td=0ns val='.99' cross=1
$+		      targ v(out)  td=0ns val='.01' cross=1

.meas tran delay_arrnand trig v(a)  td=0ns val='.01' cross=1
+		      targ v(out)  td=0ns val='.99' cross=1

$.meas tran delay_nor trig v(a)  td=0ns val='vdd/2' cross=1
$+		      targ v(zno)  td=0ns val='vdd/2' cross=1
$.meas tran delay_inv trig v(a)  td=0ns val='vdd/2' cross=1
$+		      targ v(zi)  td=0ns val='vdd/2' cross=1
.meas tran risetime param = 'rftime'

.PRINT TRAN V(a) v(b) v(c) V(zna)

$ Import the subcircuts and the library files 
$ NOTE - you will need to change the subcircuits directory to your directory 
.LIB "IBM96w90nm.lib" CMOS_MODELS
.lib "./Subcircuits/subcircuits.lib" std_cells

$ Rerun the simulaiton with rftime=.005n
.alter case 1:
.param rftime = .005n

.alter case 2:
.param rftime = .01n

.alter case 3:
.param rftime = .015n

.alter case 4:
.param rftime = .02n

.alter case 5: 
.param rftime = .025n

.alter case 6:
.param rftime = .03n

.alter case 7:
.param rftime = .035n

.alter case 8:
.param rftime = .04n

.alter case 9:
.param rftime = .045n

.alter case 10:
.param rftime = .05n

.alter case 11:
.param rftime = .055n

.alter case 12:
.param rftime = .06n

.alter case 13:
.param rftime = .065n

.alter case 14:
.param rftime = .07n

.alter case 15:
.param rftime = .075n

.alter case 16:
.param rftime = .08n

.alter case 17:
.param rftime = .085n

.alter case  18:
.param rftime = .09n

.alter case  19:
.param rftime = .095n

.alter case  20:
.param rftime = .1n

.alter case 21:
.param rftime = .105n

.alter case 22:
.param rftime = .11n

.alter case 23:
.param rftime = .115n

.alter case 24:
.param rftime = .120n

.alter case 25: 
.param rftime = .125n

.alter case 26:
.param rftime = .13n

.alter case 27:
.param rftime = .135n

.alter case 28:
.param rftime = .14n

.alter case 29:
.param rftime = .145n

.alter case 30:
.param rftime = .15n

.alter case 31:
.param rftime = .155n

.alter case 32:
.param rftime = .16n

.alter case 33:
.param rftime = .165n

.alter case 34:
.param rftime = .17n

.alter case 35:
.param rftime = .175n

.alter case 36:
.param rftime = .18n

.alter case 37:
.param rftime = .185n

.alter case 38:
.param rftime = .19n

.alter case 39:
.param rftime = .195n

.alter case 40:
.param rftime = .2n

.alter case 41:
.param rftime = .205n

.alter case 42:
.param rftime = .21n

.alter case 43:
.param rftime = .215n

.alter case 44:
.param rftime = .22n

.alter case 45:
.param rftime = .225n

.alter case 46:
.param rftime = .23n

.alter case 47:
.param rftime = .235n

.alter case 48:
.param rftime = .24n

.alter case 49:
.param rftime = .245n

.alter case 50:
.param rftime = .25n

.alter case 51:
.param rftime = .255n

.alter case 52:
.param rftime = .26n

.alter case 53:
.param rftime = .265n

.alter case 54:
.param rftime = .27n

.alter case 55:
.param rftime = .275n

.alter case 56:
.param rftime = .28n

.alter case 57:
.param rftime = .285n

.alter case 58:
.param rftime = .29n

.alter case 59:
.param rftime = .295n

.alter case 60:
.param rftime = .3n

.alter case 61:
.param rftime = .305n

.alter case 62:
.param rftime = .310n

.alter case 63:
.param rftime = .315n

.alter case 64:
.param rftime = .320n

.alter case 65:
.param rftime = .325n

.alter case 66:
.param rftime = .330n

.alter case 67:
.param rftime = .335n

.alter case 68:
.param rftime = .340n

.alter case 69:
.param rftime = .345n

.alter case 70:
.param rftime = .350n

.alter case 71:
.param rftime = .355n

.alter case 72:
.param rftime = .360n

.alter case 73:
.param rftime = .365n

.alter case 74:
.param rftime = .370n

.alter case 75:
.param rftime = .375n

.alter case 76:
.param rftime = .380n

.alter case 77:
.param rftime = .385n

.alter case 78:
.param rftime = .390n

.alter case 79:
.param rftime = .395n

.alter case 80:
.param rftime = .400n
.END 
