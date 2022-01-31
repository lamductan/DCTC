#Usage: time bash test_script_2.sh > out.txt &> err.txt

./build/main > logs/out_$rc.txt 2> logs/err_$rc.txt &
echo "Done"

