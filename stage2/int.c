#include "int.h"

struct err {
  uint8_t err;
  const char *msg;
};

static struct err errs[]  = {
  {0x00,"successful completion" },
  {0x01,"invalid function in AH or invalid parameter" },
  {0x02,"address mark not found" },
  {0x03,"disk write-protected" },
  {0x04,"sector not found/read error" },
  {0x05,"reset failed (hard disk)" },
  {0x05,"data did not verify correctly (TI Professional PC)" },
  {0x06,"disk changed (floppy)" },
  {0x07,"drive parameter activity failed (hard disk)" },
  {0x08,"DMA overrun" },
  {0x09,"data boundary error (attempted DMA across 64K boundary or >80h sectors)" },
  {0x0A,"bad sector detected (hard disk)" },
  {0x0B,"bad track detected (hard disk)" },
  {0x0C,"unsupported track or invalid media" },
  {0x0D,"invalid number of sectors on format (PS/2 hard disk)" },
  {0x0E,"control data address mark detected (hard disk)" },
  {0x0F,"DMA arbitration level out of range (hard disk)" },
  {0x10,"uncorrectable CRC or ECC error on read" },
  {0x11,"data ECC corrected (hard disk)" },
  {0x20,"controller failure" },
  {0x31,"no media in drive (IBM/MS INT 13 extensions)" },
  {0x32,"incorrect drive type stored in CMOS (Compaq)" },
  {0x40,"seek failed" },
  {0x80,"timeout (not ready)" },
  {0xAA,"drive not ready (hard disk)" },
  {0xB0,"volume not locked in drive (INT 13 extensions)" },
  {0xB1,"volume locked in drive (INT 13 extensions)" },
  {0xB2,"volume not removable (INT 13 extensions)" },
  {0xB3,"volume in use (INT 13 extensions)" },
  {0xB4,"lock count exceeded (INT 13 extensions)" },
  {0xB5,"valid eject request failed (INT 13 extensions)" },
  {0xB6,"volume present but read protected (INT 13 extensions)" },
  {0xBB,"undefined error (hard disk)" },
  {0xCC,"write fault (hard disk)" },
  {0xE0,"status register error (hard disk)" },
  {0xFF,"sense operation failed (hard disk)" },
};

const char *errc(uint8_t code)
{
  for (uint16_t i = 0; i < sizeof(errs) / sizeof(*errs); i++)
  {
    if (errs[i].err == code)
      return errs[i].msg;
  }
  return "no error for this code";
}
