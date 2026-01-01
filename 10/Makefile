main1: main1.cpp ../helpers.cpp
	g++ -o main1 main1.cpp ../helpers.cpp

main2: main2.cpp ../helpers.cpp
	g++ -o main2 main2.cpp ../helpers.cpp -lz3

run1: main1
	./main1

run2: main2
	./main2

all: main1 main2

clean:
	rm -f main1 main2
