#Usage: time bash test_script.sh

thetac=90
ntests=100 #50

testtype=fixedrs
rs=50
for thetas in 60 90 120
do
    for (( rc=10; rc<=100; rc+=10 ))
    do
        ./build/main $testtype $thetas $thetac $rs $rc $ntests >logs/out_$rc.txt 2> logs/err_$testtype_$thetas_$rs_$rc.txt &
    done
    wait
    echo
done
echo "Done Fixed RS"

testtype=fixedrc
rc=70
for thetas in 60 90 120
do
    for (( rs=10; rs<=100; rs+=10 ))
    do
        ./build/main $testtype $thetas $thetac $rs $rc $ntests >logs/out_$rc.txt 2> logs/err_$testtype_$thetas_$rs_$rc.txt &
    done
    wait
    echo
done
echo "Done Fixed RC"

echo "All done"
