echo "Starting Hspice Sim"

hspice capchar.sp > capchar.lis

echo "Enter File Name"
read filename

mv capchar.mt0 Results\ Parser/"$filename".res

rm *.ic*
rm *.pa*

echo "Simulation Complete"
