#ifndef SHARED_FLOPPY_H
#define SHARED_FLOPPY_H

#include <stdint.h>
#include <stdbool.h>
#include "disk_props.h"
#include "outin.h"

#define FLOPPY_IRQ 38

typedef const char * err_t;
#define _STR(X) #X
#define _EXP_STR(X) _STR(X)
#define MK_ERR(ERR) ("(" __FILE__ ")-(l." _EXP_STR(__LINE__) ") : " ERR)
#define NO_ERR ((err_t)0)

static inline void fpy_motor_disable(void);
static inline void fpy_motor_enable(void);

static inline void fpy_disable_controller(void);
static inline void fpy_enable_controller(void);
/* ==== COMMANDS ==== */

struct fpy_cmd_head_param
{
  uint8_t drive;
  uint8_t head_number; // 1 or 0
};

/* READTRACK */
struct fpy_cmd_readtrack_params
{
  uint8_t drive;
  bool double_density;
  bool skip_mode;
  uint8_t head_number; // 1 or 0
  uint8_t cylinder;
  uint8_t head;
  uint8_t sector; // documentation says that it's ignored TODO : verify
  uint8_t sector_size;
  uint8_t track_length;
  uint8_t length_of_gap3;
  uint8_t data_length;
};
struct fpy_cmd_readtrack_res
{
  err_t error;
  uint8_t st0;
  uint8_t st1;
  uint8_t st2;
  uint8_t cylinder;
  uint8_t head;
  uint8_t sector;
  uint8_t sector_size;
};
static inline struct fpy_cmd_readtrack_res fpy_cmd_readtrack(struct fpy_cmd_readtrack_params *params);

/* WRITE_SECTOR */
struct fpy_cmd_write_sector_params
{
  uint8_t drive;
  bool double_density;
  bool multitrack;
  uint8_t head_number; // 1 or 0
  uint8_t cylinder;
  uint8_t head;
  uint8_t sector;
  uint8_t sector_size;
  uint8_t track_length;
  uint8_t length_of_gap3;
  uint8_t data_length;
};
static inline struct fpy_cmd_readtrack_res fpy_cmd_write_sector(struct fpy_cmd_write_sector_params *params);

/* READ SECTOR */
struct fpy_cmd_read_sector_params
{
  uint8_t drive;
  bool double_density;
  bool multitrack;
  bool skip_mode;
  uint8_t head_number; // 1 or 0
  uint8_t cylinder;
  uint8_t head;
  uint8_t sector;
  uint8_t sector_size;
  uint8_t track_length;
  uint8_t length_of_gap3;
  uint8_t data_length;
};
static inline struct fpy_cmd_readtrack_res fpy_cmd_read_sector(struct fpy_cmd_read_sector_params *params);

/* WRITE DELETED SECTOR */
static inline struct fpy_cmd_readtrack_res fpy_cmd_write_deleted_sector(struct fpy_cmd_write_sector_params *params);

/* READ DELETED SECTOR */
static inline struct fpy_cmd_readtrack_res fpy_cmd_read_deleted_sector(struct fpy_cmd_read_sector_params *params);

/* FORMAT TRACK */
// TODO : reimplement
// struct fpy_cmd_format_track_params
// {
//   uint8_t drive;
//   bool double_density;
//   uint8_t head_number; // 1 or 0
//   uint8_t sector_size;
//   uint8_t track_length;
//   uint8_t length_of_gap3;
//   uint8_t fill_byte;
// };
// struct fpy_cmd_format_track_res
// {
//   bool success;
//   uint8_t track;
//   uint8_t head;
//   uint8_t sector;
//   uint8_t sector_size;
// };
// static inline struct fpy_cmd_format_track_res fpy_cmd_format_track(struct fpy_cmd_format_track_params *params);

/* FIX DRIVE DATA */
struct fpy_cmd_fix_drive_data_params
{
  uint8_t step_rate; /* in ms */
  uint8_t head_unload_time; /* in ms */
  uint8_t head_load_time; /* in ms */
  bool ndm;
};
static inline err_t fpy_cmd_fix_drive_data(struct fpy_cmd_fix_drive_data_params *params);

/* CHECK DRIVE STATUS */

struct fpy_cmd_check_drive_status_res
{
  err_t error;
  uint8_t st3;
};
static inline struct fpy_cmd_check_drive_status_res fpy_cmd_check_drive_status(struct fpy_cmd_head_param *params);

/* CALIBRATE DRIVE */
static inline err_t fpy_cmd_calibrate_drive(struct fpy_cmd_head_param *params);

/* CHECK INTERRUPT */
struct fpy_cmd_check_interrupt_res
{
  err_t error;
  uint8_t st0;
  uint8_t current_cylinder;
};
static inline struct fpy_cmd_check_interrupt_res fpy_cmd_check_interrupt(void);

/* READ SECTOR ID */
struct fpy_cmd_read_sector_id_res
{
  bool succcess;
  uint8_t st0;
  uint8_t st1;
  uint8_t st2;
  uint8_t cylinder;
  uint8_t head;
  uint8_t sector;
  uint8_t sector_size;
};
static inline struct fpy_cmd_read_sector_id_res fpy_cmd_read_sector_id(struct fpy_cmd_head_param *params);

/* SEEK HEAD */
struct fpy_cmd_seek_head_params
{
  uint8_t drive;
  uint8_t head_number; // 1 or 0
  uint8_t cylinder;
};
static inline err_t fpy_cmd_seek_head(struct fpy_cmd_seek_head_params *params);

/* CHECK VERSION */
static inline err_t fpy_cmd_check_version(void);

/* CONFIGURE */
struct fpy_cmd_configure_params
{
  bool implied_seek_enable;
  bool fifo_disable;
  bool drive_polling_disable;
  uint8_t treshold_val;
  uint8_t precomp_val; // 0 = manufacturer value
};
static inline err_t fpy_cmd_configure(struct fpy_cmd_configure_params *params);

static inline err_t fpy_cmd_lock(bool lock);

// TODO command verify (0x16)
// TODO command seek-rel (0xfh)


extern const void* fpy_irq_received; // 0 = wait, 1 = 0k , ptr = err
extern uint8_t fpy_irq_handle_state; // 0 = handle directly on interrupt, 1 = do not handle

static void fpy_interrupt_handler(struct int_infos *regs)
{
  (void)regs; // regs must not be used as this function will be used in 32&64bit
  if (fpy_irq_handle_state)
  {
    fpy_irq_handle_state = 0;
    fpy_irq_received = (void*)1;
  }
  else
  {
    struct fpy_cmd_check_interrupt_res r = fpy_cmd_check_interrupt();
    fpy_irq_received = ((bool)r.error) ? r.error : (void*)1;
  }
  do_outb(0x20, 0x20);
}


static void fpy_prepare_irq_no_handle(void)
{
  fpy_irq_handle_state = 1;
}

static err_t fpy_wait_irq(void)
{
  while (!fpy_irq_received);
  err_t err = NO_ERR;
  if (fpy_irq_received != (const char *)1)
    err = (err_t)fpy_irq_received;
  fpy_irq_received = 0;
  return err;
}

#include "floppy_impl.h"

#endif
