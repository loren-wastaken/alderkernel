// test_app.c - minimal freestanding test ELF for runelf
// entry point is `main` itself here - no argc/argv, no libc, nothing.
// It just writes a known value to a known memory address, then
// returns - proving the loader placed and jumped to real code.
void main(void)
{
    volatile unsigned int* marker = (volatile unsigned int*)0x00090000;
    *marker = 0xDEADC0DE;
    // falls off the end -> compiler emits `ret` -> control returns
    // cleanly to command_runelf in the kernel
}