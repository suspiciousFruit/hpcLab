for ((i=1; i <= 15; i++))
do
    echo -n "threads: $i |time: "
    srun -c $i ./a.out
done