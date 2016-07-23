/* This file is a modification of the evtest source code. this is the
 * original copyright notice:
*
 *  Copyright (c) 1999-2000 Vojtech Pavlik
 *  Copyright (c) 2009-2011 Red Hat, Inc

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Should you need to contact me, the author, you can do so either by
 * e-mail - mail your message to <vojtech@ucw.cz>, or by paper mail:
 * Vojtech Pavlik, Simunkova 1594, Prague 8, 182 00 Czech Republic
 */

#define _GNU_SOURCE /* need this for versionsort */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <linux/input.h>

#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"

/**
 * Filter for the AutoDevProbe scandir on /dev/input.
 *
 * @param dir The current directory entry provided by scandir.
 *
 * @return Non-zero if the given directory entry starts with "event", or zero
 * otherwise.
 */
static int is_event_device(const struct dirent *dir) {
	return strncmp(EVENT_DEV_NAME, dir->d_name, 5) == 0;
}

/**
 * Scans all /dev/input/event* and display them in stdout
 *
 * @return The event device file name of the device file selected. This
 * string is allocated and must be freed by the caller.
 */
static void scan_gamepads(void)
{
	struct dirent **namelist;
	int i, connected_pads;

	connected_pads = scandir(DEV_INPUT_EVENT, &namelist, is_event_device, versionsort);
	if (connected_pads <= 0)
		return;

	printf("Available devices:\n");

	for (i = 0; i < connected_pads; i++)
	{
		char fname[64];
		int fd = -1;
		char name[256] = "???";

		snprintf(fname, sizeof(fname),
			 "%s/%s", DEV_INPUT_EVENT, namelist[i]->d_name);
		fd = open(fname, O_RDONLY);
		if (fd < 0)
			continue;
		ioctl(fd, EVIOCGNAME(sizeof(name)), name);

		printf("%s:	%s\n", fname, name);
		close(fd);

		free(namelist[i]);
	}
}


int main(int argc, char** argv) {
    scan_gamepads();
    return 0;
}
