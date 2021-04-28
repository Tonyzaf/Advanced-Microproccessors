volatile unsigned int * y = (unsigned int *)0x101f1000;
volatile unsigned int * a = (unsigned int *)0x101f1004;
volatile unsigned int * b = (unsigned int *)0x101f1008;
void _start() {
int k;
int arra[10] = {1,2,3,4,5,6,7,8,9,0};
int arrb[10] = {1,2,3,4,5,6,7,8,9,0};
*a = arra[];
*b = arrb[];
for (k=0; k<10; k++)
{
y = (*a[k]) * (*b[k]);
}
}