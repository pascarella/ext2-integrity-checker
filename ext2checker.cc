#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include "ext2checker.h"

void printSuperBlock(SuperBlock &sb) {
  printf("\nSuperblock from block group %i\n", sb.s_block_group_nr);
  printf("Inodes count:          %15u\n"
         "Blocks count:          %15u\n"
         "Reserved blocks count: %15u\n"
         "Free blocks count:     %15u\n"
         "Free inodes count:     %15u\n"
         "State:                 %15u\n"
         "Block per group:       %15u\n"
         "Magic number:          %15x\n\n",
         sb.s_inodes_count,
         sb.s_blocks_count,
         sb.s_r_blocks_count,
         sb.s_free_blocks_count,
         sb.s_free_inodes_count,
         sb.s_state,
         sb.s_blocks_per_group,
         sb.s_magic);
}

void printBGDT(BlockGroup *bg, u32 totalBlockGroups) {
  printf("Group    Block     Inode      Inode    Free      Free        Used\n"
         "         Bitmap    Bitmap     Table    Blocks    Inodes      Dirs\n"
         "-----------------------------------------------------------------\n");
  for (int i = 0; i < totalBlockGroups; i++) {
    printf("%5d %9u %9u %9u %9u %9u %9u\n",
           i,
           bg[i].bg_block_bitmap,
           bg[i].bg_inode_bitmap,
           bg[i].bg_inode_table,
           bg[i].bg_free_blocks_count,
           bg[i].bg_free_inodes_count,
           bg[i].bg_used_dirs_count);
  }
  printf("\n");
}

void compareSuperBlocks(SuperBlock sb, SuperBlock sbCopy) {
  if (sb.s_inodes_count != sbCopy.s_inodes_count) {
    printf("Error: s_inodes_count\n");
    sbCopy.s_inodes_count = sb.s_inodes_count;
  }
  if (sb.s_blocks_count != sbCopy.s_blocks_count) {
    printf("Error: s_blocks_count \n");
    sbCopy.s_blocks_count = sb.s_blocks_count;
  }
  if (sb.s_r_blocks_count != sbCopy.s_r_blocks_count) {
    printf("Error: s_r_blocks_count \n");
    sbCopy.s_r_blocks_count = sb.s_r_blocks_count;
  }
  if (sb.s_free_blocks_count != sbCopy.s_free_blocks_count) {
    printf("Error: s_free_blocks_count \n");
  }
  if (sb.s_free_inodes_count != sbCopy.s_free_inodes_count) {
    printf("Error: s_free_inodes_count \n");
  }
  if (sb.s_first_data_block != sbCopy.s_first_data_block) {
    printf("Error: s_first_data_block \n");
    sbCopy.s_first_data_block = sb.s_first_data_block;
  }
  if (sb.s_log_block_size != sbCopy.s_log_block_size) {
    printf("Error: s_log_block_size \n");
    sbCopy.s_log_block_size = sb.s_log_block_size;
  }
  if (sb.s_log_frag_size != sbCopy.s_log_frag_size) {
    printf("Error: s_log_frag_size \n");
    sbCopy.s_log_frag_size = sb.s_log_frag_size;
  }
  if (sb.s_blocks_per_group != sbCopy.s_blocks_per_group) {
    printf("Error: s_blocks_per_group \n");
    sbCopy.s_blocks_per_group = sb.s_blocks_per_group;
  }
  if (sb.s_frags_per_group != sbCopy.s_frags_per_group) {
    printf("Error: s_frags_per_group \n");
    sbCopy.s_frags_per_group = sb.s_frags_per_group;
  }
  if (sb.s_inodes_per_group != sbCopy.s_inodes_per_group) {
    printf("Error: s_inodes_per_group \n");
    sbCopy.s_inodes_per_group = sb.s_inodes_per_group;
  }
  if (sb.s_mtime != sbCopy.s_mtime) {
    printf("Error: s_mtime \n");
    sbCopy.s_mtime = sb.s_mtime;
  }
  if (sb.s_wtime != sbCopy.s_wtime) {
    printf("Error: s_wtime \n");
  }
  if (sb.s_mnt_count != sbCopy.s_mnt_count) {
    printf("Error: s_mnt_count \n");
  }
  if (sb.s_max_mnt_count != sbCopy.s_max_mnt_count) {
    printf("Error: s_max_mnt_count \n");
    sbCopy.s_max_mnt_count = sb.s_max_mnt_count;
  }
  if (sb.s_magic != sbCopy.s_magic) {
    printf("Error: s_magic \n");
    sbCopy.s_magic = sb.s_magic;
  }
  if (sb.s_state != sbCopy.s_state) {
    printf("Error: s_state \n");
  }
  if (sb.s_errors != sbCopy.s_errors) {
    printf("Error: s_errors \n");
    sbCopy.s_errors = sb.s_errors;
  }
  if (sb.s_minor_rev_level != sbCopy.s_minor_rev_level) {
    printf("Error: s_minor_rev_level \n");
    sbCopy.s_minor_rev_level = sb.s_minor_rev_level;
  }
  if (sb.s_lastcheck != sbCopy.s_lastcheck) {
    printf("Error: s_lastcheck \n");
    sbCopy.s_lastcheck = sb.s_lastcheck;
  }
  if (sb.s_checkinterval != sbCopy.s_checkinterval) {
    printf("Error: s_checkinterval \n");
    sbCopy.s_checkinterval = sb.s_checkinterval;
  }
  if (sb.s_creator_os != sbCopy.s_creator_os) {
    printf("Error: s_creator_os \n");
    sbCopy.s_creator_os = sb.s_creator_os;
  }
  if (sb.s_rev_level != sbCopy.s_rev_level) {
    printf("Error: s_rev_level \n");
    sbCopy.s_rev_level = sb.s_rev_level;
  }
  if (sb.s_def_resuid != sbCopy.s_def_resuid) {
    printf("Error: s_def_resuid \n");
    sbCopy.s_def_resuid = sb.s_def_resuid;
  }
  if (sb.s_def_resgid != sbCopy.s_def_resgid) {
    printf("Error: s_def_resgid \n");
    sbCopy.s_def_resgid = sb.s_def_resgid;
  }
  if (sb.s_first_ino != sbCopy.s_first_ino) {
    printf("Error: s_first_ino \n");
    sbCopy.s_first_ino = sb.s_first_ino;
  }
  if (sb.s_inode_size != sbCopy.s_inode_size) {
    printf("Error: s_inode_size \n");
    sbCopy.s_inode_size = sb.s_inode_size;
  }
  if (sb.s_block_group_nr != sbCopy.s_block_group_nr) {
    printf("Error: s_block_group_nr \n");
  }
  if (sb.s_feature_compat != sbCopy.s_feature_compat) {
    printf("Error: s_feature_compat \n");
    sbCopy.s_feature_compat = sb.s_feature_compat;
  }
  if (sb.s_feature_incompat != sbCopy.s_feature_incompat) {
    printf("Error: s_feature_incompat \n");
    sbCopy.s_feature_incompat = sb.s_feature_incompat;
  }
  if (sb.s_feature_ro_compat != sbCopy.s_feature_ro_compat) {
    printf("Error: s_feature_ro_compat \n");
    sbCopy.s_feature_ro_compat = sb.s_feature_ro_compat;
  }
}

