.LIB "IBM96w90nm.lib" CMOS_MODELS

.option ingold = 1
.OPTION MEASFILE=1
.option post
.option captab

.param cap2 = 100f

VDD vdd 0 1

IP1 z gnd PWL(time, pm1)
$IP2 gnd z PWL(time, pm2)
$IN1 gnd z PWL(time, nm1)
CLOADA z gnd cap2

.data setsrce MER
FILE='testpulse.dat' time=1 pm1=2 pm2=3 nm1=4
.enddata

.tran data=setsrce 

$.print V(z) I(IP1) I(IP2) I(IN1) I(CLOADA)
.print V(z) I(IP1)
.END
