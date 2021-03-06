/* 超级块数据 */
struct super_block
{
	unsigned bitmap_block; //引导块编号
	unsigned inode_table[1024 - 1]; //inode块编号
};

extern struct super_block sblock;

void super_block_load();
void super_block_save();
int super_block_get_index();
void block_load(unsigned int, char*);
void block_save(unsigned int, char*);
void block_cleanup(unsigned int);
int block_create();