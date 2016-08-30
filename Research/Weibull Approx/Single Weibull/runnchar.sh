echo "Starting Hspice Sim"

hspice nodechar.sp > nodechar.lis

rm *.ic*
rm *.pa*

echo "Simulation Complete"
