#include <devices/pci/pci.h>
#include <stdio.h>
#include <panic/panic.h>

void initializePciDevice(pci_device active_dev)
{
	unsigned char class = pciDeviceGetClass(active_dev);
	unsigned char subclass = pciDeviceGetSubclass(active_dev);

	switch (class)
	{

		case CLASS_TYPE_MASS_STORAGE:
		{
			//Mass storage device, iterate through the possibles
			switch (subclass)
			{
				case MASS_STORAGE_TYPE_IDE:
				{
					printf("IDE controller found, being initialized\n");
					printf("Vendor 0x%x\n", pciDeviceGetVendor(active_dev));
					printf("ID 0x%x\n", pciDeviceGetDeviceId(active_dev));
					printf("Bus %x slot %x function %x\n", active_dev.bus, active_dev.slot, active_dev.function);
					printf("Bar0 0x%x Bar1 0x%x Bar2 0x%x Bar3 0x%x\n", pciDeviceGetBar0(active_dev), pciDeviceGetBar1(active_dev), pciDeviceGetBar2(active_dev), pciDeviceGetBar3(active_dev));

					unsigned long primary_channel_io = 0, secondary_channel_io = 0, primary_channel_ctrl = 0, secondary_channel_ctrl = 0;

					if (pciDeviceGetBar0(active_dev) == 0x0 || pciDeviceGetBar0(active_dev) == 0x1)
					{
						primary_channel_io = 0x1F0;
					}

					if (pciDeviceGetBar1(active_dev) == 0x0 || pciDeviceGetBar1(active_dev) == 0x1)
					{
						primary_channel_ctrl = 0x3F4;
					}

					if (pciDeviceGetBar2(active_dev) == 0x0 || pciDeviceGetBar0(active_dev) == 0x1)
					{
						secondary_channel_io = 0x170;
					}

					if (pciDeviceGetBar3(active_dev) == 0x0 || pciDeviceGetBar0(active_dev) == 0x1)
					{
						secondary_channel_ctrl = 0x37F;
					}

					initializeIdeHardDrive(primary_channel_io, primary_channel_ctrl, secondary_channel_io, secondary_channel_ctrl);	

					break;
				};

				default:
				{
					printf("Unhandled mass storage device\n");
					//Unhandled storage device
					break;
				};
			};
		};

		default:
		{
			//Unhandled device
			break;
		};
	}
}

void initializeDevices()
{

	printf("Scanning PCI bus\n");

	pci_device Dev;
	Dev.bus = 0;
	Dev.slot = 0;

	unsigned int busi = 0;
	unsigned int sloti = 0;
	unsigned int functioni = 0;
	unsigned int ndev = 0;


	for (busi = 0; busi < 255; busi++)
	{
		for (sloti = 0; sloti < 255; sloti++)
		{
			Dev.bus = busi;
			Dev.slot = sloti;
			Dev.function = 0;

			if (pciDeviceExists(Dev))
			{
				if (pciDeviceGetHeaderType(Dev) == 0x80)
				{

					for (functioni = 0; functioni < 3; functioni++)
					{
						Dev.function = functioni;
						initializePciDevice(Dev);
						ndev++;
					}

				} else
				{
					initializePciDevice(Dev);
					ndev++;					
				}
			
			}
		}

		sloti = 0;
	}

	printf("%i devices found\n", ndev);
}
