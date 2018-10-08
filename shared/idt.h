#ifndef SHARED_IDT_ENTRIES_H
#define SHARED_IDT_ENTRIES_H

#define IS_FAULT(E)																														\
	(((E) == 0) || ((E) == 5) || ((E) == 6) || ((E) == 7) || ((E) == 9) ||			\
	 ((E) == 10) || ((E) == 11) || ((E) == 12) || ((E) == 13) || ((E) == 14) || \
	 ((E) == 16) || ((E) == 17) || ((E) == 19) || ((E) == 20))

#define IS_TRAP(E) (((E) == 3) || ((E) == 4))

#define IS_INTERRUPT(E) (((E) == 2) || ((E) >= 32 && (E) <= 255))

// XMacro for idt informations

#define X_IDT_ENTRIES																													\
	X(0, XNOERRORCODE, "Divide Error")																		      \
	X(1, XNOERRORCODE, "Debug Exception")																        \
	X(2, XNOERRORCODE, "NMI Interruption")															       	\
	X(3, XNOERRORCODE, "Breakpoint")																		       	\
	X(4, XNOERRORCODE, "Overflow")																			       	\
	X(5, XNOERRORCODE, "Bound range exceeded")													       	\
	X(6, XNOERRORCODE, "Invalid OpCode")																       	\
	X(7, XNOERRORCODE, "Device not compatible")													        \
	X(8, XERRORCODE, "Double fault")																		       	\
	X(9, XNOERRORCODE, "Coprocessor segment overrun")										        \
	X(10, XERRORCODE, "Invalid TSS")																		       	\
	X(11, XERRORCODE, "Segment not present")														       	\
	X(12, XERRORCODE, "Stack-segment Not present")											       	\
	X(13, XERRORCODE, "General protection")															        \
	X(14, XERRORCODE, "Page fault")																			        \
	X(15, XNOERRORCODE, "Intel-reserved")																        \
	X(16, XNOERRORCODE, "x87 FPU Floating point error")									        \
	X(17, XERRORCODE, "Alignement check")																        \
	X(18, XNOERRORCODE, "machine check")																       	\
	X(19, XNOERRORCODE, "SMID floating-point excepion")									        \
	X(20, XNOERRORCODE, "virtualization exception")											        \
	X(32, XNOERRORCODE, "IRQ0 - PIT")													        				  \
	X(33, XNOERRORCODE, "IRQ1 - Keyboard")									        				    \
	X(35, XNOERRORCODE, "IRQ3 - UART (COM2 and COM4)")									       	\
	X(36, XNOERRORCODE, "IRQ4 - UART (COM1 and COM3)")									       	\
	X(37, XNOERRORCODE, "IRQ5 - Hard disk/ Parallel port")							       	\
	X(38, XNOERRORCODE, "IRQ6 - Floppy disk controller")								       	\
	X(39, XNOERRORCODE, "IRQ7 - Parallel port LPT1")										       	\
	X(40, XNOERRORCODE, "IRQ8 - RTC")																		        \
	X(41, XNOERRORCODE, "IRQ9 - APCI")																					\
	X(42, XNOERRORCODE, "IRQ10 - Available")																	  \
	X(43, XNOERRORCODE, "IRQ11 - Available")																	  \
	X(44, XNOERRORCODE, "IRQ12 - PS/2 mouse controller")								       	\
	X(45, XNOERRORCODE, "IRQ13 - Math coprocessor")											        \
	X(46, XNOERRORCODE, "IRQ14 - Hard disk controller 1")								        \
	X(47, XNOERRORCODE, "IRQ15 - Hard disk controller 2")

#endif // SHARED_IDT_ENTRIES_H
