echo "Enter Results File Name: "
read filename
#rm ./*.res

./resparse ./currentcalc.lis ./"$filename".res

#rm  ./*.o
#rm  ./*.~