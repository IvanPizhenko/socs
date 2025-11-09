TDF=data-1m.txt

CFLAGS+=-std=c99 -O3 -Wall -Werror -Wextra

all: soc6 rndnf

clean:
	-rm -f soc6
	-rm -f rndnf

soc6: soc6.c

rndnf: rndnf.c

$(TDF): rndnf
	./rndnf 1000000 999 time $(TDF)
	wc -l $(TDF)

100_random_numbers.txt: 100_random_numbers.txt.bz2
	bzip2 -dk 100_random_numbers.txt.bz2

10000_random_numbers.txt: 10000_random_numbers.txt.bz2
	bzip2 -dk 10000_random_numbers.txt.bz2

1M_random_numbers.txt: 1M_random_numbers.txt.bz2
	bzip2 -dk 1M_random_numbers.txt.bz2

test: soc6 $(TDF)
	time ./soc6 $(TDF)

test2: soc6 rndnf
	./rndnf 1000000 999 time $(TDF)
	time ./soc6 $(TDF)

test100: soc6 100_random_numbers.txt
	time ./soc6 100_random_numbers.txt

test10k: soc6 10000_random_numbers.txt
	time ./soc6 10000_random_numbers.txt

test1m: soc6 1M_random_numbers.txt
	time ./soc6 1M_random_numbers.txt
