void gpfHandler()
{
	printf("General protection fault\n");
	for (;;) { }
}

void initializeGeneralProtectionFaultHandler()
{
	register_interrupt_handler (13 , &gpfHandler);	
}
