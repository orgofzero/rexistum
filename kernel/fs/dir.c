#include <kernel/fs/file.h>
#include <kernel/fs/inode.h>
#include <kernel/fs/path.h>
#include <kernel/string.h>

extern struct inode inode_list[INODE_NUM];
extern int inode_count;
/* 创建文件夹 */
void dir_create(char *path)
{
	struct file dir;
	if(file_create(&dir, path) != -1)
	{
		inode_list[dir.inode].type = TYPE_DIRECTOR; //类型更改为文件夹
		inode_save();
	}
}
/* 获取文件inode编号 */
int dir_get_inode(char *path)
{
	if(str_cmp(path, "/") == 1)
	{
		return 0;
	}
	if(path_exist(path) == 0)
	{
		return -1;
	}
	char now_name[20];
	int times = 0;
	int i = 0;
	int now = 0;
	for(; times < str_count(path, "/"); times++)
	{
		str_split(now_name, path, "/", times + 1); //获取目录名
		for(i = 1; i < inode_count; i++)
		{
			if(str_cmp(inode_list[i].name, now_name) == 1)
			{
				if(inode_list[i].parent_inode == now)
				{
					now = i;
				}
			}
		}
	}
	return now;
}
/* 列出子目录及文件inode */
int dir_list_inode(int *ret, char *path)
{
	int inode = dir_get_inode(path);
	int count = 0;
	int i = 0;
	for(i = 1; i < inode_count; i++)
	{
		if(inode_list[i].parent_inode == inode)
		{
			if(inode_list[i].type == TYPE_AVAILABLE)
			{
				continue;
			}
			ret[count] = i;
			count += 1;
		}
	}
	return count;
}