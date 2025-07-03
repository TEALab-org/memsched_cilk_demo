OUTPUT_DIR="data/macbook_run_01"
POW_MIN="5"
POW_MAX="9"
BASES=("1" "3" "5" "7" "11", "13");
TYPES=("0" "1");
N_TRIALS="5"
CILK_WORKERS=8

~/installed_software/opencilk-2.1.0-arm64-apple-darwin21.6.0/bin/clang \
	-fopencilk \
	-std=c11 \
	-I /Users/russell/installed_software/opencilk-2.1.0-arm64-apple-darwin21.6.0/lib/clang/16/include/cilk \
	-03 \
	-Wall \
	mem_sched_demo.c \
	-o mem_sched_demo

for power in `seq $POW_MIN $POW_MAX`; do
  for base in "${BASES[@]}"; do
    for type in "${TYPES[@]}"; do
      echo "Running power ${power}, base ${base}, type: ${type} ..."
      timings_file="${OUTPUT_DIR}/p${power}_b${base}_t${type}.txt"
      ./mem_sched_demo \
        "${timings_file}" \
        "${base}" "${power}" "max" "${N_TRIALS}" "${type}" 
    done
  done
done
