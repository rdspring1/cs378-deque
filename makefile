all:
	make Deque.zip

clean:
	rm -f Deque.log
	rm -f Deque.zip
	rm -f TestDeque
	rm -f TestDeque1
	rm -f TestDeque2
	rm -f TestDeque3

doc: Deque.h
	doxygen Doxyfile

turnin-list:
	turnin --list dlessin cs378pj4

turnin-submit: Deque.zip
	turnin --submit dlessin cs378pj4 Deque.zip

turnin-verify:
	turnin --verify dlessin cs378pj4

Deque.log:
	git log > Deque.log

Deque.zip: Deque.h Deque.log TestDeque.c++ TestDeque.out
	zip -r Deque.zip html/ Deque.h Deque.log TestDeque.c++ TestDeque.out

TestDeque: Deque.h TestDeque.c++
	g++ -pedantic -std=c++0x -Wall TestDeque.c++ -o TestDeque -lgtest -lpthread -lgtest_main

TestDeque1: Deque.h tsm544-TestDeque.c++
	g++ -pedantic -std=c++0x -Wall tsm544-TestDeque.c++ -o TestDeque1 -lgtest -lpthread -lgtest_main

TestDeque2: Deque.h tsm544-TestDeque.c++
	g++ -pedantic -std=c++0x -Wall davismc-TestDeque.c++ -o TestDeque2 -lgtest -lpthread -lgtest_main
    
TestDeque3: Deque.h aashishs-TestDeque.c++
	g++ -pedantic -std=c++0x -Wall aashishs-TestDeque.c++ -o TestDeque3 -lgtest -lpthread -lgtest_main

TestDeque.out: TestDeque
	valgrind ./TestDeque > TestDeque.out
