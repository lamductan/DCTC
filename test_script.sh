#Usage: time bash test_script.sh
ntests=100 #50
echo "Run $ntests tests"

## Create directories
makedirs() {
    p=""
    for str in "$@"
    do
        p+=$str
        if [ ! -d $p ];
        then
            mkdir $p 
        fi
        p+='/'
    done
}

timestamp=`date +"%Y-%m-%d-%H-%M-%S"`
echo "timestamp = $timestamp"

makedirs "results" $timestamp
makedirs "logs" $timestamp
makedirs "data" $timestamp


## Perform simulations
thetac=90

testtype=fixed_rs
rs=50
for thetas in 60 90 120
do
    for (( rc=10; rc<=100; rc+=10 ))
    do
        command="./build/main $testtype $thetas $thetac $rs $rc $ntests $timestamp" 
        filename="${testtype}_${thetas}_${rs}_${rc}.txt"
        outfile="logs/$timestamp/out_$filename"
        errfile="logs/$timestamp/err_$filename"
        echo $command $outfile $errfile
        $command >$outfile 2>$errfile &
    done
    wait
done
echo "Done Fixed RS. Results are saved in results/$timestamp/fixed_rs.csv"

testtype=fixed_rc
rc=70
for thetas in 60 90 120
do
    for (( rs=10; rs<=100; rs+=10 ))
    do
        command="./build/main $testtype $thetas $thetac $rs $rc $ntests $timestamp" 
        filename="${testtype}_${thetas}_${rs}_${rc}.txt"
        outfile="logs/$timestamp/out_$filename"
        errfile="logs/$timestamp/err_$filename"
        echo $command $outfile $errfile
        $command >$outfile 2>$errfile &
    done
    wait
done
echo "Done Fixed RC. Results are saved in results/$timestamp/results_all_fixed_rc.csv"

## Merge results
python3 merge_results.py $timestamp

echo "Done Fixed RS. Results are saved in results/$timestamp/fixed_rs.csv"
echo "Done Fixed RC. Results are saved in results/$timestamp/results_all_fixed_rc.csv"
echo "All done"
