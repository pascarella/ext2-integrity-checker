#include <iostream>
#include <cassert>
#include <math.h>
#include "ext2checker.h"

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
         "Block per group:       %15u\n"
         "Magic number:          %15x\n\n",
         sb.s_inodes_count,
         sb.s_blocks_count,
         sb.s_r_blocks_count,
         sb.s_free_blocks_count,
         sb.s_free_inodes_count,
         block_size,
         sb.s_state,
         sb.s_blocks_per_group,
         sb.s_magic);
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

auto printBlockGroupDescriptorTable(FILE *fs, u32 totalBlockGroups) -> void {
  struct ext2_group_desc bgdt[totalBlockGroups];
  for (int i = 0; i < totalBlockGroups; i++) {
    fseek(fs, (i * 32) + 2048, SEEK_SET);
    readBlockGroupDescriptorTable(fs, bgdt[i]);
  }
  printf("Group    Block     Inode      Inode    Free      Free        Used\n"
         "         Bitmap    Bitmap     Table    Blocks    Inodes      Dirs\n"
         "-----------------------------------------------------------------\n");
  for (int j = 0; j < totalBlockGroups; j++) {
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
}

auto compareSuperBlock(ext2_super_block sb, ext2_super_block sbCopy) -> void {
  if (sb.s_inodes_count != sbCopy.s_inodes_count) {
    printf("Error: s_inodes_count\n");
  }
  if (sb.s_blocks_count != sbCopy.s_blocks_count) {
    printf("Error: s_blocks_count \n");
  }
  if (sb.s_r_blocks_count != sbCopy.s_r_blocks_count) {
    printf("Error: s_r_blocks_count \n");
  }
  if (sb.s_free_blocks_count != sbCopy.s_free_blocks_count) {
    printf("Error: s_free_blocks_count \n");
  }
  if (sb.s_free_inodes_count != sbCopy.s_free_inodes_count) {
    printf("Error: s_free_inodes_count \n");
  }
  if (sb.s_first_data_block != sbCopy.s_first_data_block) {
    printf("Error: s_first_data_block \n");
  }
  if (sb.s_log_block_size != sbCopy.s_log_block_size) {
    printf("Error: s_log_block_size \n");
  }
  if (sb.s_log_frag_size != sbCopy.s_log_frag_size) {
    printf("Error: s_log_frag_size \n");
  }
  if (sb.s_blocks_per_group!= sbCopy.s_blocks_per_group) {
    printf("Error: s_blocks_per_group \n");
  }
  if (sb.s_frags_per_group != sbCopy.s_frags_per_group) {
    printf("Error: s_frags_per_group \n");
  }
  if (sb.s_inodes_per_group != sbCopy.s_inodes_per_group) {
    printf("Error: s_inodes_per_group \n");
  }
  if (sb.s_mtime != sbCopy.s_mtime) {
    printf("Error: s_mtime \n");
  }
  if (sb.s_wtime != sbCopy.s_wtime) {
    printf("Error: s_wtime \n");
  }
  if (sb.s_mnt_count != sbCopy.s_mnt_count) {
    printf("Error: s_mnt_count \n");
  }
  if (sb.s_max_mnt_count != sbCopy.s_max_mnt_count) {
    printf("Error: s_max_mnt_count \n");
  }
  if (sb.s_magic != sbCopy.s_magic) {
    printf("Error: s_magic \n");
  }
  if (sb.s_state != sbCopy.s_state) {
    printf("Error: s_state \n");
  }
  if (sb.s_errors != sbCopy.s_errors) {
    printf("Error: s_errors \n");
  }
  if (sb.s_minor_rev_level != sbCopy.s_minor_rev_level) {
    printf("Error: s_minor_rev_level \n");
  }
  if (sb.s_lastcheck != sbCopy.s_lastcheck) {
    printf("Error: s_lastcheck \n");
  }
  if (sb.s_checkinterval != sbCopy.s_checkinterval) {
    printf("Error: s_checkinterval \n");
  }
  if (sb.s_creator_os != sbCopy.s_creator_os) {
    printf("Error: s_creator_os \n");
  }
  if (sb.s_rev_level != sbCopy.s_rev_level) {
    printf("Error: s_rev_level \n");
  }
  if (sb.s_def_resuid != sbCopy.s_def_resuid) {
    printf("Error: s_def_resuid \n");
  }
  if (sb.s_def_resgid != sbCopy.s_def_resgid) {
    printf("Error: s_def_resgid \n");
  }
  if (sb.s_first_ino != sbCopy.s_first_ino) {
    printf("Error: s_first_ino \n");
  }
  if (sb.s_inode_size != sbCopy.s_inode_size) {
    printf("Error: s_inode_size \n");
  }
  if (sb.s_block_group_nr != sbCopy.s_block_group_nr) {
    printf("Error: s_block_group_nr \n");
  }
  if (sb.s_feature_compat != sbCopy.s_feature_compat) {
    printf("Error: s_feature_compat \n");
  }
  if (sb.s_feature_incompat != sbCopy.s_feature_incompat) {
    printf("Error: s_feature_incompat \n");
  }
  if (sb.s_feature_ro_compat != sbCopy.s_feature_ro_compat) {
    printf("Error: s_feature_ro_compat \n");
  }
}

auto isSparse(int groupNumber) -> bool {
  // backups stored in 0, 1, and powers of 3, 5, and 7
  // TODO: create a function that will check if a number is a power of another
  //       number.
  if (groupNumber == 9 | groupNumber == 7 | groupNumber == 5 |
      groupNumber == 3 | groupNumber == 1 | groupNumber == 0) {
    return true;
  } else {
    return false;
  }
}

auto checkSuperBlock(FILE *fs, ext2_super_block &sb) -> void {
  struct ext2_super_block sbCopy;
  u32 blockSize = 1024 << sb.s_log_block_size;
  u32 blocksPerGroup = sb.s_blocks_per_group;
  u32 totalBlockGroups = (sb.s_blocks_count/sb.s_blocks_per_group);
  for (int i = 0; i < totalBlockGroups; i++) {
    if (isSparse(i)) {
      printf("\nBlock group: %i \n", i);
      fseek(fs, ((i * blockSize) * blocksPerGroup + blockSize), SEEK_SET);
      readSuperBlock(fs, sbCopy);
      compareSuperBlock(sb, sbCopy);
    }
  }
}

auto main(int argc, char *argv[]) -> int {
  struct ext2_super_block sb;
  FILE *fs = fopen(argv[1], "rb");
  assert(fs != NULL);
  fseek(fs, 1024, SEEK_SET);
  readSuperBlock(fs, sb);
  // check if magic number is correct (0xEF53)
  assert(sb.s_magic == 61267);
  // calculate filesystem size
  u32 blockSize = 1024 << sb.s_log_block_size;
  u32 fsSize = sb.s_blocks_count * blockSize;
  printf("\nTotal size of filesystem:    %u bytes\n", fsSize);
  // calculate free space
  u32 freeSpace = blockSize * sb.s_free_blocks_count;
  printf("Free space:                   %u bytes\n", freeSpace);
  // calculate used space
  u32 usedSpace = blockSize * (sb.s_blocks_count - sb.s_free_blocks_count);
  printf("Used space:                   %u bytes\n", usedSpace);
  printSuperBlock(sb);
  // calculate total number of block groups
  // round up the total # of blocks divided by the # of blocks per block group
  u32 totalBlockGroups = ceil(sb.s_blocks_count/sb.s_blocks_per_group);
  printBlockGroupDescriptorTable(fs, totalBlockGroups);
  checkSuperBlock(fs, sb);
}
