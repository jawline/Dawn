#include <devices/pci/pci.h>

uint32 pciConfigReadRegister(pci_device device, uint8 reg)
{
	uint32 lbus = (uint32)  device.bus;
	uint32 lslot = (uint32) device.slot;
	uint32 lfunc = (uint32) device.function;

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
	uint8 tmp = (uint8) ((pciConfigReadRegister(device, offset & 0xFC) >> ((offset) * 8)) & 0xff);
	return tmp;
}

unsigned char pciDeviceExists(pci_device device)
{

    if (pciConfigReadWord(device, 0) == 0xFFFF) {
    	return 0;
    }

return 1;
}

//512 256 128 64 32 16 8 4 2 1

unsigned short pciDeviceGetVendor(pci_device device)
{
	uint16 ret = pciConfigReadWord(device, 0);
	return ret;
}

unsigned short pciDeviceGetDeviceId(pci_device device)
{
	uint16 ret = pciConfigReadWord(device, 2);
	return ret;
}

unsigned char pciDeviceGetClass(pci_device device)
{
	return pciConfigReadByte(device, 11);
}

unsigned char pciDeviceGetSubclass(pci_device device)
{
	return pciConfigReadByte(device, 10);
}

unsigned char pciDeviceGetHeaderType(pci_device dev)
{
	return pciConfigReadByte(dev, 14);
}

uint32 pciDeviceGetBar0(pci_device dev)
{
	return pciConfigReadRegister(dev, 16);
}

uint32 pciDeviceGetBar1(pci_device dev)
{
	return pciConfigReadRegister(dev, 18);
}

uint32 pciDeviceGetBar2(pci_device dev)
{
	return pciConfigReadRegister(dev, 20);
}

uint32 pciDeviceGetBar3(pci_device dev)
{
	return pciConfigReadRegister(dev, 22);
}

uint32 pciDeviceGetBar4(pci_device dev)
{
	return pciConfigReadRegister(dev, 24);
}

uint32 pciDeviceGetBar5(pci_device dev)
{
	return pciConfigReadRegister(dev, 26);
}
