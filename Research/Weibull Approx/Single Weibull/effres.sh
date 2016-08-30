echo "Starting Hspice Sim"

hspice effresistance.sp > effresistance.lis
#gedit calcdelay.lis &

#gedit calcdelay.mt* &
rm *.ic*

echo "Simulation Complete"
