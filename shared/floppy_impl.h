#ifndef SHARED_FLOPPY_IMPL
#define SHARED_FLOPPY_IMPL

// this file is included by floppy.h, do not include

enum fpy_io {
  FPY_DOR   = 0x3f2,
  FPY_MSR   = 0x3f4,
  FPY_DSR   = 0x3f4,
  FPY_FIFO  = 0x3f5,
  FPY_CTRL  = 0x3f7,
};

enum fpy_cmd {
  FPY_CMD_READ_TRACK          = 0x2,
  FPY_CMD_FIX_DRIVE_DATA      = 0x3,
  FPY_CMD_CHECK_STAT          = 0x4,
  FPY_CMD_WRITE_SECT          = 0x5,
  FPY_CMD_READ_SECT           = 0x6,
  FPY_CMD_CALIBRATE           = 0x7,
  FPY_CMD_CHECK_INT           = 0x8,
  FPY_CMD_WRITE_DELETED_SECT  = 0x9,
  FPY_CMD_READ_SECTOR_ID      = 0xa,
  FPY_CMD_READ_DELETED_SECT   = 0xc,
  FPY_CMD_FORMAT_TRACK        = 0xd,
  FPY_CMD_SEEK                = 0xf,
  FPY_CMD_VERSION             = 0x10,
  FPY_CMD_CONFIGURE           = 0x13,
  FPY_CMD_UNLOCK              = 0x14,
  FPY_CMD_LOCK                = 0x94,
};

enum fpy_cmd_ext {
  FPY_CMD_EXT_SKIP        = 0x20,
  FPY_CMD_EXT_DENSITY     = 0x40,
  FPY_CMD_EXT_MULTITRACK  = 0x80,
};

enum fpy_dor_mask {
  FPY_DOR_MASK_DRIVE0       = 0,
  FPY_DOR_MASK_DRIVE1       = 1,
  FPY_DOR_MASK_DRIVE2       = 2,
  FPY_DOR_MASK_DRIVE3       = 3,
  FPY_DOR_MASK_RESET        = 4,
  FPY_DOR_MASK_DMA          = 8,
  FPY_DOR_MASK_DRIVE0_MOTOR = 16,
  FPY_DOR_MASK_DRIVE1_MOTOR = 32,
  FPY_DOR_MASK_DRIVE2_MOTOR = 64,
  FPY_DOR_MASK_DRIVE3_MOTOR = 128,
};

struct fpy_msr {
  uint8_t drive1_pos_mode : 1;
  uint8_t drive2_pos_mode : 1;
  uint8_t drive3_pos_mode : 1;
  uint8_t drive4_pos_mode : 1;
  uint8_t busy : 1;
  uint8_t ndma : 1;
  uint8_t dataio : 1;
  uint8_t mrq : 1;
} __attribute__((packed));

enum fpy_st0_intcode_typ {
	FPY_ST0_TYP_NORMAL		    =	0,
	FPY_ST0_TYP_ABNORMAL_ERR  =	1,
	FPY_ST0_TYP_INVALID_ERR	  =	2,
	FPY_ST0_TYP_NOTREADY	    =	3
};

enum fpy_gap3_length {
	FPY_GAP3_LENGTH_STD = 42,
	FPY_GAP3_LENGTH_5_14= 32,
	FPY_GAP3_LENGTH_3_5= 27
};

enum fpy_sector_dtl {
	FPY_SECTOR_DTL_128	=	0,
	FPY_SECTOR_DTL_256	=	1,
	FPY_SECTOR_DTL_512	=	2,
	FPY_SECTOR_DTL_1024	=	4
};


/* ==== UTILITY ==== */
static inline struct fpy_msr fpy_read_status(void)
{
  uint8_t a = do_inb(FPY_MSR);
  return *(struct fpy_msr*)&a;
}

