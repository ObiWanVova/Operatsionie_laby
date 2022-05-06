echo Enter name:
read var
echo you enter $var
for k in *.txt 
do 
    mkdir -p $var/$k
    mv $k $var/$k
done
