#!/bin/bash
population_size=100
elite_size=0.2
mutatants_size=0.10
crossover_probability=0.70
restart_generations=50
restart_strategy=1
# training
# times=(0.056 0.158 0.220 0.213 0.548 0.747 0.528 0.413 0.965 1.286 2.033 0.940 0.929 2.023 2.506 2.184 14.690 89.926 165.218 744.487)
#experiment
times=(194.656 77.2497 70.1256 130.052 164.975 305.704 341.349 954.066 1485.79 1552.44 231.292 25.1253 36.7734 29.8114 45.186 47.1609 1228.73 1407.8 1383.37 1359.19 241.837 1381.53 109.134 99.909 125.741 641.406 229.205 576.442 2056.91 2156.84 38.6962 51.4102 13.7692 28.7786 37.966 38.2239 57.6022 58.614 33.0936 32.3872 60.2067 60.0646 31.4412 10.8244 43.0273 18.3757 219.218 163.616 1659.88 1659.37)
# best_values=(3097 2068 3960 695 2212 2590 832 914 2307 2819 1657 3177 1967 4351 1081 1137 1941 5848 1319 10587)
# Compile the C++ program using make
file_index=0
# make clean
# make
# read filenames
headers="filename,population_size,elite_size,mutatants_size,crossover_probability,restart_generations,restart_strategy,seed,objective_value,time_elapsed,stop_time"
echo -e $headers >> exp_results.csv

execute=false
for filename in experiment/*.txt; do


  echo 'Processing file: ' $filename
  stop_time=${times[$file_index]}
  # best_value=${best_values[$file_index]}
  file_index=$((file_index+1))


  # Execute the program 10 times
  for ((seed=0; seed<10; seed++)) do
    if [ "$filename" == "experiment/70-C1000.9.txt" ] && [ "$seed" -eq 8 ]; then
      execute=true
      echo 'vou executar'
    fi
    if [ "$execute" == false ]; then
      continue
    fi
    trap 'echo "filename: $filename, seed: $seed, stop_time: $stop_time" >> trapped_args; exit 0' SIGINT
    result=$(./brkga-mccpp $filename $population_size $elite_size $mutatants_size $crossover_probability $restart_generations $restart_strategy $seed $stop_time)
    echo "$filename,$population_size,$elite_size,$mutatants_size,$crossover_probability,$restart_generations,$restart_strategy,$seed,$result,$stop_time" >> exp_results.csv
  done
done

# ./brkga-mccpp Instances/14-games120.txt 200 0.25 0.10 0.70 50 1 0 5.34 4351
# training/20-DSJC1000.1.txt 100 0.25 0.10 0.70 50 1 0 13583 744.716 744.487 10587
# training/20-DSJC1000.1.txt 100 0.25 0.10 0.70 100 1 0 13552 745.174 744.487 10587
# ./brkga-mccpp training/20-DSJC1000.1.txt 100 0.25 0.10 0.70 50 1 0 744.487 10587
# ./brkga-mccpp training/20-DSJC1000.1.txt 100 0.25 0.10 0.70 100 1 0 744.487 10587