echo "Starting Hspice Sim"

hspice singleapprox.sp > singleapprox.lis
#gedit singleapprox.lis &

#gedit singleapprox.mt* &
rm *.ic*
rm *.pa*

echo "Simulation Complete"
