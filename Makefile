# Advent of Code 2025 - Master Makefile

DAYS := 01 02 03 04 05 06 07 08 09 10 11 12
START ?= 01
END ?= 12

.PHONY: run-all-part1 run-all-part2 run-all benchmark run-range help

# Run all Part 1 solutions
run-all-part1:
@echo "========================================="
@echo "Running All Part 1 Solutions"
@echo "========================================="
@for day in $(DAYS); do \
echo ""; \
echo "=== Day $$day - Part 1 ==="; \
if [ -d $$day ]; then \
cd $$day && $(MAKE) run1 2>/dev/null || echo "Day $$day Part 1 not available"; \
cd ..; \
fi; \
done

# Run all Part 2 solutions
run-all-part2:
@echo "========================================="
@echo "Running All Part 2 Solutions"
@echo "========================================="
@for day in $(DAYS); do \
echo ""; \
echo "=== Day $$day - Part 2 ==="; \
if [ -d $$day ]; then \
cd $$day && $(MAKE) run2 2>/dev/null || echo "Day $$day Part 2 not available"; \
cd ..; \
fi; \
done

# Run everything (Part 1 and Part 2 for all days)
run-all:
@$(MAKE) run-all-part1
@echo ""
@echo "========================================="
@echo ""
@$(MAKE) run-all-part2

# Run a specific range of days (both parts)
# Usage: make run-range START=3 END=7
run-range:
@echo "========================================="
@echo "Running Days $(START) to $(END)"
@echo "========================================="
@for i in $$(seq -f "%02g" $(START) $(END)); do \
echo ""; \
echo "=== Day $$i - Part 1 ==="; \
if [ -d $$i ]; then \
cd $$i && $(MAKE) run1 2>/dev/null || echo "Day $$i Part 1 not available"; \
cd ..; \
fi; \
echo ""; \
echo "=== Day $$i - Part 2 ==="; \
if [ -d $$i ]; then \
cd $$i && $(MAKE) run2 2>/dev/null || echo "Day $$i Part 2 not available"; \
cd ..; \
fi; \
done

# Benchmark - time all solutions
benchmark:
@echo "========================================="
@echo "Benchmarking All Solutions"
@echo "========================================="
@for day in $(DAYS); do \
if [ -d $$day ]; then \
echo ""; \
echo "=== Day $$day ==="; \
cd $$day; \
$(MAKE) run1 2>/dev/null || true; \
$(MAKE) run2 2>/dev/null || true; \
cd ..; \
fi; \
done; \
echo ""; \
echo "========================================="

# Build all solutions
build-all:
@echo "Building all solutions..."
@for day in $(DAYS); do \
if [ -d $$day ]; then \
echo "Building Day $$day..."; \
cd $$day && $(MAKE) all 2>/dev/null || echo "Day $$day build failed"; \
cd ..; \
fi; \
done

# Clean all compiled files
clean-all:
@echo "Cleaning all compiled files..."
@for day in $(DAYS); do \
if [ -d $$day ]; then \
cd $$day && $(MAKE) clean 2>/dev/null || true; \
cd ..; \
fi; \
done

help:
@echo "Advent of Code 2025 - Available Commands:"
@echo ""
@echo "  make run-all-part1    - Run all Part 1 solutions"
@echo "  make run-all-part2    - Run all Part 2 solutions"
@echo "  make run-all          - Run all solutions (Part 1 & 2)"
@echo "  make run-range        - Run specific days (use START=X END=Y)"
@echo "  make benchmark        - Time all solutions"
@echo "  make build-all        - Compile all solutions"
@echo "  make clean-all        - Remove all compiled files"
@echo ""
@echo "Examples:"
@echo "  make run-range START=3 END=7"
@echo "  make benchmark"
