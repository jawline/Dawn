#include <devices/pci/pci.h>
#include <stdio.h>

void init_devices()
{
	printf("DEVICE SUBSYSTEM STARTED\n");

	printf("Scanning PCI bus\n");

	pci_device Dev;
	Dev.bus = 0;
	Dev.slot = 0;

	unsigned int busi = 0;
	unsigned int sloti = 0;
	unsigned int ndev = 0;

	for (busi = 0; busi < 255; busi++)
	{
		for (sloti = 0; sloti < 255; sloti++)
		{
			Dev.bus = busi;
			Dev.slot = sloti;

			if (pciDeviceExists(Dev))
			{
				printf("Device exists on bus %i slot %i\n", busi, sloti);
				printf("Device ID: 0x%x\n", pciDeviceGetDeviceId(Dev));
				printf("Device Class: 0x%x\n", pciDeviceGetClass(Dev));
				printf("Device Subclass: 0x%x\n", pciDeviceGetSubclass(Dev));				

				if (pciDeviceGetHeaderType(Dev) == 0x80)
				{
					printf("Device has many functions\n");
				} else if (pciDeviceGetHeaderType(Dev) == 0)
				{
					printf("Device is a general device\n");
				} else if (pciDeviceGetHeaderType(Dev) == 0x1)
				{
					printf("PCI->PCI Bridge\n");
				} else if (pciDeviceGetHeaderType(Dev) == 0x2)
				{
					printf("CardBus Bridge\n");
				} else
				{
					printf("Header type %x unknown\n", pciDeviceGetHeaderType(Dev));
				}

				printf("Device Vendor: 0x%x\n", pciDeviceGetVendor(Dev));
				ndev++;
			}
		}

		sloti = 0;
	}

	printf("%i devices found\n", ndev);
	printf("DEVICE SUBSYSTEM INITIALIZED\n");
}
