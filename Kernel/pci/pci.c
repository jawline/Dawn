#include <pci/pci.h>

uint16 pciConfigReadWord (pci_device device, uint16 offset)
{
	uint32 address;
	uint32 lbus = (uint32)  device.bus;
	uint32 lslot = (uint32) device.slot;
	uint32 lfunc = (uint32) device.function;
	uint16 tmp = 0;
	 
	/* create configuration address as per Figure 1 */
	address = (unsigned long)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc) | ((uint32)0x80000000));

	/* write out the address */
	outl(0xCF8, address);
	    
	/* read in the data */
	tmp = (unsigned short)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
	return (tmp);
}

uint8 pciConfigReadByte (pci_device device, uint16 offset)
{
	uint32 address;
	uint32 lbus = (uint32)  device.bus;
	uint32 lslot = (uint32) device.slot;
	uint32 lfunc = (uint32) device.function;
	uint16 tmp = 0;
	 
	/* create configuration address as per Figure 1 */
	address = (unsigned long)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc) | ((uint32)0x80000000));

	/* write out the address */
	outl(0xCF8, address);
	    
	/* read in the data */
	tmp = (unsigned short)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
	return (tmp);
}

uint8 pciDeviceExists(pci_device device)
{
    device.function = 0;

    if (pciConfigReadWord(device, 0) == 0xFFFF) {
    	return 0;
    }

return 1;
}

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
	return 0x0; //TODO: FINISH
}
