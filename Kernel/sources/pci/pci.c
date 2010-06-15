#include <pci/pci.h>

uint32 pciConfigReadRegister(pci_device device, uint8 reg)
{
	uint32 lbus = (uint32)  device.bus;
	uint32 lslot = (uint32) device.slot;
	uint32 lfunc = (uint32) device.function;

	/* create configuration address as per Figure 1 */
	uint32 address = (uint32)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (reg) | ((uint32)0x80000000));

	outl(0xCF8, address);
	return inl(0xCFC);
}

uint16 pciConfigReadWord (pci_device device, uint16 offset)
{    
	/* read in the data */
	uint16 tmp = (uint16)((pciConfigReadRegister(device, offset & 0xFC) >> ((offset & 2) * 8)) & 0xffff);
	return (tmp);
}

uint8 pciConfigReadByte (pci_device device, uint16 offset)
{
	printf("Lo %x\n", pciConfigReadRegister(device, offset & 0xFC));
	uint8 tmp = (uint8) ((pciConfigReadRegister(device, offset & 0xFC) >> ((offset) * 8)) & 0xff);
	return tmp;
}

uint8 pciDeviceExists(pci_device device)
{
    device.function = 0;

    if (pciConfigReadWord(device, 0) == 0xFFFF) {
    	return 0;
    }

return 1;
}

//512 256 128 64 32 16 8 4 2 1

uint16 pciDeviceGetVendor(pci_device device)
{
	device.function = 0;
	uint16 ret = pciConfigReadWord(device, 0);
	return ret;
}

uint16 pciDeviceGetDeviceId(pci_device device)
{
	device.function = 0;
	uint16 ret = pciConfigReadWord(device, 2);
	return ret;
}

uint8 pciDeviceGetClass(pci_device device)
{
	device.function = 0;
	return pciConfigReadByte(device, 11);
}