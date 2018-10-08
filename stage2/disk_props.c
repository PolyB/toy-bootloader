#include <stdint.h>

#include "init.h"
#include "int.h"
#include "print.h"
#include "printf.h"
#include "shared/disk_props.h"

#define HIGH(A) (((A) >> 8) & 0xFF)
#define LOW(A) ((A) & 0xFF)
#define PRINT_PROP(P) \
  printf("| " #P " = 0x%x\n", (P));

static void disk_props(void)
{
  printf("| reading disk properties\n");
  uint16_t ax, bx, cx, dx;
  __asm__ volatile ("int $0x13;" 
                      :
                      "=a"(ax),
                      "=b"(bx),
                      "=c"(cx),
                      "=d"(dx)
                      :
                      "a"(0x0800),
                      "d"(drive_number),
                      "D"(0)
                      :"cc", "memory");
  if (HIGH(ax))
  {
    printf("%ccannot read disk properties \"%s\"%c\n", 
        COLOR(red), errc(HIGH(ax)), COLOR(light_gray));
    return;
  }

  drive_types = LOW(bx);
  cylinder_number = HIGH(cx);
  cylinder_number |= (LOW(cx) >> 5) << 8; // >> 6 ?
  max_sector_number = LOW(cx) & 0x3F;
  max_head_number = HIGH(dx) + 1;
  number_of_drives = LOW(dx);

  PRINT_PROP(drive_types);
  PRINT_PROP(cylinder_number);
  PRINT_PROP(max_sector_number);
  PRINT_PROP(max_head_number);
  PRINT_PROP(number_of_drives);
}
INIT(disk_props);