static inline uint8_t fpy_get_dor_mask_drive(void)
{
  switch (drive_number)
  {
    case 0: return FPY_DOR_MASK_DRIVE0_MOTOR;
    case 1: return FPY_DOR_MASK_DRIVE1_MOTOR;
    case 2: return FPY_DOR_MASK_DRIVE2_MOTOR;
    case 3: return FPY_DOR_MASK_DRIVE3_MOTOR;
  }
  return 0xFF; // TODO :error
}
static inline void fpy_write_dor(uint8_t v) { do_outb(FPY_DOR, v); }
static inline void fpy_write_dsr(uint8_t v) { do_outb(FPY_DSR, v); }
static inline void fpy_disable_controller(void) { fpy_write_dor(0); }
static inline void fpy_enable_controller(void) { fpy_write_dor(FPY_DOR_MASK_RESET | FPY_DOR_MASK_DMA); }
static inline void fpy_write_ccr(uint8_t val) { do_outb(FPY_CTRL, val); }
#define TEST_BIT(VAL, BIT) \
  ((VAL) & (1 << (BIT)))

#define MUST_TRUE(V, BIT, ERR)                                                 \
  do {                                                                         \
  if (!TEST_BIT(V, BIT))                                                       \
    return MK_ERR("bad bit in register " #V " expected bit " #BIT              \
                  " (" ERR ") to be true");                                    \
  } while (0)

#define MUST_FALSE(V, BIT, ERR)                                                \
  do {                                                                         \
  if (TEST_BIT(V, BIT))                                                        \
    return MK_ERR("bad bit in register " #V " expected bit " #BIT              \
                  " (" ERR ") to be false");                                   \
  } while (0)

static inline err_t fpy_check_st0(uint8_t st0)
{
  MUST_FALSE(st0, 3, "NR");
  uint8_t IC = st0 & 0xC0;
  switch (IC >> 6)
  {
    case 1: return MK_ERR("bad IC value in register st0 : 1 (abnormal termination)");
    case 2: return MK_ERR("bad IC value in register st0 : 2 (invalid command)");
    case 3: return MK_ERR("bad IC value in register st0 : 3 (abnormal termination by polling)");
    default: return NO_ERR;
  }
}
static inline err_t fpy_check_st1(uint8_t st1)
{
  MUST_FALSE(st1, 5, "DE"); /* DE */
  MUST_FALSE(st1, 4, "TO"); /* TO */
  MUST_FALSE(st1, 2, "NDAT"); /* NDAT */
  MUST_FALSE(st1, 1, "NW"); /* NW */
  MUST_FALSE(st1, 0, "NID"); /* NID */
  return NO_ERR;
}
static inline err_t fpy_check_st2(uint8_t st2)
{
  MUST_FALSE(st2, 5, "CRCE"); /* CRCE */
  MUST_FALSE(st2, 4, "WCYL"); /* WCYL */
  MUST_FALSE(st2, 1, "BCYL"); /* BCYL */
  MUST_FALSE(st2, 0, "NDAM"); /* NDAM */
  return NO_ERR;
}
static inline err_t fpy_check_st3(uint8_t st3)
{
  MUST_FALSE(st3, 7, "ESIG"); /* ESIG */
  MUST_TRUE(st3, 5, "RDY"); /* RDY */
  MUST_FALSE(st3, 1, "BCYL"); /* BCYL */
  return NO_ERR;
}
#undef MUST_TRUE
#undef MUST_FALSE
#undef TEST_BIT

static inline uint8_t fpy_read_data()
{
  for (int i = 0; i < 1000; i++) // wait for data register
  {
    if (fpy_read_status().mrq)
      return do_inb(FPY_FIFO);
  }
  return 0xFF; // TODO : real error
}

static inline err_t fpy_send_command_byte(uint8_t cmd)
{
  struct fpy_msr msr;
  for (int i = 0; i < 1000; i++) // wait for data register
  {
    msr = fpy_read_status();
    if (msr.mrq)
    {
      do_outb(FPY_FIFO, cmd);
      return NO_ERR;
    }
  }
  return MK_ERR("timeout");
}

