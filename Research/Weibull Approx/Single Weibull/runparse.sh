echo "Enter Results File Name: "
read filename
#rm ./*.res

./resparse ./singleapprox.lis ./"$filename".res

#rm  ./*.o
#rm  ./*.~
