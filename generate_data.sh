OUTPUT_DIR="data/macbook_run_04"
POW_MIN="3"
POW_MAX="5"
# BASES=("1" "3" "5" "7" "11" "13");
BASES=("64")
TYPES=("0" "1")
FACTORS=("0.5")
N_TRIALS="2"
CILK_WORKERS=8

~/installed_software/opencilk-2.1.0-arm64-apple-darwin21.6.0/bin/clang \
	-fopencilk \
	-std=c11 \
	-I /Users/russell/installed_software/opencilk-2.1.0-arm64-apple-darwin21.6.0/lib/clang/16/include/cilk \
	-O3 \
	-Wall \
	mem_sched_demo.c \
	-o mem_sched_demo

for power in `seq $POW_MIN $POW_MAX`; do
  for base in "${BASES[@]}"; do
    for type in "${TYPES[@]}"; do
      echo "Running power ${power}, base ${base}, type: ${type} ..."
      timings_file_base="${OUTPUT_DIR}/p${power}_b${base}_t${type}"
      timings_file_final="${timings_file_base}.txt"
      timings_file_aside="${timings_file_base}.aside"
      if [ -e "${timings_file_final}" ]; then
        echo "  ... already completed"
      else
        ./mem_sched_demo \
          "${timings_file_aside}" \
          "${base}" "${power}" "max" "${N_TRIALS}" "${type}" \
        && mv "${timings_file_aside}" "${timings_file_final}"
      fi
    done
    for factor in "${FACTORS[@]}"; do
      type="2"
      echo "Running power ${power}, base ${base}, type: ${type}, factor: ${factor} ..."
      timings_file_base="${OUTPUT_DIR}/p${power}_b${base}_t${type}_f${factor}"
      timings_file_final="${timings_file_base}.txt"
      timings_file_aside="${timings_file_base}.aside"
      if [ -e "${timings_file_final}" ]; then
        echo "  ... already completed"
      else
        ./mem_sched_demo \
          "${timings_file_aside}" \
          "${base}" "${power}" "${factor}" "${N_TRIALS}" "${type}" \
        && mv "${timings_file_aside}" "${timings_file_final}"
      fi
    done
  done
done
