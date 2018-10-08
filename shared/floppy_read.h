#ifndef SHARED_FLOPPY_READ_H
#define SHARED_FLOPPY_READ_H

#include <stddef.h>

#include "floppy.h"
#include "dma.h"
#include "disk_props.h"

#define TRANSFER_BUFFER ((uint8_t *)0x1000)

// this file is in headers because it will be used in multiple achitecture size (32-64 (and maybe16))
// return NULL on success

#ifdef PRINTF_H
#define printf(...) printf(__VA_ARGS__)
#else
#define printf(...) do{}while(0)
#endif

// #define DUMP_COPY
#define FPY_DMA_PORT 2

static inline void fpy_setup_idt(void)
{
  fpy_irq_received = 0;
  set_interrupt(FLOPPY_IRQ, fpy_interrupt_handler);
}

static inline const char *fpy_copy_bytes(uint8_t sector, uint8_t *dest, long count)
{
  while (count > 0)
  {
    uint32_t lba = sector;
    uint32_t c = lba / (max_head_number * max_sector_number);
    uint32_t h = (lba /  max_sector_number) % max_head_number;
    uint32_t s = (lba % max_sector_number) + 1;
    struct fpy_cmd_seek_head_params params_seek =
    {
      .drive = drive_number,
      .head_number = h,
      .cylinder = c,
    };
    err_t err = fpy_cmd_seek_head(&params_seek);
    if (err)
      return err;

    struct fpy_cmd_read_sector_params params =
    {
      .multitrack = true,
      .double_density = true,
      .skip_mode = false,
      .drive = drive_number,
      .head_number = h,
      .cylinder = c,
      .head = h,
      .sector = s,
      .sector_size = 2,
      .track_length = max_sector_number,
      .length_of_gap3 = 0x1b,
      .data_length = 0xff,
    };
    struct fpy_cmd_readtrack_res res = fpy_cmd_read_sector(&params);
    if (res.error)
      return res.error;
    // bytes are in 0x1000, copy just the right number at desired location
    uint16_t copy_count = count > 0x200 ? 0x200 : count;
    for (uint16_t i = 0; i < copy_count; i++)
    {
      dest[i] = TRANSFER_BUFFER[i];
#ifdef DUMP_COPY
      if (!(i & 0xF))
        printf("\n %p : ", dest + i);
      printf("%.2x ", dest[i]);
#endif
    }
#ifdef DUMP_COPY
    printf("\n");
#endif
    count -= 0x200;
    dest += 0x200;
    sector+=1;
  }
  return NULL;
}

#endif // SHARED_FLOPPY_READ_H