static inline err_t fpy_send_command(uint8_t size, uint8_t *cmd, bool mrq)
{
  struct fpy_msr msr = fpy_read_status();
  if (!msr.mrq || msr.dataio)
    return MK_ERR("bad msr values");
  for (uint8_t i = 0 ; i < size ; i++)
  {
    while (!fpy_read_status().mrq);
    if (msr.dataio)
      return MK_ERR("msr.dataio");
    err_t err;
    if ((err = fpy_send_command_byte(cmd[i])))
      return err;
  }
  if (mrq)
    while (!fpy_read_status().mrq);
  return NO_ERR;
}

/* ==== INTERFACE ==== */
static inline void fpy_motor_disable(void) { fpy_write_dor(FPY_DOR_MASK_RESET); }
static inline void fpy_motor_enable(void)
{
  fpy_write_dor(drive_number | fpy_get_dor_mask_drive() | FPY_DOR_MASK_RESET | FPY_DOR_MASK_DMA);
}

static inline  void fpy_cmd_gen_res(struct fpy_cmd_readtrack_res *res)
{
  res->st0 = fpy_read_data();
  res->error = fpy_check_st0(res->st0);
  if (res->error)
    return ;
  res->st1 = fpy_read_data();
  res->error = fpy_check_st1(res->st1);
  if (res->error)
    return ;
  res->st2 = fpy_read_data();
  res->error = fpy_check_st2(res->st2);
  if (res->error)
    return ;
  res->cylinder = fpy_read_data();
  res->head = fpy_read_data();
  res->sector = fpy_read_data();
  res->sector_size = fpy_read_data();
  res->error = NO_ERR;
}

/* ==== COMMANDS ==== */
static inline struct fpy_cmd_readtrack_res fpy_cmd_readtrack(struct fpy_cmd_readtrack_params *params)
{
  struct fpy_cmd_readtrack_res res;
  uint8_t cmd[9] =
  {
    [0] = FPY_CMD_READ_TRACK
        | (params->double_density ? FPY_CMD_EXT_DENSITY : 0)
        | (params->skip_mode ? FPY_CMD_EXT_SKIP : 0),

    [1] = params->drive
        | (params->head_number << 2),

    [2] = params->cylinder,
    [3] = params->head,
    [4] = params->sector,
    [5] = params->sector_size,
    [6] = params->track_length,
    [7] = params->length_of_gap3,
    [8] = params->data_length
  };

  res.error = fpy_send_command(9, cmd, true);
  if (res.error)
    return res;
  fpy_cmd_gen_res(&res);
  return res;
}

static inline struct fpy_cmd_readtrack_res fpy_cmd_write_sector(struct fpy_cmd_write_sector_params *params)
{
  struct fpy_cmd_readtrack_res res;
  uint8_t cmd[9] =
  {
    [0] = FPY_CMD_WRITE_SECT
        | (params->double_density ? FPY_CMD_EXT_DENSITY : 0)
        | (params->multitrack ? FPY_CMD_EXT_MULTITRACK : 0),

    [1] = params->drive
        | (params->head_number << 2),

    [2] = params->cylinder,
    [3] = params->head,
    [4] = params->sector,
    [5] = params->sector_size,
    [6] = params->track_length,
    [7] = params->length_of_gap3,
    [8] = params->data_length
  };

  res.error = fpy_send_command(9, cmd, true);
  if (res.error)
    return res;
  fpy_cmd_gen_res(&res);
  return res;
}

