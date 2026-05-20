TIMEFORMAT="real %R"
echo "" > benchmark_kap.txt
for i in {3..11}; do
    if ((i % 2 == 0)); then
        continue
    fi
    for j in {1..5}; do
        echo "r=$i input_$j" >> benchmark_kap.txt
        { time ./med_of_med $i "../../part4-analysis/inputs/input_$j.txt";} >> benchmark_kap.txt 2>&1
        echo "" >> benchmark_kap.txt
    done
    echo "================================" >> benchmark_kap.txt
done