void compareBlockGroupTableCopies(BlockGroup &bg, BlockGroup &bgCopy) {
  if(bg.bg_block_bitmap != bgCopy.bg_block_bitmap) {
    printf("Error: bg_block_bitmap \n");
    bgCopy.bg_block_bitmap = bg.bg_block_bitmap;
  }
  if(bg.bg_inode_bitmap != bgCopy.bg_inode_bitmap) {
    printf("Error: bg_inode_bitmap \n");
    bgCopy.bg_inode_bitmap = bg.bg_inode_bitmap;
  }
  if(bg.bg_inode_table != bgCopy.bg_inode_table) {
    printf("Error: bg_inode_table \n");
    bgCopy.bg_inode_table = bg.bg_inode_table;
  }
  if(bg.bg_free_blocks_count != bgCopy.bg_free_blocks_count) {
    printf("Error: bg_free_blocks_count \n");
    bgCopy.bg_free_blocks_count = bg.bg_free_blocks_count;
  }
  if(bg.bg_free_inodes_count != bgCopy.bg_free_inodes_count) {
    printf("Error: bg_free_inodes_count \n");
    bgCopy.bg_free_inodes_count = bg.bg_free_inodes_count;
  }
  if(bg.bg_used_dirs_count != bgCopy.bg_used_dirs_count) {
    printf("Error: bg_used_dirs_count \n");
    bgCopy.bg_used_dirs_count = bg.bg_used_dirs_count;
  }
}

bool isPower(u32 x, u32 y) {
  while(x > 1) {
    if (x % y != 0) {
      return false;
    }
    x /= y;
  }
  return true;
}

bool isSparse(int groupNumber) {
  // backups stored in 0, 1, and powers of 3, 5, and 7
  if (isPower(groupNumber, 7) || isPower(groupNumber, 5) ||
      isPower(groupNumber, 3) || groupNumber == 1 || groupNumber == 0) {
    return true;
  } else {
    return false;
  }
 }

void checkSuperBlockCopies(int fs, SuperBlock sb, u32 totalBlockGroups) {
  struct SuperBlock sbCopy;
  u32 blockSize = 1024 << sb.s_log_block_size;
  printf("----------------------------");
  printf("\nTesting Superblock copies...\n");
  printf("----------------------------");
  for (int i = 0; i < totalBlockGroups; i++) {
    if (isSparse(i)) {
      printf("\nBlock group: %i\n", i);
      lseek(fs, i * blockSize * sb.s_blocks_per_group + 1024, SEEK_SET);
      read(fs, &sbCopy, sizeof(sbCopy));
      compareSuperBlocks(sb, sbCopy);
    }
  }
}

