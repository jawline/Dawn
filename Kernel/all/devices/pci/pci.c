#include <devices/pci/pci.h>

uint32_t pciConfigReadRegister(pci_device device, uint8_t reg)
{
	uint32_t lbus = (uint32_t)  device.bus;
	uint32_t lslot = (uint32_t) device.slot;
	uint32_t lfunc = (uint32_t) device.function;

	uint32_t address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (reg) | ((uint32_t)0x80000000));

	outl(0xCF8, address);
	return inl(0xCFC);
}

uint16_t pciConfigReadWord (pci_device device, uint16_t offset)
{
	/* read in the data */
	uint16_t tmp = (uint16_t)((pciConfigReadRegister(device, offset & 0xFC) >> ((offset & 2) * 8)) & 0xffff);
	return (tmp);
}

uint8_t pciConfigReadByte (pci_device device, uint16_t offset)
{
	uint8_t tmp = (uint8_t) ((pciConfigReadRegister(device, offset & 0xFC) >> ((offset) * 8)) & 0xff);
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
	uint16_t ret = pciConfigReadWord(device, 0);
	return ret;
}

unsigned short pciDeviceGetDeviceId(pci_device device)
{
	uint16_t ret = pciConfigReadWord(device, 2);
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

uint32_t pciDeviceGetBar0(pci_device dev)
{
	return pciConfigReadRegister(dev, 16);
}

uint32_t pciDeviceGetBar1(pci_device dev)
{
	return pciConfigReadRegister(dev, 18);
}

uint32_t pciDeviceGetBar2(pci_device dev)
{
	return pciConfigReadRegister(dev, 20);
}

uint32_t pciDeviceGetBar3(pci_device dev)
{
	return pciConfigReadRegister(dev, 22);
}

uint32_t pciDeviceGetBar4(pci_device dev)
{
	return pciConfigReadRegister(dev, 24);
}

uint32_t pciDeviceGetBar5(pci_device dev)
{
	return pciConfigReadRegister(dev, 26);
}
