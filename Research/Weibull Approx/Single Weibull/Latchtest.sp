
$ Calculate delay of gate

.param vdd = 1.05
.param hvolt = 0.25
.param rftime = 30p 
.param starttime = 300p
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
.print curabs = PAR('ABS(I(VDD))')

$ Injection Current Falling (if 0)
$IUP1 gnd n4 PWL(time, cur)

$ Injection Current Rising
IUP2 out gnd PWL(time, cur)

$ Injection Current 2 Falling
IUP3 gnd n4 PWL(time, cur2)

$ Injection Current 2 Rising
$IUP4 co3 gnd PWL(time, cur2)

$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

$ Injection Current Falling (if 0)
IUP6 gnd out PWL(time, cur3)

$ Injection Current Rising
$IUP7 n4 gnd PWL(time, cur3)

$ Injection Current 2 Falling
$IUP8 gnd co3 PWL(time, cur4)

$ Injection Current 2 Rising
IUP9 n4 gnd PWL(time, cur4)

$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

$IUP5 n5 gnd PULSE(0 4u 1500p 0 0 50p)
$.print I(IUP5) I(IUP4)

VINB b 0 hvolt

$ High  
$VDATA1 d 0 PULSE(0 1.05 500p 0 0 1000p 2000p)

$ Low 
VDATA1 d 0 PULSE(1.05 0 500p 0 0 1000p 2000p)

$ Clock Signals
VCLKH ckh 0 PULSE(0 1.05 0 0 0 500p 1000p)
VCLKL ckl 0 PULSE(1.05 0 0 0 0 500p 1000p) 

$ Stable Signals
$VDATA1 d 0 vdd

$VCLKH ckh 0 vdd
$VCLKL ckl 0 0

$$ Begin Proposed Double Node Tolerant Latch $$

$xrec d ckh ckl vdd out reclatch

$$ CELL 1

mp1a pm1a n5 vdd vdd CMOSP W=80n L=32n
mp1b pm1b n2 pm1a vdd CMOSP W=80n L=32n
mp1c co1 ckh pm1b vdd CMOSP W=80n L=32n
mn1a co1 ckl nm1a 0 CMOSN W=40n L=32n
mn1b nm1a n2 nm1b 0 CMOSN W=40n L=32n
mn1c nm1b n5 0 0 CMOSN W=40n L=32n

mp2 n1 co1 vdd vdd CMOSP W=80n L=32n
mn2 n1 co1 0 0 CMOSN W=40n L=32n

xpg1 ckl ckh vdd d co1 pgate

$$ CELL 2
mp3a pm3a n1 vdd vdd CMOSP W=80n L=32n
mp3b pm3b n5 pm3a vdd CMOSP W=80n L=32n
mp3c co2 n3 pm3b vdd CMOSP W=80n L=32n 
mn3a co2 n1 nm3a 0 CMOSN W=40n L=32n 
mn3b nm3a n4 nm3b 0 CMOSN W=40n L=32n 
mn3c nm3b n3 0 0 CMOSN W=40n L=32n 

mp4a pm4 out vdd vdd CMOSP W=80n L=32n
mp4b n2 co2 pm4 vdd CMOSP W=80n L=32n
mn4a n2 out nm4 0 CMOSN W=40n L=32n
mn4b nm4 co2 0 0 CMOSN W=40n L=32n

xpg4 ckl ckh vdd d co2 pgate

$$ CELL 3 

mp5a pm5b n4 vdd vdd CMOSP W=80n L=32n
mp5b co3 n2 pm5b vdd CMOSP W=80n L=32n
mn5a co3 n4 nm5a 0 CMOSN W=40n L=32n 
mn5b nm5a n2 0 0 CMOSN W=40n L=32n

mp6 n3 co3 vdd vdd CMOSP W=80n L=32n
mn6 n3 co3 0 0 CMOSN W=40n L=32n

$xpg3 ckl ckh vdd d co3 pgate

$$ CELL 4

