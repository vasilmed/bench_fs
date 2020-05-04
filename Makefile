all: bench_fs
bench_fs: error.o bench_fs.o
	gcc error.o bench_fs.o -o bench_fs
error.o: error.c
	gcc -c error.c
bench_fs.o: bench_fs.c
		gcc -c bench_fs.c
clean:
	rm -rf *.o bench_fs