static inline struct fpy_cmd_readtrack_res fpy_cmd_read_sector(struct fpy_cmd_read_sector_params *params)
{
  struct fpy_cmd_readtrack_res res;
  uint8_t cmd[9] =
  {
    [0] = FPY_CMD_READ_SECT
        | (params->double_density ? FPY_CMD_EXT_DENSITY : 0)
        | (params->multitrack ? FPY_CMD_EXT_MULTITRACK : 0)
        | (params->skip_mode ? FPY_CMD_EXT_SKIP : 0),

    [1] = params->drive
        | (params->head_number << 2),

    [2] = params->cylinder,
    [3] = params->head,
    [4] = params->sector,
    [5] = params->sector_size,
    [6] = params->track_length,
    [7] = params->length_of_gap3,
    [8] = params->data_length
  };
  fpy_prepare_irq_no_handle();
  res.error = fpy_send_command(9, cmd, true);
  if (res.error)
    return res;
  fpy_wait_irq();
  fpy_cmd_gen_res(&res);
  return res;
}

static inline struct fpy_cmd_readtrack_res fpy_cmd_write_deleted_sector(struct fpy_cmd_write_sector_params *params)
{
  struct fpy_cmd_readtrack_res res;
  uint8_t cmd[9] =
  {
    [0] = FPY_CMD_WRITE_DELETED_SECT
        | (params->double_density ? FPY_CMD_EXT_DENSITY : 0)
        | (params->multitrack ? FPY_CMD_EXT_MULTITRACK : 0),

    [1] = params->drive
        | (params->head_number << 2),

    [2] = params->cylinder,
    [3] = params->head,
    [4] = params->sector,
    [5] = params->sector_size,
    [6] = params->track_length,
    [7] = params->length_of_gap3,
    [8] = params->data_length
  };

  res.error = fpy_send_command(9, cmd, true);
  if (res.error)
    return res;
  fpy_cmd_gen_res(&res);
  return res;
}

static inline struct fpy_cmd_readtrack_res fpy_cmd_read_deleted_sector(struct fpy_cmd_read_sector_params *params)
{
  struct fpy_cmd_readtrack_res res;
  uint8_t cmd[9] =
  {
    [0] = FPY_CMD_READ_DELETED_SECT
        | (params->double_density ? FPY_CMD_EXT_DENSITY : 0)
        | (params->multitrack ? FPY_CMD_EXT_MULTITRACK : 0)
        | (params->skip_mode ? FPY_CMD_EXT_SKIP : 0),

    [1] = params->drive
        | (params->head_number << 2),

    [2] = params->cylinder,
    [3] = params->head,
    [4] = params->sector,
    [5] = params->sector_size,
    [6] = params->track_length,
    [7] = params->length_of_gap3,
    [8] = params->data_length
  };

  res.error = fpy_send_command(9, cmd, true);
  if (res.error)
    return res;
  fpy_cmd_gen_res(&res);
  return res;
}

  /*
static inline struct fpy_cmd_format_track_res fpy_cmd_format_track(struct fpy_cmd_format_track_params *params)
{
  struct fpy_cmd_format_track_res res;
  uint8_t cmd[6] =
  {
    [0] = FPY_CMD_FORMAT_TRACK
        | (params->double_density ? FPY_CMD_EXT_DENSITY : 0),

    [1] = params->drive
        | (params->head_number << 2),

    [2] = params->sector_size,
    [3] = params->track_length,
    [4] = params->length_of_gap3,
    [5] = params->fill_byte
  };

  res.success = fpy_send_command(6, cmd, true);
  if (!res.success)
    return res;
  res.track = fpy_read_data();
  res.head = fpy_read_data();
  res.sector = fpy_read_data();
  res.sector_size = fpy_read_data();
  return res;
  // TODO : reimplement
}
  */

static inline err_t fpy_cmd_fix_drive_data(struct fpy_cmd_fix_drive_data_params *params)
{
  uint8_t cmd[3] =
  {
    [0] = FPY_CMD_FIX_DRIVE_DATA,
    [1] = params->head_load_time
        | (params->step_rate << 4),

    [2] = params->ndm
        | (params->head_load_time << 1)
  };
  return fpy_send_command(3, cmd, true);
}

