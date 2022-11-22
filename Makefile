CC=g++ -std=c++20 -Wall
run:
	$(CC) src/*.cpp -o build/simulator
clean:
	rm a.* *.gch