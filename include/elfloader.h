/*
 * elfloader.c - lvx elf loader.
 * 
 * Copyright (C) 2012 jestero
 * 
 * Licensed under the terms of the GNU GPL, version 2
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 */

#ifndef __ELFLOADER_H__
#define __ELFLOADER_H__

int elfloader_sb_load(u8 *buffer_location);
void elfloader_file_launch(const u8 *buffolo);

#endif /* __ELFLOADER_H__ */

