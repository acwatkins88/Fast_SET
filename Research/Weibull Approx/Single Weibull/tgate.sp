$ Calculate delay of gate

.param vdd = 1.05

$.param rtime = .001f
$.param ftime = .001f

$ Options
.option ingold = 1
.option post

VDD vdd 0 vdd

VUP a gnd PWL(time, vol1)

VUP1 b gnd PWL(time, vol2)

VINC c 0 1V

$VINB b 0 vdd
 
$ Pulse 1 2 input
$xand1 a b vdd p9 nand2inp

$ Pulse 1 3 input
xand1 a b c vdd p9 nand3inp

.data setsrce MER
FILE='t_input.dat' time=1 vol1=2 vol2=3
.enddata

.tran data=setsrce

$.PRINT TRAN I(IUP) V(z) V(p) V(output) 
$.LIB "IBM96w90nm.lib" CMOS_MODELS

$.LIB "IBM65nm.lib" CMOS_MODELS
$.LIB "./Subcircuits/subcircuits65nm.lib" std_cells

$.LIB "45nm_HP.lib" CMOS_MODELS
$.LIB "./Subcircuits/subcircuits45nm.lib" std_cells

.LIB "32nm_HP.lib" CMOS_MODELS
.lib "./Subcircuits/subcircuits32nm.lib" std_cells
.END 
