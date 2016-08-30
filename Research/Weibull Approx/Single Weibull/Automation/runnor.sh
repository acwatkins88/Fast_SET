echo "Starting Hspice Sim"
rm *.tr*

hspice norapprox.sp > norapprox.lis

rm *.ic*
rm *.pa*

echo "Simulation Complete"
