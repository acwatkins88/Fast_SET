$Parallel and Series Stack Current Characterization

.param vdd = 1.0
vdd vdd 0 vdd

$ Parallel Transistor Simulation
.param vd = .838
.param vgs = 0

vd1 d 0 vd
vg1 g 0 vgs

$ Series Transistor Simulation

$.param vinpa = 0
$.param vinpb = 0

$vina ag 0 vinpa
$vinb bg 0 vinpb

$ Single Transistor Simulation

$MP1 p11 vdd vdd vdd CMOSP W=0.24U L=0.08U 
$MP2 p11 g vdd vdd CMOSP W=0.24U L=0.08U 
$MN1 p11 vdd cn1 0 CMOSN W=0.12U L=0.08U 
$MN2 cn1 g 0 0 CMOSN W=0.12U L=0.08U

mpo 0 g vdd vdd CMOSP W=0.24U L=0.08U
$mn1 out g 0 0 CMOSN W=0.12U L=0.08U

$ Parallel DC Simulation
.dc vg1 start=0 stop=1 step=.005 

$ Print Parallel Simulation
.PRINT DC I(mpo) 
$.PRINT DC I(MP2)

$ Series Transistor Simulation
$ NMOS
$msn1 vdd ag minpt 0 CMOSN W=0.12U L=0.08U
$msn2 minpt bg 0 0 CMOSN W=0.12U L=0.08U

$ Series DC Simulation
$.dc vinpa 0 1 .5 sweep vinpb 0 1 .5 

$ Print Series Simulation
$.PRINT DC V(ag) V(bg) I(msn1)

.option measform = 3
.option MEASDGT = 8
.option ingold = 1
.option post
 
.LIB "IBM96w90nm.lib" CMOS_MODELS
.lib "./Subcircuits/subcircuits.lib" std_cells

.END 
