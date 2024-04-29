#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
int n;
int main(int argc, char** argv){
    int threads = atoi(argv[1]);
    omp_set_num_threads(threads);
    scanf("%d",&n);
   clock_t start_t, end_t;
    struct timeval begin, end;
    gettimeofday(&begin, 0);
   double total_t;
   start_t = clock();
    int** A=(int**)malloc((n+1)*sizeof(int*));
    for(int i=0;i<n;i++){
            A[i]=(int*)malloc((n+1)*sizeof(int));
            for(int j=0;j<=n;j++)scanf("%d",A[i]+j);
    }

    double* x=(double*)malloc(n*sizeof(double));
    double* xnew=(double*)malloc(n*sizeof(double));

    for(int i=0;i<n;i++)x[i]=0;
    for(int t=1;t<=n;t++){
            #pragma omp parallel
    {
        int step=n/omp_get_num_threads()+1;
        int thread=omp_get_thread_num();
        int l=thread*step;
        int r=(thread+1)*(step);
        if(r>n)r=n;
        for(int k=l;k<r;k++){
            xnew[k]=A[k][n];
            for(int i=0;i<n;i++)if(i!=k)xnew[k]-=A[k][i]*x[i];
            xnew[k]/=A[k][k];
        }
    }
        for(int i=0;i<n;i++)x[i]=xnew[i];

    }
   end_t = clock();
   total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds*1e-6;
   printf("Total time taken by CPU: %f\n", total_t  );
   printf("Real time measured: %.3f seconds.\n", elapsed);
    for(int i=0;i<n;i++)printf("%f ",x[i]);
    freopen("check.txt","w",stdout);
    for(int i=0;i<n;i++){
        double ans=0;
        for(int j=0;j<n;j++)ans+=x[j]*A[i][j];
        ans-=A[i][n];
        printf("%f ",ans);
    }
    return 0;
}
