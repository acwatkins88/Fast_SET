echo "Starting Hspice Sim"

hspice currentcalc.sp > currentcalc.lis
#gedit singleapprox.lis &

#gedit singleapprox.mt* &
cp currentcalc.lis ./Results\ Parser

rm *.ic*
rm *.pa*

echo "Running Parser"
cd Results\ Parser/
sh runparse.sh
cd ..

echo "Simulation Complete"

