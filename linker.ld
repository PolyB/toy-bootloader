ENTRY(main_asm)
OUTPUT_FORMAT(binary)

SECTIONS
{

  . = 0x7C00;
  /* STAGE 1 */
  .stage1 : {
    *(.stage1*);
  }
  .sign : AT(0x7DFE) { SHORT(0xaa55) }

  .shared 0x7E00 : AT(0x7E00) {
    _shared_start = . ;
    *(.data_shared.load)
    _shared_end_load = . ;
    *(.data_shared.bss)
  }
  _content_start = ALIGN(512);


  /* STAGE 2 */
  .stage2 _content_start : AT(ALIGN((LOADADDR(.shared) + SIZEOF(.shared)), 512)) {
    _stage_2_start = . ;
    *(.stage2*);
    _stage_2_end = .;
  }
  /* STAGE 3 */
  .stage3 _content_start : AT(ALIGN((LOADADDR(.stage2) + SIZEOF(.stage2)), 512)) {
    _stage_3_start = . ;
    *(.stage3*);
    _stage_3_end = .;
  }
  .bss_stage3 (NOLOAD) : {
    _stage_3_bss_start = . ;
    *(.bss.stage3);
    _stage_3_bss_end = . ;
  }

  /* STAGE 4 */
  .stage4 _content_start : AT(ALIGN((LOADADDR(.stage3) + SIZEOF(.stage3)), 512)) {
    _stage_4_start = . ;
    *(.stage4*);
    _stage_4_end = .;
  }
  /* STAGE 5 */
  /* stage5 assumes that stage4 is loaded in memory*/
  .stage5 : AT(ALIGN((LOADADDR(.stage4) + SIZEOF(.stage4)), 512)) {
    _stage_5_start = . ;
    *(.stage5*);
    _stage_5_end = .;
  }
  .bss_stage4 (NOLOAD) : {
    _stage_4_bss_start = . ;
    *(.bss.stage4);
    _stage_4_bss_end = . ;
  }

  .loader_protected 0x7000 : AT(ALIGN((LOADADDR(.stage5) + SIZEOF(.stage5)), 512)) {
    _loader_protected_start = .;
    *(.loader_protected*)
    _loader_protected_end = .;
  }
  ASSERT(SIZEOF(.loader_protected) < 0xE00, "loader_protected is too big !")

}

PROVIDE(_shared_first_section_index = ((LOADADDR(.shared) - 0x7C00) / 512));
PROVIDE(_shared_load_count = (511 + _shared_end_load - _shared_start) / 512);

PROVIDE(_stage_2_first_section_index = ((LOADADDR(.stage2) - 0x7C00) / 512));
PROVIDE(_stage_2_load_count = (511 + _stage_2_end - _stage_2_start) / 512);

PROVIDE(_stage_3_first_section_index = ((LOADADDR(.stage3) - 0x7C00) / 512));
PROVIDE(_stage_3_load_count = (511 + _stage_3_end - _stage_3_start) / 512);

PROVIDE(_stage_4_first_section_index = ((LOADADDR(.stage4) - 0x7C00) / 512));
PROVIDE(_stage_4_size = _stage_4_end - _stage_4_start);

PROVIDE(_loader_protected_size = _loader_protected_end - _loader_protected_start);
PROVIDE(_loader_protected_first_section_index = ((LOADADDR(.loader_protected) - 0x7C00) / 512));

PROVIDE(_stage_5_first_section_index = ((LOADADDR(.stage5) - 0x7C00) / 512));
PROVIDE(_stage_5_size = _stage_5_end - _stage_5_start);

NOCROSSREFS(.stage2 .stage3 .stage4);
NOCROSSREFS(.stage2 .stage3 .stage5);
NOCROSSREFS(.stage1 .loader_protected);
NOCROSSREFS_TO(.stage1 .stage2 .stage3 .stage4 .stage5 .shared);
NOCROSSREFS_TO(.loader_protected .stage4 .stage5 .shared);
NOCROSSREFS_TO(.stage4 .stage5 .shared)

/* TODO : stage 2 & 1 bss */
