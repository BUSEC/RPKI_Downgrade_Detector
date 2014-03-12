CC=g++ 
CFLAGS=-Wall -pthread -std=c++11 -pedantic -lstdc++ -static -static-libgcc -static-libstdc++ -g 
TEST_INCLUDE=-I/opt/gtest-1.6.0/include
TEST_FILES=/opt/gtest-1.6.0/src/gtest_main.cc /opt/libgtest.a

all: 
	$(CC) $(CFLAGS) -o detector.out cli.cpp

clean:
	rm exp.o detector.out unittests.out

test_cli: all 
	./detector.out test_files/old.txt test_files/new.txt test_files/output.txt

#See instructions here to setup our c++ test environment: https://github.com/BUSEC/rPKI/wiki/Setting-up-our-Test-Environment-for-C-plus-plus
compile_test: exp.o
	$(CC) $(CFLAGS) $(TEST_INCLUDE) $(TEST_FILES) -o unittests.out unittests.cpp

test: compile_test
	./unittests.out

val_test: compile_test
	valgrind ./unittests.out

exp.o:
	$(CC) $(CFLAGS) -c exp.cpp
