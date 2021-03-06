/*
 * Copyright (c) 2018 FUJITSU LIMITED. All rights reserved.
 * Author(s): Xiao Yang <yangx.jy@cn.fujitsu.com>
 *
 * This program is free software;  you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;  without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program, if not, see <http://www.gnu.org/licenses/>.
 */

#define TST_NO_DEFAULT_MAIN

#include <unistd.h>
#include "tst_test.h"
#include "ksm_helper.h"

void wait_ksmd_full_scan(void)
{
	unsigned long full_scans, at_least_one_full_scan;
	int count = 0;

	SAFE_FILE_SCANF(PATH_KSM "full_scans", "%lu", &full_scans);
	/*
	 * The current scan is already in progress so we can't guarantee that
	 * the get_user_pages() is called on every existing rmap_item if we
	 * only waited for the remaining part of the scan.
	 *
	 * The actual merging happens after the unstable tree has been built so
	 * we need to wait at least two full scans to guarantee merging, hence
	 * wait full_scans to increment by 3 so that at least two full scans
	 * will run.
	 */
	at_least_one_full_scan = full_scans + 3;
	while (full_scans < at_least_one_full_scan) {
		sleep(1);
		count++;
		SAFE_FILE_SCANF(PATH_KSM "full_scans", "%lu", &full_scans);
	}

	tst_res(TINFO, "ksm daemon takes %ds to run two full scans", count);
}
