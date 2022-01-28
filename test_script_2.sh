#Usage: time bash test_script_2.sh > out.txt &> err.txt

n_tests=1000

for (( rc=110; rc<=200; rc+=10 ))
do
    echo "r_c = $rc"
    ./build/prog $rc $n_tests > logs/out_$rc.txt 2> logs/err_$rc.txt &
done

wait
echo "All done"
