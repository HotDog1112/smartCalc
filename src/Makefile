CC=gcc
CFLAGS= -std=c11 -pedantic 

SOURCES=*.c
OBJECTS=*.o

all: s21_calc_polish.a test

s21_calc_polish.a:
	$(CC) $(CFLAGS) -g -c $(SOURCES)
	ar rc s21_calc_polish.a $(OBJECTS)
	ranlib s21_calc_polish.a
	rm -f *.o

test: s21_calc_polish.a
	# checkmk clean_mode=1 tests.check > test_calc_polish.c
	$(CC) $(CFLAGS) -g -c test_calc_polish.c
	ar rc test_calc_polish.a test_calc_polish.o
	ranlib test_calc_polish.a
	$(CC) $(CFLAGS) -g --coverage -o test_cases s21_calc_polish.a test_calc_polish.a $(SOURCES) -lcheck -lm -pthread
	./test_cases

clean:
	rm -rf *.a *.o test_cases *.gcda *.gcno *.html *.css

gcov_report: test
	gcovr -r . --html --html-details -o report.html
	./test_cases
	rm *.gcda *.gcno *.a *.o
	open report.html



install:
	cd Calculator && cmake .
	cd Calculator && make
	cd Calculator/Calculator.app/Contents/MacOS && ./Calculator

uninstall:
	cd Calculator && make clean
	rm -rf Calculator/CMakeCache.txt Calculator/Calculator.app


dist:
	cd .. && tar -czf s21_SmartCalc_v1.0.tgz src


dvi: dvi_clean
	makeinfo manual.texi --html
	open ./manual/index.html

dvi_clean:
	rm -rf manual

clean_cash:
	sh cleaner.sh
	sh cleaner_2.sh
