echo "Starting Hspice Sim"

hspice norapprox.sp > norapprox.lis

rm *.ic*
rm *.pa*

echo "Simulation Complete"
