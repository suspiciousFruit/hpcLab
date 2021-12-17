./run.sh
for ((i=1; i <= 4; i++))
do
    echo -n "processes: $i |time(sec): "
    srun -n $i -c 2 ./a.out
done