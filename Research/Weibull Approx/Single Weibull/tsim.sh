echo "Starting Hspice Sim"

hspice tgate.sp > tgate.lis
#gedit singleapprox.lis &

#gedit singleapprox.mt* &
rm *.ic*
rm *.pa*

echo "Simulation Complete"
