all: api_test

api_test:
	clang api_test.c -o api_test.out -I../lib/src/clacks-api/ ../build/lib/libclacks.so

id_benchmark:
	clang id_gen_benchmark.c -o id_gen_benchmark.out -I../lib/src/clacks-api ../build/lib/libclacks.so
