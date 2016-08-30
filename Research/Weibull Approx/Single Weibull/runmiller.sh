echo "Starting Hspice Sim"

hspice millerchar.sp > millerchar.lis

rm *.ic*
rm *.pa*

echo "Simulation Complete"
