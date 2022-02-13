#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<time.h>

int **matrixA, **matrixB, **matrixC;
int A_row,A_column,B_row,B_column,C_row,C_column;

struct Index
{
    int rowIndex;
    int columnIndex;
};


void readinput()   //read input file and get 2 matrices
{
    char filename[256];
    puts("Enter file name: ");
    puts("----------------------------------");
    fgets(filename, 260, stdin);

    filename[strlen(filename)-1]='\0';
    if(fopen(filename,"r"))
    {
        FILE *fpointer = fopen(filename,"r");


        fscanf(fpointer,"%d %d", &A_row,&A_column);
        matrixA=(int**)malloc(A_row*sizeof(int*));
        for (int i = 0; i < A_row; i++)
        {
            matrixA[i] = (int*)malloc(A_column* sizeof(int));
        }
        for(int i=0; i<A_row; i++)
        {
            for(int j=0; j<A_column; j++)
                fscanf(fpointer,"%d ",&matrixA[i][j]);

            fscanf(fpointer,"\n");
        }


        fscanf(fpointer,"%d %d", &B_row,&B_column);
        matrixB=(int**)malloc(B_row*sizeof(int*));
        for (int i = 0; i < B_row; i++)
        {
            matrixB[i] = (int*)malloc(B_column* sizeof(int));
        }
        for(int i=0; i<B_row; i++)
        {
            for(int j=0; j<B_column; j++)
                fscanf(fpointer,"%d ",&matrixB[i][j]);

            fscanf(fpointer,"\n");
        }

        fclose(fpointer);
    }
    else
    {
        puts("File Not Found!");
        puts("----------------------------------");
    }
}



void *mult_row(void *arg)
{

    struct Index *index=(struct Index*) arg;
    int RowIndex = index->rowIndex;

    for (int i = 0; i < C_column; i++)
    {
        int result = 0;
        for (int j = 0; j < A_column; j++)
            result += matrixA[RowIndex][j] * matrixB[j][i];

        matrixC[RowIndex][i] = result;
    }
}

void byRow()
{
    pthread_t thread[C_row];
    struct Index *index = malloc(sizeof (struct Index) * C_row);

    for (int i = 0; i < A_row; i++)
    {
        index[i].rowIndex= i;
        pthread_create(&thread[i],NULL,&mult_row,&index[i]);  //create thread according to num of rows
    }

    for(int i=0; i<C_row; i++)
         pthread_join(thread[i],NULL);  //terminate thread
}


void *mult_element(void*arg)
{
    struct Index *index=(struct Index*) arg;
    int RowIndex = index->rowIndex;
    int ColumnIndex = index->columnIndex;
    int result = 0;
    for (int i = 0; i < A_column; i++)
        result += matrixA[RowIndex][i] * matrixB[i][ColumnIndex];

    matrixC[RowIndex][ColumnIndex] = result;
}


void byElement()
{
    int numberofelements=0;
    pthread_t thread[C_row*C_column];
    struct Index *index = malloc(sizeof (struct Index) * C_row * C_column);
    for (int i = 0; i < A_row; i++)
    {
        for (int j = 0; j < B_column; j++)
        {
            index[numberofelements].rowIndex= i;
            index[numberofelements].columnIndex= j;
            pthread_create(&thread[numberofelements],NULL,&mult_element,&index[numberofelements]);  //create threads according to number of elements
            numberofelements++;
        }
    }

    for(int i=0; i<C_row*C_column; i++)
         pthread_join(thread[i],NULL);
}


int main()
{
    readinput();

    if (A_column!=B_row)   //elements don't match and multiplication not possible
    {
        printf("Multiplication of Matrix is not Possible !!\n");
        return 0 ;
    }

    C_row=A_row;
    C_column=B_column;
    matrixC=(int**)malloc(C_row*sizeof(int*));  //allocate result matrix
    for (int i = 0; i < C_row; i++)
    {
        matrixC[i] = (int*)malloc(C_column* sizeof(int));
    }

    start=clock();
    byRow();
    end=clock();
    elapsedtime_row=(double)(end-start)/CLOCKS_PER_SEC;  //get time

    fprintf(fpointer,"By Row\n");   //write in file
    for (int i = 0; i < C_row; i++)
    {
        for (int j = 0; j < C_column; j++)
             fprintf(fpointer,"%d ", matrixC[i][j]);

        fprintf(fpointer,"\n");
    }
    fprintf(fpointer,"Time : %f s\n",elapsedtime_row);

        FILE *fpointer = freopen("output.txt","w",stdout);  //open output file and clear
    clock_t start,end;
    double elapsedtime_element, elapsedtime_row;

    start=clock();
    byElement();
    end=clock();
    elapsedtime_element=(double)(end-start)/CLOCKS_PER_SEC;  //get time

    fprintf(fpointer,"By Element\n");   //write in file
    for (int i = 0; i < C_row; i++)
    {
        for (int j = 0; j < C_column; j++)
            fprintf(fpointer,"%d ", matrixC[i][j]);

        fprintf(fpointer,"\n");
    }
    fprintf(fpointer,"Time : %f s\n",elapsedtime_element);

    fclose(fpointer);

    return 0;
}
