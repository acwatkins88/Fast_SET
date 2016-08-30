echo "Starting Hspice Sim"

hspice threshtest.sp > threshtest.lis

rm *.ic*
rm *.pa*

echo "Simulation Complete"
