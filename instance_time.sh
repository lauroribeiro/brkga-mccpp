population_size=100
elite_size=0.2
mutatants_size=0.10
crossover_probability=0.70
restart_generations=50
restart_strategy=1

# read filenames
# make clean
# make
execute=false
for filename in experiment/*.txt; do
    if [ "$filename" == "experiment/37-latin_square_10.txt" ]; then
      execute=true
    fi
    echo 'Processing file: ' $filename
    if [ "$execute" == false ]; then
      continue
    fi
    result=$(./brkga-mccpp $filename $population_size $elite_size $mutatants_size $crossover_probability $restart_generations $restart_strategy 0 0)
    # time=$(echo $result | grep "BRKGA run finished in" | awk '{print $5}')
    # fitness=$(echo $result | grep "Best solution found has objective value" | awk '{print $4}')
    echo "$filename - $result" >> experiment_time.txt
  # done
done