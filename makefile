IEEE_LDPC_EXAMPLE_OBJ= cl_create.o ieee_ldpc_example.o matrix.o ieee_ldpc_encode.o  ieee_ldpc_decode.o matrix_double.o char2bool.o coder.o
TEST_OBJ=cl_create.o matrix.o test.o
SET_OBJ= cl_create.o create_set.o matrix.o ieee_ldpc_encode.o  ieee_ldpc_decode.o matrix_double.o char2bool.o coder.o
LBS= -framework OpenCL 
#LBS=-l OpenCL -L/home/lagrange/AMDAPPSDK-3.0-0-Beta/lib/x86_64/
#CFLAGS=-I /home/lagrange/AMDAPPSDK-3.0-0-Beta/include/

ieee_ldpc_example:${IEEE_LDPC_EXAMPLE_OBJ}
	gcc -o $@ ${IEEE_LDPC_EXAMPLE_OBJ} ${LBS} 


create_set:${SET_OBJ}
	gcc -o $@ ${SET_OBJ} ${LBS} 

test:${TEST_OBJ}
	gcc -o $@ ${TEST_OBJ} ${LBS}

clean:
	-rm  create_set test ${TEST_OBJ} ieee_ldpc_example   ${IEEE_LDPC_EXAMPLE_OBJ}
