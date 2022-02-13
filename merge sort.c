#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int numberofelements;
int *numbers;

typedef struct
{   int left;
    int right;
} argument;



void merge(int start, int mid, int end) //merge two sub arrays
{
    int n1= mid-start+1;  //no of elements
    int n2= end-mid;
    int L[n1];           //initializing size of first sub array
    int R[n2];           //initializing size of second sub array

    for(int i=0; i<n1; i++)
    {
        L[i]=numbers[start+i];
    }
    for(int j=0; j<n2; j++)
    {
        R[j]=numbers[mid+j+1];
    }

    int i = 0, j = 0;
    int k = start;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            numbers[k] = L[i];
            i++;
        }
        else
        {
            numbers[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)       //Copy remaining elements of L[] if any
    {
        numbers[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)       //Copy remaining elements of R[] if any
    {
        numbers[k] = R[j];
        j++;
        k++;
    }
}



void *mergeSort(void*a)
{

    argument *arg =(argument* )a;

    int l = arg->left;
    int r = arg->right;

    pthread_t thread1,thread2;

    if (l < r)
    {

        int m =(l + r)/2;

        argument arg1;
        argument arg2;

        arg1.left = l;
        arg1.right = m;
        arg2.left = m+1;
        arg2.right = r;


        pthread_create(&thread1,NULL,&mergeSort,&arg1);  //create threads for both sub arrays
        pthread_create(&thread2,NULL,&mergeSort,&arg2);

        pthread_join(thread1, NULL);                     //terminate threads
        pthread_join(thread2, NULL);



        merge(l,m, r);    //merge the two sub arrays
    }

}





int main()
{
    char filename[256];  //read input file and get number of elements and elements
    puts("Enter file name:");
    puts("----------------------------------");
    fgets(filename, 260, stdin);

    filename[strlen(filename)-1]='\0';
    if(fopen(filename,"r"))
    {
        FILE *fpointer = fopen(filename,"r");
        fscanf(fpointer, "%d", &numberofelements);
        printf("number of elements: %d\n",numberofelements);

        numbers=malloc(sizeof(int)*numberofelements);  //allocating array size

        for (int i = 0; i < numberofelements; i++)  //reading the array
            fscanf(fpointer, "%d", &numbers[i]);
        fclose(fpointer);
    }
    else
    {
        puts("File Not Found!");
        puts("----------------------------------");
    }

   // for (int i = 0; i < numberofelements; i++)
   // {
   //     printf("%d  ",numbers[i]);
   // }
   // printf("\n");


    argument arg;
    arg.left=0;
    arg.right=numberofelements-1;

    mergeSort(&arg);

    printf("Sorted array: ");
    for (int i = 0; i < numberofelements; i++)
    {
        printf("%d  ",numbers[i]);
    }
    return 0;

}
