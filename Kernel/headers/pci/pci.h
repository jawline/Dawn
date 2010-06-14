#ifndef _PCI_DEFINITION_DEF_H_
#define _PCI_DEFINITION_DEF_H_
#include <types/int_types.h>

typedef struct {
	uint8 bus;
	uint8 slot;
	uint8 function;

	uint16 vendor_id;
	uint16 device_id;
	uint8 header_type;
} pci_device;

#endif //_PCI_DEFINITION_DEF_H_
