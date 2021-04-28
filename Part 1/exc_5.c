/*
*Arm system
*/
// Define UART Data Register
volatile unsigned int * const Uart_DR = (unsigned int
*)0x101f1000;
// Main C function:
void _start() {
volatile unsigned int * A = (unsigned int
*)0x101f1004;
volatile unsigned int * B = (unsigned int
*)0x101f1008;
volatile unsigned int * C = (unsigned int
*)0x101f100C;
A = (unsigned int) B + C;
}