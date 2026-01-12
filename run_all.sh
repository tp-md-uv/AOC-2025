#!/bin/bash
# Advent of Code 2025 - Runner Script

DAYS=(01 02 03 04 05 06 07 08 09 10 11 12)

# Arrays to store timing results
declare -A PART1_TIMES
declare -A PART2_TIMES

extract_time() {
    # Extract time from output (looks for "Time: X ms" or similar patterns)
    local output="$1"
    local time=$(echo "$output" | grep -oP '(?<=Time: )\d+(?= ms)' | tail -1)
    if [ -z "$time" ]; then
        # Try alternative format: "X ms"
        time=$(echo "$output" | grep -oP '\d+(?= ms)' | tail -1)
    fi
    echo "${time:-N/A}"
}

run_part1() {
    echo "========================================="
    echo "Running All Part 1 Solutions"
    echo "========================================="
    for day in "${DAYS[@]}"; do
        if [ -d "$day" ]; then
            echo ""
            echo "=== Day $day - Part 1 ==="
            cd "$day"
            output=$(make run1 2>/dev/null)
            if [ $? -eq 0 ]; then
                echo "$output"
                PART1_TIMES[$day]=$(extract_time "$output")
            else
                echo "Day $day Part 1 not available"
                PART1_TIMES[$day]="N/A"
            fi
            cd ..
        fi
    done
}

run_part2() {
    echo "========================================="
    echo "Running All Part 2 Solutions"
    echo "========================================="
    for day in "${DAYS[@]}"; do
        if [ -d "$day" ]; then
            echo ""
            echo "=== Day $day - Part 2 ==="
            cd "$day"
            output=$(make run2 2>/dev/null)
            if [ $? -eq 0 ]; then
                echo "$output"
                PART2_TIMES[$day]=$(extract_time "$output")
            else
                echo "Day $day Part 2 not available"
                PART2_TIMES[$day]="N/A"
            fi
            cd ..
        fi
    done
}

print_summary() {
    echo ""
    echo "========================================="
    echo "TIMING SUMMARY"
    echo "========================================="
    printf "%-6s %-15s %-15s %-15s\n" "Day" "Part 1 (ms)" "Part 2 (ms)" "Total (ms)"
    echo "---------------------------------------------------------"
    
    local total_p1=0
    local total_p2=0
    local count_p1=0
    local count_p2=0
    
    for day in "${DAYS[@]}"; do
        local p1="${PART1_TIMES[$day]:-N/A}"
        local p2="${PART2_TIMES[$day]:-N/A}"
        local total="N/A"
        
        if [[ "$p1" != "N/A" && "$p2" != "N/A" ]]; then
            total=$((p1 + p2))
        elif [[ "$p1" != "N/A" ]]; then
            total="$p1"
        elif [[ "$p2" != "N/A" ]]; then
            total="$p2"
        fi
        
        printf "%-6s %-15s %-15s %-15s\n" "$day" "$p1" "$p2" "$total"
        
        if [[ "$p1" != "N/A" ]]; then
            total_p1=$((total_p1 + p1))
            count_p1=$((count_p1 + 1))
        fi
        if [[ "$p2" != "N/A" ]]; then
            total_p2=$((total_p2 + p2))
            count_p2=$((count_p2 + 1))
        fi
    done
    
    echo "---------------------------------------------------------"
    printf "%-6s %-15s %-15s %-15s\n" "TOTAL" "$total_p1" "$total_p2" "$((total_p1 + total_p2))"
    
    if [ $count_p1 -gt 0 ]; then
        printf "%-6s %-15s\n" "AVG P1" "$((total_p1 / count_p1))"
    fi
    if [ $count_p2 -gt 0 ]; then
        printf "%-6s %-15s\n" "AVG P2" "$((total_p2 / count_p2))"
    fi
    echo "========================================="
}

run_range() {
    local start=$1
    local end=$2
    echo "========================================="
    echo "Running Days $start to $end"
    echo "========================================="
    for i in $(seq -f "%02g" "$start" "$end"); do
        if [ -d "$i" ]; then
            echo ""
            echo "=== Day $i ==="
            cd "$i"
            output=$(make run1 2>/dev/null)
            if [ $? -eq 0 ]; then
                echo "$output"
                PART1_TIMES[$i]=$(extract_time "$output")
            else
                echo "Part 1 not available"
                PART1_TIMES[$i]="N/A"
            fi
            
            output=$(make run2 2>/dev/null)
            if [ $? -eq 0 ]; then
                echo "$output"
                PART2_TIMES[$i]=$(extract_time "$output")
            else
                echo "Part 2 not available"
                PART2_TIMES[$i]="N/A"
            fi
            cd ..
        fi
    done
    print_summary
}

benchmark() {
    echo "========================================="
    echo "Benchmarking All Solutions"
    echo "========================================="
    run_part1
    echo ""
    run_part2
    print_summary
}

# Default to running all if no argument
if [ $# -eq 0 ]; then
    run_part1
    echo ""
    run_part2
    print_summary
    exit 0
fi

case "$1" in
    part1|1)
        run_part1
        print_summary
        ;;
    part2|2)
        run_part2
        print_summary
        ;;
    all)
        run_part1
        echo ""
        run_part2
        print_summary
        ;;
    range)
        if [ -z "$2" ] || [ -z "$3" ]; then
            echo "Usage: $0 range START END"
            exit 1
        fi
        run_range "$2" "$3"
        ;;
    benchmark|bench)
        benchmark
        ;;
    *)
        echo "Usage: $0 [part1|part2|all|range START END|benchmark]"
        echo ""
        echo "Examples:"
        echo "  $0              # Run all solutions (default)"
        echo "  $0 part1        # Run all Part 1 solutions"
        echo "  $0 part2        # Run all Part 2 solutions"
        echo "  $0 range 3 7    # Run days 3-7"
        echo "  $0 benchmark    # Time all solutions"
        exit 1
        ;;
esac