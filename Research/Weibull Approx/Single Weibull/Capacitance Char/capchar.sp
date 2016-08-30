$ Characterize Capacitance

$ Value of Current Source
.param maxcurval = 5m
.param mincurval = 0

$ VDD
.param vdd = 1
vdd vdd 0 vdd

$ Nand Gate
MP1 z a vdd vdd CMOSP W=0.24U L=0.08U 
MP2 z b vdd vdd CMOSP W=0.24U L=0.08U 
MN1 z a cn1 0 CMOSN W=0.12U L=0.08U 
MN2 cn1 b 0 0 CMOSN W=0.12U L=0.08U

$ Input Source Characterize a-z
ita a z pulse(mincurval maxcurval 50ps 0 0 50ps)
vsa a 0 0V
vsb b 0 vdd

$ Input Source Characterize b-z
$itb b z pulse(mincurval maxcurval 50ps 0 0 50ps)
$vsa a 0 vdd
$vsb b 0 0V

$ Transient Analysis 
.tran .05p .15n

.print I(ita) par('V(z) - V(a)')

.OPTION MEASFILE=1
$.option measform=3
.option MEASDGT = 8
.option ingold=0
.option post

$
$
$ Calculate Time Between .2*Vdd and .8*Vdd (Input a)
$ Comment for Input b Simulation
.meas tran absmax MAX par('V(z) - V(a)')
.meas tran absmin MIN par('V(z) - V(a)')
.meas maxvolt param='(abs(absmax - absmin))*.8 + absmin'
.meas minvolt param='(abs(absmax - absmin))*.2 + absmin'
.meas deltav param='maxvolt-minvolt' 


.meas tran atimer trig par('V(z)-V(a)') td=0ns val=maxvolt cross = 1
+	targ par('V(z)-V(a)') td=0ns val=minvolt cross = 1

.meas tran atimef trig par('V(z)-V(a)') td=0ns val=maxvolt cross = 2
+	targ par('V(z)-V(a)') td=0ns val=minvolt cross = 2
$ End Input a Simulation
$
$

$
$
$ Calculate Time Between .2*Vdd and .8*Vdd (Input b)
$ Comment for Input a Simulation
$.meas tran maxvolt MAX par('V(b) - V(z)')
$.meas tran minvolt MIN par('V(b) - V(z)')
$.meas deltav param='abs(maxvolt - minvolt)' 

$.meas tran atimer trig par('V(b)-V(z)') td=0ns val='minvolt+abs(.05*deltav)' cross = 1
$+	targ par('V(b)-V(z)') td=0ns val='maxvolt-abs(.05*deltav)' cross = 1

$.meas tran atimef trig par('V(b)-V(z)') td=0ns val='maxvolt-abs(.05*deltav)' cross = 2
$+	targ par('V(b)-V(z)') td=0ns val='minvolt+abs(.05*deltav)' cross = 2
$ End Input b Simulation
$
$

.meas capr param='abs((abs(maxcurval-mincurval)*atimer)/(deltav))'
.meas capf param='abs((abs(maxcurval-mincurval)*atimef)/(deltav))'
.meas captot param='abs(capr+capf)/2'


.LIB "IBM96w90nm.lib" CMOS_MODELS
.END
