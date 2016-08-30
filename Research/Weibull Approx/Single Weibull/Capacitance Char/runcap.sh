echo "Starting Hspice Sim"

hspice capchar.sp > capchar.lis

echo "Enter File Name"
read filename

mv capchar.mt0 "$filename".res

rm *.ic*

echo "Simulation Complete"
