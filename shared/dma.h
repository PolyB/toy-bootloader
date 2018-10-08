#ifndef SHARED_DMA_H
#define SHARED_DMA_H

#include <stdint.h>

#include "outin.h"


enum dma_mode_sel
{
  DMA_MODE_DEMAND   = 0x0,
  DMA_MODE_SINGLE   = 0x1,
  DMA_MODE_BLOCK    = 0x2,
  DMA_MODE_CASCADE  = 0x3,
};

enum dma_mode_init
{
  DMA_MODE_INIT_SINGLE_CYCLE = 0,
  DMA_MODE_INIT_AUTO = 1,
};

enum dma_mode_transfer
{
  DMA_MODE_TRANSFER_VERIFY  = 0x0,
  DMA_MODE_TRANSFER_READ    = 0x1,
  DMA_MODE_TRANSFER_WRITE   = 0x2,
};

enum dma_mode_incdec
{
  DMA_MODE_INCREMENT = 0,
  DMA_MODE_DECREMENT = 1,
};

struct dma_mode_raw
{
  uint8_t channel : 2;
  enum dma_mode_transfer transfer : 2;
  enum dma_mode_init init: 1;
  enum dma_mode_incdec inc_dec : 1;

  enum dma_mode_sel mode : 2;
} __attribute__((packed));

union dma_mode
{
  struct dma_mode_raw str;
  uint8_t val;
};

struct dma_control_raw
{
  uint8_t channel : 2;
  uint8_t disable : 1;
  uint8_t _zero : 5;
} __attribute__((packed));

union dma_control
{
  struct dma_control_raw str;
  uint8_t val;
};

static inline void dma_mask_channel(uint8_t chan)
{
  union dma_control c;
  c.str._zero = 0;
  c.str.disable = 1;
  c.str.channel = chan;
  do_outb(0x0a, c.val);
}

static inline void dma_unmask_channel(uint8_t chan)
{
  union dma_control c;
  c.str._zero = 0;
  c.str.disable = 0;
  c.str.channel = chan;
  do_outb(0x0a, c.val);
}

static uint8_t dma_get_port_address(uint8_t chan)
{
  uint8_t ports[] = {
    [0] = 0x00,
    [1] = 0x02,
    [2] = 0x04,
    [3] = 0x06,
    [4] = 0xC0,
    [5] = 0xC4,
    [6] = 0xC8,
    [7] = 0xCC
  };
  return ports[chan];
}
static uint8_t dma_get_port_count(uint8_t chan)
{
  uint8_t ports[] = {
    [0] = 0x01,
    [1] = 0x03,
    [2] = 0x05,
    [3] = 0x07,
    [4] = 0xC2,
    [5] = 0xC6,
    [6] = 0xCA,
    [7] = 0xCE
  };
  return ports[chan];
}
static uint8_t dma_get_port_page(uint8_t chan)
{
  uint8_t ports[] = {
    [0] = 0x87,
    [1] = 0x83,
    [2] = 0x81,
    [3] = 0x81,
    [4] = 0x8F, // used for irq must not use
    [5] = 0x8B,
    [6] = 0x89,
    [7] = 0x8A
  };
  return ports[chan];
}

static inline void dma_setup_address(uint8_t chan, uint16_t addr)
{
  uint8_t port = dma_get_port_address(chan);
  do_outb(port, addr & 0xFF);
  do_outb(port, addr >> 8);
}
static inline void dma_setup_count(uint8_t chan, uint16_t count)
{
  uint8_t port = dma_get_port_count(chan);
  do_outb(port, count & 0xFF);
  do_outb(port, count >> 8);
}
static inline void dma_setup_page(uint8_t chan, uint8_t val)
{
  uint8_t port = dma_get_port_page(chan);
  do_outb(port, val);
}

static inline void dma_reset_flipflop(uint8_t chan) { do_outb(chan > 3 ? 0xD8 : 0x0C, 0xFF); }
static inline void dma_set_mode(uint8_t chan, enum dma_mode_sel sel, enum dma_mode_incdec incdec, enum dma_mode_init init, enum dma_mode_transfer tr)
{
  union dma_mode mode;
  mode.str.mode = sel;
  mode.str.inc_dec = incdec;
  mode.str.init = init;
  mode.str.transfer = tr;
  mode.str.channel = chan > 3 ? chan - 4 : chan;
  do_outb(chan > 3 ? 0xD6 : 0x0B, mode.val);
}

#endif
