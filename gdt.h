#ifndef __GDT_H
#define __GDT_H

#include "types.h"

class GlobalDescriptorTable {
  public:
	class SegmentDescriptor {
	private:
		uint16_t limit_lwr;
		uint16_t base_lwr;
		uint8_t base_lwr_upr;
		uint8_t type;
		uint8_t flags_limit_upr;
		uint8_t base_upr_upr;
	public:
		SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
		uint32_t getBase();
		uint32_t getLimit();

	} __attribute__((packed));

	SegmentDescriptor nullSegmnetSelector;
	SegmentDescriptor unusedSegmentSelector;
	SegmentDescriptor codeSegmentSelector;
	SegmentDescriptor dataSegmentSelector;

public:
	GlobalDescriptorTable();
	~GlobalDescriptorTable();

	uint16_t getCodeSegmentSelector();
	uint16_t getDataSegmentSelector();
};

#endif