void checkBlockGroupTableCopies(int fs, SuperBlock sb, BlockGroup *bg) {
  u32 blockSize = 1024 << sb.s_log_block_size;
  u32 blocksPerGroup = sb.s_blocks_per_group;
  u32 totalBlockGroups = ceil(sb.s_blocks_count/sb.s_blocks_per_group);

  struct BlockGroup *bgCopy;
  bgCopy = (BlockGroup*) malloc(totalBlockGroups * sizeof(BlockGroup));
  printf("\n----------------------------------");
  printf("\nTesting Block Group copies...\n");
  printf("----------------------------------");
  for (int i = 0; i < totalBlockGroups; i++) {
    if (isSparse(i)) {
      printf("\nBlock group: %i \n", i);
      if (blockSize == 1024) {
        lseek(fs, i * blockSize * sb.s_blocks_per_group + 2048, SEEK_SET);
      } else {
        lseek(fs, i * blockSize * sb.s_blocks_per_group + blockSize, SEEK_SET);
      }
      for (int j = 0; j < totalBlockGroups; j++) {
        read(fs, &bgCopy[j], sizeof(BlockGroup));
        compareBlockGroupTableCopies(bgCopy[j], bgCopy[j]);
      }
    }
  }
}

void printRootInode(Inode *inode) {
  printf("\nReading root inode (2)\n"
         "Owner UID:  %5hu\n"
         "Size:       %5u bytes\n"
         "1/2K Blocks:%5u\n"
         "Ref count:  %5u\n",
         inode->i_uid,
         inode->i_size,
         inode->i_blocks,
         inode->i_links_count);

  for (int i = 0; i < EXT2_N_BLOCKS; i++) {
    // direct blocks (entries 1-12)
    if (i < EXT2_NDIR_BLOCKS) {
      printf("Block %2u:   %5u\n", i, inode->i_block[i]);
    }
    // single indirect (13th entry)
    else if (i == EXT2_IND_BLOCK) {
      printf("Single:     %5u\n", inode->i_block[i]);
    }
    // double indirect (14th entry)
    else if (i == EXT2_DIND_BLOCK) {
      printf("Double:     %5u\n", inode->i_block[i]);
    }
    // triple indirect (15th entry)
    else if (i == EXT2_TIND_BLOCK) {
      printf("Triple:     %5u\n\n", inode->i_block[i]);
    }
  }
}

void readRootInode(int fs, int inodeNo, SuperBlock *sb, BlockGroup *bg,
                   Inode *inode, u32 blockSize) {
  u32 inodeGroup = (inodeNo-1)/sb->s_inodes_per_group;
  u32 fInode = (1024 + ((bg[inodeGroup].bg_inode_table)-1) * blockSize);
  lseek(fs, fInode + (inodeNo-1) * sizeof(Inode), SEEK_SET);
  read(fs, inode, sizeof(Inode));
}

int main(int argc, char *argv[]) {
  struct SuperBlock sb;
  struct BlockGroup *bg;
  struct Inode inode;

  int fs = open(argv[1], O_RDONLY);
  assert(fs != 0);
  // read superblock
  lseek(fs, 1024, SEEK_SET);
  read(fs, &sb, sizeof(sb));
  // check if magic number is correct (0xEF53)
  assert(sb.s_magic == 61267);
  u32 blockSize = 1024 << sb.s_log_block_size;
  // calculate filesystem size
  u32 fsSize = sb.s_blocks_count * blockSize;
  printf("\nTotal size of filesystem:    %u bytes\n", fsSize);
  // calculate free space
  u32 freeSpace = blockSize * sb.s_free_blocks_count;
  printf("Free space:                   %u bytes\n", freeSpace);
  // calculate used space
  u32 usedSpace = blockSize * (sb.s_blocks_count - sb.s_free_blocks_count);
  printf("Used space:                   %u bytes\n", usedSpace);
  // calculate total number of block groups
  u32 totalBlockGroups = ceil(sb.s_blocks_count/sb.s_blocks_per_group);
  bg = (BlockGroup*) malloc(totalBlockGroups * sizeof(BlockGroup));
  // read block group descriptor table
  if (blockSize == 1024) {
    lseek(fs, 1024 + blockSize, SEEK_SET);
  } else {
    lseek(fs, blockSize, SEEK_SET);
  }
  for (int i = 0; i < totalBlockGroups; i++) {
    read(fs, &bg[i], sizeof(BlockGroup));
  }

  // calculate number of inodes per block
  u32 inodesPerBlock = blockSize / sizeof(Inode);
  printf("inodesPerBlock: %u\n", inodesPerBlock);
  // calculate size in blocks of the inode table
  u32 itableBlocks = sb.s_inodes_per_group / inodesPerBlock;
  printf("itableBlocks: %u\n", itableBlocks);
  u32 inodeSize = sizeof(Inode);

  printSuperBlock(sb);
  printBGDT(bg, totalBlockGroups);
  checkSuperBlockCopies(fs, sb, totalBlockGroups);
  checkBlockGroupTableCopies(fs, sb, bg);
  readRootInode(fs, 2, &sb, bg, &inode, blockSize);
  printRootInode(&inode);

  close(fs);
  return 0;
}


