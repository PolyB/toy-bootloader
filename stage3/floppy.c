#include "idt.h"
#include "init.h"
#include "printf.h"
#include "shared/dma.h"
#include "shared/floppy.h"
#include "shared/floppy_read.h"

#define FPY_DMA_PORT 2


static void cmd_fail(const char *cmd, unsigned int line)
{
  printf("| floppy : error `%s` (l.%u)\n", cmd, line);
}

static void cmd_error(const char *cmd, unsigned int line, const char *err)
{
  printf("| floppy : command `%s` failed (l.%u), got error : `%s`\n", cmd, line, err);
}


#define CMD_FAIL(CMD) \
  cmd_fail(CMD, __LINE__)
#define CMD_ERROR(CMD, ERR) \
  cmd_error(CMD, __LINE__, ERR)
#define LOG(STR) \
  printf("| floppy : " STR "\n")

static void floppy_dma(void)
{
  dma_mask_channel(FPY_DMA_PORT);
  dma_reset_flipflop(FPY_DMA_PORT);
  dma_setup_address(FPY_DMA_PORT, 0x1000);
  dma_reset_flipflop(FPY_DMA_PORT);
  dma_setup_count(FPY_DMA_PORT, 0x300 - 1);
  dma_setup_page(FPY_DMA_PORT, 0);
  dma_unmask_channel(FPY_DMA_PORT);
  LOG("dma setup for floppy disk ok");
}

static void floppy_dma_setup_read(void)
{
  dma_mask_channel(FPY_DMA_PORT);
  dma_set_mode(
      FPY_DMA_PORT,
      DMA_MODE_SINGLE,
      DMA_MODE_INCREMENT,
      DMA_MODE_INIT_AUTO,
      DMA_MODE_TRANSFER_READ);
  dma_unmask_channel(FPY_DMA_PORT);
  LOG("dma setup for read ok");
}

static void floppy_init(void)
{
  fpy_setup_idt();

  err_t err;
  // VERSION
  if ((err = fpy_cmd_check_version()))
    return CMD_ERROR("version", err);
  LOG("version ok");

  fpy_write_dsr(3);
  fpy_motor_enable();

  {// FIX DRIVE DATA
    struct fpy_cmd_fix_drive_data_params params =
    {
      .step_rate = 8,
      .ndm = 0,
      .head_unload_time = 0,
      .head_load_time = 5,
    };
    if ((err = fpy_cmd_fix_drive_data(&params)))
      return CMD_ERROR("fix drive data", err);
    LOG("fix drive data ok");
  }



  {// CONFIGURE
    struct fpy_cmd_configure_params params =
    {
      .implied_seek_enable = 1,
      .fifo_disable = 0,
      .drive_polling_disable = 1,
      .treshold_val = 1,
      .precomp_val = 0
    };
    if ((err = fpy_cmd_configure(&params)))
      return CMD_ERROR("configure", err);
    LOG("configure ok");
  }
  // if (!fpy_cmd_lock(true))
  //     return CMD_FAIL("lock");
  //   LOG("lock ok");

  {// CALIBRATE
    LOG("start calibrate");
    struct fpy_cmd_head_param params =
    {
      .drive = drive_number,
      .head_number = 0,
    };
    if ((err = fpy_cmd_calibrate_drive(&params)))
      return CMD_ERROR("calibrate head", err);
    if ((err = fpy_wait_irq()))
      return CMD_ERROR("wait_irq", err);
    params.head_number = 1;
    if ((err = fpy_cmd_calibrate_drive(&params)))
      return CMD_ERROR("calibrate head", err);
    if ((err = fpy_wait_irq()))
      return CMD_ERROR("wait_irq", err);

  }
  LOG("calibrate head ok");

}

static void floppy_test(void)
{
  uint32_t lba = 0;
  uint32_t c = lba / (max_head_number * max_sector_number);
  uint32_t h = (lba /  max_sector_number) % max_head_number;
  uint32_t s = (lba % max_sector_number) + 1;

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
    .track_length = 1,
    .length_of_gap3 = 0x1b,
    .data_length = 0xff,
  };
  struct fpy_cmd_readtrack_res res = fpy_cmd_read_sector(&params);
  if (res.error)
    return CMD_ERROR("read_sector", res.error);
  LOG("read_sector ok");

  uint16_t floppy_sign = *(uint16_t *)0x11FE;
  if (floppy_sign != 0xaa55)
    CMD_FAIL("cannot read floppy disk signature");
  else
    LOG("floppy disk signature check ok");
}

INIT_ORDERED
{
INIT_ENTRY(floppy_init)
INIT_ENTRY(floppy_dma)
INIT_ENTRY(floppy_dma_setup_read)
INIT_ENTRY(floppy_test)
INIT_ENTRY(floppy_test)
};
