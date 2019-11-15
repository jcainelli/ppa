#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int rank, size;
	
	printf("Hello World!");

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	printf("Hello World! I'm %d of %d\n",rank,size);
	
	int j = 9;
	for (int i = 0; i< 1000; i++) {
		j = i*j;
	}

	MPI_Finalize();
	return 0;
}

