$Effective Resistance Calculation

.param gate=-1.5
.param drain=-1.5
.param vdd=1.05
$.param VDD=0.5

$Find the Effective Resistance
VDD vdd gnd DC vdd

$Sources for Characterization
VG1 g gnd DC gate
VD1 d gnd DC drain

$ NMOS
$ 90nm NMOS
$MN1 d g gnd gnd CMOSN W=0.12U L=0.08U

$ 65nm NMOS
$MN1 d g gnd gnd CMOSN W=120n L=60n

$ 45nm NMOS
$MN1 d g gnd gnd CMOSN W=50n L=45n

$ 32nm NMOS
$MN1 d g gnd gnd CMOSN W=40n L=32n

$ PMOS
$ 90nm PMOS
$MP1 d g vdd vdd CMOSP W=0.24U L=0.08U 

$ 65nm PMOS
$MP1 d g vdd vdd CMOSP W=240n L=60n

$ 45nm PMOS
$MP1 d g vdd vdd CMOSP W=100n L=45n

$ 32nm PMOS
MP1 d g vdd vdd CMOSP W=80n L=32n 

.option post
.option measform = 1

.print gate

.dc gate -2 2.5 0.0125 drain -2 2.5 0.0125
$.dc gate -1.5 1.5 0.0125 

$ PMOS eff resistance
$.probe DC equivres = PAR('abs(V(vdd)-V(d))/abs(I(vd1))')


$ NMOS eff resistance
$.probe DC equivres = PAR('abs(V(d))/abs(I(vd1))')

.probe DC ivd = PAR('I(vd1)')

$.meas mres PARAM="V(d)/abs(I(vd1))"
$.meas gval PARAM="V(g)"

$.LIB "IBM96w90nm.lib" CMOS_MODELS
$.LIB "IBM65nm.lib" CMOS_MODELS
.LIB "32nm_HP.lib" CMOS_MODELS
.end
