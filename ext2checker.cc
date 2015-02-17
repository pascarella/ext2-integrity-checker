#include <cassert>
#include <iostream>
#include "ext2checker.h"

auto printSuperBlock(ext2_super_block &sb) -> void {
  u32 block_size;
  block_size = 1024 << sb.s_log_block_size;
  printf("\nSuperblock from block group %i\n", sb.s_block_group_nr);
  printf("Inodes count:          %15u\n"
         "Blocks count:          %15u\n"
         "Reserved blocks count: %15u\n"
         "Free blocks count:     %15u\n"
         "Free inodes count:     %15u\n"
         "Block size:            %15u\n"
         "State:                 %15u\n"
         "Magic number:          %15x\n\n",
         sb.s_inodes_count,
         sb.s_blocks_count,
         sb.s_r_blocks_count,
         sb.s_free_blocks_count,
         sb.s_free_inodes_count,
         block_size,
         sb.s_state,
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

auto readBlockGroupDescriptorTable(FILE *fs, ext2_group_desc &bgdt) -> void {
  u32 *u32Buffer = new u32;
  u16 *u16Buffer = new u16;

  fread(u32Buffer, 4, 1, fs);
  bgdt.bg_block_bitmap = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  bgdt.bg_inode_bitmap = *u32Buffer;

  fread(u32Buffer, 4, 1, fs);
  bgdt.bg_inode_table = *u32Buffer;

  fread(u16Buffer, 2, 1, fs);
  bgdt.bg_free_blocks_count = *u16Buffer;

  fread(u16Buffer, 2, 1, fs);
  bgdt.bg_free_inodes_count = *u16Buffer;

  fread(u16Buffer, 2, 1, fs);
  bgdt.bg_used_dirs_count = *u16Buffer;
}

auto main(int argc, char *argv[]) -> int {
  struct ext2_super_block sb;

  FILE *fs = fopen(argv[1], "rb");
  assert(fs != NULL);
  // set the fs position with offset 1024
  fseek(fs, 1024, SEEK_SET);
  readSuperBlock(fs, sb);
  // check if magic number is correct (0xEF53)
  assert(sb.s_magic == 61267);
  // calculate filesystem size
  u32 blockSize = 1024 << sb.s_log_block_size;
  u32 fsSize = sb.s_blocks_count * blockSize;
  printf("\nTotal size of filesystem: %u bytes.\n", fsSize);
  // calculate free space
  u32 freeSpace = blockSize * sb.s_free_blocks_count;
  printf("Free space: %u bytes.\n", freeSpace);
  // calculate used space
  u32 usedSpace = blockSize * (sb.s_blocks_count - sb.s_free_blocks_count);
  printf("Used space: %u bytes.\n", usedSpace);
  printSuperBlock(sb);

  // calculate total number of block groups
  u32 totalGroups = (sb.s_blocks_count/sb.s_blocks_per_group);
  // initialize block group descriptor table array
  struct ext2_group_desc bgdt[totalGroups-1];
  for (int i = 0; i < totalGroups; i++) {
    fseek(fs, (2048 + (i * 32)), SEEK_SET);
    readBlockGroupDescriptorTable(fs, bgdt[i]);
  }
  // print out block group descriptor table
  printf("Group    Block     Inode      Inode    Free      Free        Used\n"
       "         Bitmap    Bitmap     Table    Blocks    Inodes      Dirs\n"
       "-------------------------------------------------------------------\n");
  for (int j = 0; j < totalGroups; j++) {
      printf("%5d %9u %9u %9u %9u %9u %9u \n",
      j,
      bgdt[j].bg_block_bitmap,
      bgdt[j].bg_inode_bitmap,
      bgdt[j].bg_inode_table,
      bgdt[j].bg_free_blocks_count,
      bgdt[j].bg_free_inodes_count,
      bgdt[j].bg_used_dirs_count);
  }
  printf("\n");
  return 0;
}
