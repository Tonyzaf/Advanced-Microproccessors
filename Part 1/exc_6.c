// Define UART Data Register
volatile unsigned int * const y = (unsigned int
*)0x101f1000;
volatile unsigned int * const kadress = (unsigned int
*)0x101f1004;
void _start() {
int m=0;
int n=0;
int k;
for (k=0; k<=10; k++)
{
*(kadress+m)= (int)k;
m=m+1;
}
for (k=1; k<=10; k++)
{
* (y+n)= (unsigned int)((y-n))+k;
n=n+1;
}
}
