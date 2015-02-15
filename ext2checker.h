typedef unsigned int u32;
typedef int s32;
typedef unsigned short u16;
typedef short s16;

/*
 * Structure of the super block
 */
struct ext2_super_block {
  u32 s_inodes_count;   /* Inodes count */
  u32 s_blocks_count;   /* Blocks count */
  u32 s_r_blocks_count; /* Reserved blocks count */
  u32 s_free_blocks_count;  /* Free blocks count */
  u32 s_free_inodes_count;  /* Free inodes count */
  u32 s_first_data_block; /* First Data Block */
  u32 s_log_block_size; /* Block size */
  s32 s_log_frag_size;  /* Fragment size */
  u32 s_blocks_per_group; /* # Blocks per group */
  u32 s_frags_per_group;  /* # Fragments per group */
  u32 s_inodes_per_group; /* # Inodes per group */
  u32 s_mtime;    /* Mount time */
  u32 s_wtime;    /* Write time */
  u16 s_mnt_count;    /* Mount count */
  s16 s_max_mnt_count;  /* Maximal mount count */
  u16 s_magic;    /* Magic signature */
  u16 s_state;    /* File system state */
  u16 s_errors;   /* Behaviour when detecting errors */
  u16 s_minor_rev_level;  /* minor revision level */
  u32 s_lastcheck;    /* time of last check */
  u32 s_checkinterval;  /* max. time between checks */
  u32 s_creator_os;   /* OS */
  u32 s_rev_level;    /* Revision level */
  u16 s_def_resuid;   /* Default uid for reserved blocks */
  u16 s_def_resgid;   /* Default gid for reserved blocks */
  u32 s_first_ino;    /* First non-reserved inode */
  u16 s_inode_size;     /* size of inode structure */
  u16 s_block_group_nr;   /* block group # of this superblock */
  u32 s_feature_compat;   /* compatible feature set */
  u32 s_feature_incompat;   /* incompatible feature set */
  u32 s_feature_ro_compat;  /* readonly-compatible feature set */
  u32 s_reserved[230];  /* Padding to the end of the block */
};

/*
 * Structure of a blocks group descriptor
 */
struct ext2_group_desc {
  u32 bg_block_bitmap;    /* Blocks bitmap block */
  u32 bg_inode_bitmap;    /* Inodes bitmap block */
  u32 bg_inode_table;   /* Inodes table block */
  u16 bg_free_blocks_count; /* Free blocks count */
  u16 bg_free_inodes_count; /* Free inodes count */
  u16 bg_used_dirs_count; /* Directories count */
  u16 bg_pad;
  u32 bg_reserved[3];
};
