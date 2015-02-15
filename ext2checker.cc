#include <cassert>
#include <iostream>
#include <fcntl.h>
#include "ext2checker.h"

using namespace std;

auto printSuperBlock(ext2_super_block &sb) -> void {
  unsigned int block_size;
  block_size = 1024 << sb.s_log_block_size;
  printf("Superblock from block group %i\n\n", sb.s_block_group_nr);
  printf("Inodes count:          %15u\n"
         "Blocks count:          %15u\n"
         "Reserved blocks count: %15u\n"
         "Free blocks count:     %15u\n"
         "Free inodes count:     %15u\n"
         "Block size:            %15u\n"
         "Magic number:          %15x\n"
         "------------------------------------------------------\n",
         sb.s_inodes_count,
         sb.s_blocks_count,
         sb.s_r_blocks_count,
         sb.s_free_blocks_count,
         sb.s_free_inodes_count,
         block_size,
         sb.s_magic);
}

auto readSuperBlock(FILE *fs, ext2_super_block &sb) -> void {
  u32 *u32Buffer = new u32;
  u16 *u16Buffer = new u16;

  fread(u32Buffer, 4, 1, fs);
  sb.s_inodes_count = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_blocks_count = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_r_blocks_count = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_free_blocks_count = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_free_inodes_count = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_first_data_block = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_log_block_size = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_log_frag_size = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_blocks_per_group = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_frags_per_group = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_inodes_per_group = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_mtime = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_wtime = *u32Buffer;

  fread(u16Buffer, 2, 1, fs);
  sb.s_mnt_count = *u16Buffer;

  fread(u16Buffer, 2, 1, fs);
  sb.s_max_mnt_count = *u16Buffer;

  fread(u16Buffer, 2, 1, fs);
  sb.s_magic = *u16Buffer;

  fread(u16Buffer, 2, 1, fs);
  sb.s_state = *u16Buffer;

  fread(u16Buffer, 2, 1, fs);
  sb.s_errors = *u16Buffer;

  fread(u16Buffer, 2, 1, fs);
  sb.s_minor_rev_level = *u16Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_lastcheck = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_checkinterval = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_creator_os = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_rev_level = *u32Buffer;

  fread(u16Buffer, 2, 1, fs);
  sb.s_def_resuid = *u16Buffer;

  fread(u16Buffer, 2, 1, fs);
  sb.s_def_resgid = *u16Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_first_ino = *u32Buffer;

  fread(u16Buffer, 2, 1, fs);
  sb.s_inode_size = *u16Buffer;

  fread(u16Buffer, 2, 1, fs);
  sb.s_block_group_nr = *u16Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_feature_compat = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_feature_incompat = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  sb.s_feature_ro_compat = *u32Buffer;
}

auto main(int argc, char *argv[]) -> int {
  struct ext2_super_block sb;

  FILE *fs = fopen(argv[1], "rb");
  assert(fs != NULL);

  fseek(fs, 0, SEEK_END);
  unsigned long fsSize = ftell(fs);
  printf("Size of filesystem: %ld bytes.\n", fsSize);

  fseek(fs, 1024, SEEK_SET);
  readSuperBlock(fs, sb);
  printSuperBlock(sb);

  return 0;
}
