echo "Starting Hspice Sim"

hspice calcdelay.sp > calcdelay.lis
#gedit calcdelay.lis &

#gedit calcdelay.mt* &
rm *.ic*
rm *.pa*
#rm calcdelay.tr*

echo "Simulation Complete"
