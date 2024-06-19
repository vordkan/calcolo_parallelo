#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
    int i,N,t,nloc;
    float sumtot,sum,*a;
    sumtot=0;

    printf("Inserire N\n");
    scanf("%d",&N);

    a=(float *)calloc(N,sizeof(float));

    printf("Inserire i numeri da sommare\n");
    for (i=0;i<N;i++)
    {
        scanf("%f",&a[i]);
    }
#pragma omp parallel private(sum,nloc,i) shared(sumtot)

    { // se piu di un�istruzione

        t=omp_get_num_threads();
        nloc=N/t;
        printf("sono %d, di %d: numeri %d\n",omp_get_thread_num(),t,nloc);

        sum=0;
        for(i=0;i<nloc;i++)
        {
            sum=sum+a[i+nloc*omp_get_thread_num()];
        }
        sumtot+=sum;

    } //fine direttiva

    printf("somma totale: %f\n",sumtot);

}
