
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
IUP1 gnd n1 PWL(time, cur)

$ Injection Current Rising
IUP2 gnd n2 PWL(time, cur)

$ Injection Current 2 Falling
IUP3 gnd n3 PWL(time, cur)

$ Injection Current 2 Rising
$IUP4 co3 gnd PWL(time, cur2)

$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

$ Injection Current Falling (if 0)
IUP6 n1 gnd PWL(time, cur3)

$ Injection Current Rising
IUP7 n2 gnd PWL(time, cur3)

$ Injection Current 2 Falling
IUP8 n3 gnd PWL(time, cur3)

$ Injection Current 2 Rising
$IUP9 n4 gnd PWL(time, cur4)

$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

$IUP5 n5 gnd PULSE(0 4u 1500p 0 0 50p)
$.print I(IUP5) I(IUP4)

VINB b 0 hvolt

$ High  
$VDATA1 d 0 PULSE(0 1.05 500p 0 0 1000p 2000p)

$ Low 
VDATA1 d 0 PULSE(1.05 0 250p 0 0 1000p 2000p)
$VDATA1 in 0 PULSE(1.05 0 500p 0 0 1000p 2000p)

$ Clock Signals
VCLKH ckh 0 PULSE(0 1.05 0 0 0 500p 1000p)
VCLKL ckl 0 PULSE(1.05 0 0 0 0 500p 1000p) 

$ Stable Signals
$VDATA1 d 0 vdd

$VCLKH ckh 0 vdd
$VCLKL ckl 0 0

$$ Begin HRDNUT$$

$xrec d ckh ckl vdd out reclatch

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

$$ Atempt at TNU Latch

$xtnu d ckh ckl vdd out TNUlatch

mp1a n1pa cn2 vdd vdd CMOSP W=80n L=32n
mp1b n1pb cn3 n1pa vdd CMOSP W=80n L=32n
mp1c n1pc cn4 n1pb vdd CMOSP W=80n L=32n
mp1d n1pd cn5 n1pc vdd CMOSP W=80n L=32n
mp1e n1 ckh n1pd vdd CMOSP W=80n L=32n
mn1a n1 ckl n1na 0 CMOSN W=40n L=32n
mn1b n1na cn2 n1nb 0 CMOSN W=40n L=32n
mn1c n1nb cn3 n1nc 0 CMOSN W=40n L=32n
mn1d n1nc cn4 n1nd 0 CMOSN W=40n L=32n
mn1e n1nd cn5 0 0 CMOSN W=40n L=32n

mp1 cn1 n1 vdd vdd CMOSP W=80n L=32n
mn1 cn1 n1 0 0 CMOSN W=40n L=32n

xpg1 ckl ckh vdd d n1 pgate

mp2a n2pa cn1 vdd vdd CMOSP W=80n L=32n
mp2b n2pb cn3 n2pa vdd CMOSP W=80n L=32n
mp2c n2pc cn4 n2pb vdd CMOSP W=80n L=32n
mp2d n2pd cn5 n2pc vdd CMOSP W=80n L=32n
mp2e n2 ckh n2pd vdd CMOSP W=80n L=32n
mn2a n2 ckl n2na 0 CMOSN W=40n L=32n
mn2b n2na cn1 n2nb 0 CMOSN W=40n L=32n
mn2c n2nb cn3 n2nc 0 CMOSN W=40n L=32n
mn2d n2nc cn4 n2nd 0 CMOSN W=40n L=32n
mn2e n2nd cn5 0 0 CMOSN W=40n L=32n

mp2 cn2 n2 vdd vdd CMOSP W=80n L=32n
mn2 cn2 n2 0 0 CMOSN W=40n L=32n

xpg2 ckl ckh vdd d n2 pgate

mp3a n3pa cn1 vdd vdd CMOSP W=80n L=32n
mp3b n3pb cn2 n3pa vdd CMOSP W=80n L=32n
mp3c n3pc cn4 n3pb vdd CMOSP W=80n L=32n
mp3d n3pd cn5 n3pc vdd CMOSP W=80n L=32n
mp3e n3 ckh n3pd vdd CMOSP W=80n L=32n
mn3a n3 ckl n3na 0 CMOSN W=40n L=32n
mn3b n3na cn1 n3nb 0 CMOSN W=40n L=32n
mn3c n3nb cn2 n3nc 0 CMOSN W=40n L=32n
mn3d n3nc cn4 n3nd 0 CMOSN W=40n L=32n
mn3e n3nd cn5 0 0 CMOSN W=40n L=32n

mp3 cn3 n3 vdd vdd CMOSP W=80n L=32n
mn3 cn3 n3 0 0 CMOSN W=40n L=32n