mp7a pm7 co1 vdd vdd CMOSP W=80n L=32n
mp7b n4 out pm7 vdd CMOSP W=80n L=32n
mn7a n4 co1 nm7 0 CMOSN W=40n L=32n
mn7b nm7 co2 0 0 CMOSN W=40n L=32n

mp8a pm8 co2 vdd vdd CMOSP W=80n L=32n
mp8b n5 co3 pm8 vdd CMOSP W=80n L=32n
mn8a n5 out nm8 0 CMOSN W=40n L=32n
mn8b nm8 co3 0 0 CMOSN W=40n L=32n

mp9a pm9a n4 vdd vdd CMOSP W=80n L=32n
mp9b pm9b n1 pm9a vdd CMOSP W=80n L=32n
mp9c out n3 pm9b vdd CMOSP W=80n L=32n
mn9a out n1 nm9a 0 CMOSN W=40n L=32n
mn9b nm9a n3 nm9b 0 CMOSN W=40n L=32n 
mn9c nm9b n5 0 0 CMOSN W=40n L=32n 

xpg2 ckl ckh vdd d out pgate

$$ End Proposed Double Node Tolerant Latch $$

$$ Begin DONUT Latch

$xdnt d ckh ckl vdd out donut
$xdnt d ckh ckl vdd out donutstd

$$ End DONUT Latch

$$ Begin DICE Latch

$xdce d ckh ckl vdd out dice

$$ End DICE Latch

$$ Begin DNCS Latch

$xdce1 d ckh ckl vdd n1 dice
$xdce2 d ckh ckl vdd n2 dice

$xcele n1 n2 vdd out cele2

$mp7 out no vdd vdd CMOSP W=80n L=32n
$mn7 out no 0 0 CMOSN W=40n L=32n

$xinv2 out vdd no inv

$$ End DNCS Latch

$$ Begin HSMUF Latch

$xhsmuf d ckh ckl vdd out hsmuf

$$ End HSMUF Latch

$$ Begin FERST Latch

$xcele1 n1 n2 vdd n3 celeoutint2
$xinv1 n3 vdd no1 inv
$xpg1 ckh ckl vdd no1 n1 pgate

$xcele2 n1 n2 vdd n4 celeoutint2
$xinv2 n4 vdd no2 inv
$xpg2 ckh ckl vdd no2 n2 pgate

$xcele3 n3 n4 vdd out cele2
$mp7 out no vdd vdd CMOSP W=80n L=32n
$mn7 out no 0 0 CMOSN W=40n L=32n

$xinv3 out vdd no inv

$xpg3 ckl ckh vdd d n2 pgate
$xpg4 ckl ckh vdd d n1 pgate

$$ End FERST Latch

$$ Begin HIPER Latch

$mp1 n2 n1 vdd vdd CMOSP W=80n L=32n
$mn1 n2 out 0 0 CMOSN W=40n L=32n

$mp2 n3 out vdd vdd CMOSP W=80n L=32n
$mn2 n3 n1 0 0 CMOSN W=40n L=32n

$mp3a pm3 ckl vdd vdd CMOSP W=80n L=32n
$mp3b n1 n4 pm3 vdd CMOSP W=80n L=32n
$mn3a n1 n4 nm3 0 CMOSN W=40n L=32n
$mn3b nm3 ckh 0 0 CMOSN W=40n L=32n

$xinv1 n1 vdd n4 inv

$mp4a pm4 n2 vdd vdd CMOSP W=80n L=32n
$mp4b out n4 pm4 vdd CMOSP W=80n L=32n
$mn4a out n4 nm4 0 CMOSN W=40n L=32n
$mn4b nm4 n3 0 0 CMOSN W=40n L=32n

$xpg1 ckl ckh vdd d out pgate
$xpg2 ckl ckh vdd d n1 pgate

$$ End HIPER Latch

$$ Begin Interception Latch

$xinter d ckh vdd out inter

$$ End Interception Latch

.data setsrce MER
FILE='pwlpulse.dat' time=1 cur=2 cur2=3 cur3=4 cur4=5
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
