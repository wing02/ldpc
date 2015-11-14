IEEE_LDPC_OBJ= cl_create.o ieee_ldpc_main.o matrix.o ieee_ldpc_encode.o  ieee_ldpc_decode.o matrix_double.o
IEEE_LDPC_EXAMPLE_OBJ= cl_create.o ieee_ldpc_example.o matrix.o ieee_ldpc_encode.o  ieee_ldpc_decode.o matrix_double.o char2bool.o
TEST_OBJ=cl_create.o matrix.o test.o
LBS= -framework OpenCL 
#LBS=-l OpenCL -L/home/lagrange/AMDAPPSDK-3.0-0-Beta/lib/x86_64/
#CFLAGS=-I /home/lagrange/AMDAPPSDK-3.0-0-Beta/include/

ieee_ldpc_example:${IEEE_LDPC_EXAMPLE_OBJ}
	gcc -o $@ ${IEEE_LDPC_EXAMPLE_OBJ} ${LBS} 

ieee_ldpc_main:${IEEE_LDPC_OBJ}
	gcc -o $@ ${IEEE_LDPC_OBJ} ${LBS} 

test:${TEST_OBJ}
	gcc -o $@ ${TEST_OBJ} ${LBS}

clean:
	rm  test ${TEST_OBJ} ieee_ldpc_main ${IEEE_LDPC_OBJ}  ${IEEE_LDPC_EXAMPLE_OBJ}
