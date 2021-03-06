
#include "gdt.h"

GlobalDescriptorTable::GlobalDescriptorTable() : 
nullSegmnetSelector(0, 0, 0), 
unusedSegmentSelector(0, 0, 0), 
codeSegmentSelector(0,64*1024*1024, 0x9A),
 dataSegmentSelector(0,64*1024*1024, 0x92) {
	uint32_t i[2];
	i[0] = sizeof(GlobalDescriptorTable) << 16;
	i[1] = (uint32_t)this;

	asm volatile("lgdt (%0)": :"p" (((uint8_t *) i)+2));
}

GlobalDescriptorTable::~GlobalDescriptorTable() {}

uint16_t GlobalDescriptorTable::getDataSegmentSelector() {
	return (uint8_t*)&dataSegmentSelector - (uint8_t*) this;
}

uint16_t GlobalDescriptorTable::getCodeSegmentSelector() {
	return (uint8_t*)&codeSegmentSelector - (uint8_t*) this;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags) {
	uint8_t* target = (uint8_t*)this;
	if(limit <=  0x10000) {
		target [6] = 0x40; //16bit mem adress
	} else { //32bit mem adress
		if((limit & 0xfff) != 0xfff) {
			limit = (limit >> 12 ) - 1;
		} else {
			limit = limit >> 12;
		}
		target[6] = 0xc0;
	}
	target[0] = limit & 0xff;
	target[1] = (limit >> 8) & 0xff;
	target[6] |= (limit >> 16) & 0xf;

	target[2] = (base >> 0) & 0xff;
	target[3] = (base >> 8) & 0xff;
	target[4] = (base >> 16) & 0xff;
	target[7] = (base >> 24) & 0xff;

	target[5] = flags;

}

uint32_t GlobalDescriptorTable::SegmentDescriptor::getBase() {
	uint8_t* target = (uint8_t*)this;
	uint32_t result = target[7];
	result = (result << 8) + target[4];
	result = (result << 8) + target[3];
	result = (result << 8) + target[2];
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::getLimit() {
	uint8_t* target = (uint8_t*)this;
	uint32_t result = target[6] & 0xf;
	result = (result << 8) + target[1];
	result = (result << 8) + target[0];
	
	if((target[6] & 0xc0) == 0xc0) result = (result << 12) | 0xfff;

	return result;
}