static inline struct fpy_cmd_check_drive_status_res fpy_cmd_check_drive_status(struct fpy_cmd_head_param *params)
{
  struct fpy_cmd_check_drive_status_res res;
  uint8_t cmd[2] =
  {
    [0] = FPY_CMD_CHECK_STAT,
    [1] = params->drive
        | (params->head_number << 2),
  };
  res.error = fpy_send_command(2, cmd, true);
  if (res.error)
    return res;
  res.st3 = fpy_read_data();
  res.error = fpy_check_st3(res.st3);
  return res;
}

static inline err_t fpy_cmd_calibrate_drive(struct fpy_cmd_head_param *params)
{
  uint8_t cmd[2] =
  {
    [0] = FPY_CMD_CALIBRATE,
    [1] = params->drive
        | (params->head_number << 2),
  };
  err_t res = fpy_send_command(2, cmd, false);
  return res;
}

static inline struct fpy_cmd_check_interrupt_res fpy_cmd_check_interrupt(void)
{
  struct fpy_cmd_check_interrupt_res res;
  uint8_t cmd[1] = { FPY_CMD_CHECK_INT };
  res.error = fpy_send_command(1, cmd, true);
  if (res.error)
    return res;
  res.st0 = fpy_read_data();
  res.error = fpy_check_st0(res.st0);
  if (res.error)
    return res;
  res.current_cylinder = fpy_read_data();
  return res;
}

static inline struct fpy_cmd_read_sector_id_res fpy_cmd_read_sector_id(struct fpy_cmd_head_param *params)
{
  struct fpy_cmd_read_sector_id_res res;
  uint8_t cmd[2] =
  {
    [0] = FPY_CMD_READ_SECTOR_ID,
    [1] = params->drive
        | (params->head_number << 2),
  };
  res.succcess = fpy_send_command(2, cmd, false);
  if (!res.succcess)
    return res;
  res.succcess = fpy_wait_irq();
  if (!res.succcess)
    return res;
  res.st0 = fpy_read_data();
  res.succcess = fpy_check_st0(res.st0);
  if (!res.succcess)
    return res;
  res.st1 = fpy_read_data();
  res.succcess = fpy_check_st1(res.st1);
  if (!res.succcess)
    return res;
  res.st2 = fpy_read_data();
  res.succcess = fpy_check_st2(res.st2);
  if (!res.succcess)
    return res;
  res.cylinder = fpy_read_data();
  res.head = fpy_read_data();
  res.sector = fpy_read_data();
  res.sector_size = fpy_read_data();
  return res;
};

static inline err_t fpy_cmd_seek_head(struct fpy_cmd_seek_head_params *params)
{
  uint8_t cmd[3] =
  {
    [0] = FPY_CMD_SEEK,
    [1] = params->drive
        | (params->head_number << 2),
    [2] = params->cylinder
  };
  return fpy_send_command(3, cmd, true);
}

static inline err_t fpy_cmd_check_version(void)
{
  err_t err =  fpy_send_command_byte(FPY_CMD_VERSION);
  if (err)
    return err;
  if (fpy_read_data() == 0x90)
    return NO_ERR;
  else
    return MK_ERR("bad version");
}

static inline err_t fpy_cmd_configure(struct fpy_cmd_configure_params *params)
{
  uint8_t cmd[4] =
  {
    [0] = FPY_CMD_CONFIGURE,
    [1] = 0, // TODO : find more documentation for this byte
    [2] = (params->treshold_val - 1)
        | (params->drive_polling_disable << 4)
        | (params->fifo_disable << 5)
        | (params->implied_seek_enable << 6),
    [3] = params->precomp_val
  };
  return fpy_send_command(4, cmd, true);
}

static inline err_t fpy_cmd_lock(bool lock)
{
  err_t err = fpy_send_command_byte(lock ? FPY_CMD_LOCK : FPY_CMD_UNLOCK);
  if (err)
    return err;
  fpy_read_data(); // TODO : do something with this data
  return err;
}

#endif