xpg3 ckl ckh vdd d n3 pgate

mp4a n4pa cn1 vdd vdd CMOSP W=80n L=32n
mp4b n4pb cn2 n4pa vdd CMOSP W=80n L=32n
mp4c n4pc cn3 n4pb vdd CMOSP W=80n L=32n
mp4d n4pd cn5 n4pc vdd CMOSP W=80n L=32n
mp4e n4 ckh n4pd vdd CMOSP W=80n L=32n
mn4a n4 ckl n4na 0 CMOSN W=40n L=32n
mn4b n4na cn1 n4nb 0 CMOSN W=40n L=32n
mn4c n4nb cn2 n4nc 0 CMOSN W=40n L=32n
mn4d n4nc cn3 n4nd 0 CMOSN W=40n L=32n
mn4e n4nd cn5 0 0 CMOSN W=40n L=32n

mp4 cn4 n4 vdd vdd CMOSP W=80n L=32n
mn4 cn4 n4 0 0 CMOSN W=40n L=32n

xpg4 ckl ckh vdd d n4 pgate

mp5a n5pa cn1 vdd vdd CMOSP W=80n L=32n
mp5b n5pb cn2 n5pa vdd CMOSP W=80n L=32n
mp5c n5pc cn3 n5pb vdd CMOSP W=80n L=32n
mp5d n5 cn4 n5pc vdd CMOSP W=80n L=32n
mn5a n5 cn1 n5na 0 CMOSN W=40n L=32n
mn5b n5na cn2 n5nb 0 CMOSN W=40n L=32n
mn5c n5nb cn3 n5nc 0 CMOSN W=40n L=32n
mn5d n5nc cn4 0 0 CMOSN W=40n L=32n

mp5 cn5 n5 vdd vdd CMOSP W=80n L=32n
mn5 cn5 n5 0 0 CMOSN W=40n L=32n

mp6a n6pa n5 vdd vdd CMOSP W=80n L=32n
mp6b n6pb n1 n6pa vdd CMOSP W=80n L=32n
mp6c n6 n2 n6pb vdd CMOSP W=80n L=32n
mn6a n6 n5 n6na 0 CMOSN W=40n L=32n
mn6b n6na n1 n6nb 0 CMOSN W=40n L=32n
mn6c n6nb n2 0 0 CMOSN W=40n L=32n

mp7a n7pa n5 vdd vdd CMOSP W=80n L=32n
mp7b n7pb n3 n7pa vdd CMOSP W=80n L=32n
mp7c n7 n4 n7pb vdd CMOSP W=80n L=32n
mn7a n7 n5 n7na 0 CMOSN W=40n L=32n
mn7b n7na n3 n7nb 0 CMOSN W=40n L=32n
mn7c n7nb n4 0 0 CMOSN W=40n L=32n

mp8a n8pa n6 vdd vdd CMOSP W=80n L=32n
mp8b out n7 n8pa vdd CMOSP W=80n L=32n
mn8a out n6 n8na 0 CMOSN W=40n L=32n
mn8b n8na n7 0 0 CMOSN W=40n L=32n

$$ End TNU Latch

$$ Pulse Filtering Circuit

$xbuf1 in vdd n1 buf
$xbuf2 n1 vdd n2 buf 
$xbuf3 n2 vdd n3 buf 
$xbuf4 n3 vdd out1 buf  

$xbuf5 in vdd na buf
$xbuf6 na vdd nb buf 
$xbuf7 nb vdd nc buf 
$xbuf8 nc vdd nd buf  
$xbuf9 nd vdd ne buf
$xbuf10 ne vdd nf buf 
$xbuf11 nf vdd ng buf 
$xbuf12 ng vdd out2 buf  

$$ End Pulse Filtering Circuit

.data setsrce MER
FILE='pwlpulse.dat' time=1 cur=2 cur2=3 cur3=4 cur4=5
.enddata

.tran data=setsrce
$.tran 2p 2n
$.tran 2p 200n
$.PRINT TRAN I(IUP) V(z) V(p) V(output) 
$.LIB "IBM96w90nm.lib" CMOS_MODELS

$.LIB "IBM65nm.lib" CMOS_MODELS
$.LIB "./Subcircuits/subcircuits65nm.lib" std_cells

$.LIB "45nm_HP.lib" CMOS_MODELS
$.LIB "./Subcircuits/subcircuits45nm.lib" std_cells

.LIB "32nm_HP.lib" CMOS_MODELS
.lib "./Subcircuits/subcircuits32nm.lib" std_cells
.END 
