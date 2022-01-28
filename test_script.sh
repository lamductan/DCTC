#Usage: time bash test_script.sh

thetac=90
ntests=100 #50

testtype=fixed_rs
rs=50
for thetas in 60 90 120
do
    for (( rc=10; rc<=100; rc+=10 ))
    do
        command="./build/main $testtype $thetas $thetac $rs $rc $ntests" 
        filename="${testtype}_${thetas}_${rs}_${rc}.txt"
        outfile="logs/out_$filename"
        errfile="logs/err_$filename"
        echo $command $outfile $errfile
        $command >$outfile 2>$errfile &
    done
    wait
done
echo "Done Fixed RS"

testtype=fixed_rc
rc=70
for thetas in 60 90 120
do
    for (( rs=10; rs<=100; rs+=10 ))
    do
        command="./build/main $testtype $thetas $thetac $rs $rc $ntests" 
        filename="${testtype}_${thetas}_${rs}_${rc}.txt"
        outfile="logs/out_$filename"
        errfile="logs/err_$filename"
        echo $command $outfile $errfile
        $command >$outfile 2>$errfile &
    done
    wait
done
echo "Done Fixed RC"

python3 merge_results.py
echo "All done"
