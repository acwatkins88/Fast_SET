$ Miller Capacitance Characterization

$.param drain = -1.5
.param VDD = 1.05
$.param VDD = 0.5

.param rftime = 100p
.param tstep = 0.5p
.param starttime = 0

$ Find the Effective Resistance
VDD vdd gnd DC VDD

$ Sources for Simulation
VING g 0 PULSE(-2 2.5 starttime rftime)
VD1 d gnd DC drain

$CLOAD d gnd 1e-18

$ NMOS Miller Characterization

$ 90nm NMOS
$MN1 d g gnd gnd CMOSN W=0.12U L=0.08U

$ 65nm NMOS
$MN1 d g gnd gnd CMOSN W=120n L=60n

$ 45nm NMOS
$MN1 d g gnd gnd CMOSN W=50n L=45n

$ 32nm NMOS
MN1 d g gnd gnd CMOSN W=40n L=32n

$ PMOS Miller Characterization

$ 90nm NMOS
$MP1 d g vdd vdd CMOSP W=0.24U L=0.08U 

$ 65nm PMOS
$MP1 d g vdd vdd CMOSP W=240n L=60n

$ 45nm PMOS
$MP1 d g vdd vdd CMOSP W=100n L=45n

$ 32nm PMOS
$MP1 d g vdd vdd CMOSP W=80n L=32n 

.option post
$.option measform = 1
.option INTERP=1

$.DC gavol 0 1 0.05
$.TRAN tstep rftime
.TRAN tstep rftime SWEEP drain -2 2.5 0.0125

$.probe TRAN dtc=deriv('V(d) - V(g)')
$.probe TRAN dttest = deriv('V(d)')
.param dtn = -45e9

$ Calculate Miller Capacitance
.probe TRAN cmgd = PAR('abs(I(VING)/(dtn))')
$.probe TRAN cmgd = PAR('abs((I(MN1)-I(MN2))/(dttest))')

$ 90 nm
$.LIB "IBM96w90nm.lib" CMOS_MODELS

$ 65 nm
$.LIB "IBM65nm.lib" CMOS_MODELS

$ 32 nm PTM
.LIB "32nm_HP.lib" CMOS_MODELS

.end
