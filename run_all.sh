#!/bin/bash
# Advent of Code 2025 - Runner Script

DAYS=(01 02 03 04 05 06 07 08 09 10 11 12)

run_part1() {
    echo "========================================="
    echo "Running All Part 1 Solutions"
    echo "========================================="
    for day in "${DAYS[@]}"; do
        if [ -d "$day" ]; then
            echo ""
            echo "=== Day $day - Part 1 ==="
            cd "$day"
            make run1 2>/dev/null || echo "Day $day Part 1 not available"
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
            make run2 2>/dev/null || echo "Day $day Part 2 not available"
            cd ..
        fi
    done
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
            make run1 2>/dev/null || echo "Part 1 not available"
            make run2 2>/dev/null || echo "Part 2 not available"
            cd ..
        fi
    done
}

benchmark() {
    echo "========================================="
    echo "Benchmarking All Solutions"
    echo "========================================="
    for day in "${DAYS[@]}"; do
        if [ -d "$day" ]; then
            echo ""
            echo "=== Day $day ==="
            cd "$day"
            make run1 2>/dev/null || true
            make run2 2>/dev/null || true
            cd ..
        fi
    done
    echo ""
    echo "========================================="
}

# Default to running all if no argument
if [ $# -eq 0 ]; then
    run_part1
    echo ""
    run_part2
    exit 0
fi

case "$1" in
    part1|1)
        run_part1
        ;;
    part2|2)
        run_part2
        ;;
    all)
        run_part1
        echo ""
        run_part2
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
