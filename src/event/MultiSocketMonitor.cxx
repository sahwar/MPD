/*
 * Copyright (C) 2003-2013 The Music Player Daemon Project
 * http://www.musicpd.org
 *
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "config.h"
#include "MultiSocketMonitor.hxx"

MultiSocketMonitor::MultiSocketMonitor(EventLoop &_loop)
	:IdleMonitor(_loop), TimeoutMonitor(_loop), ready(false) {
}

MultiSocketMonitor::~MultiSocketMonitor()
{
	// TODO
}

void
MultiSocketMonitor::Prepare()
{
	int timeout_ms = PrepareSockets();
	if (timeout_ms >= 0)
		TimeoutMonitor::Schedule(timeout_ms);
	else
		TimeoutMonitor::Cancel();

}

void
MultiSocketMonitor::OnIdle()
{
	if (ready) {
		ready = false;
		DispatchSockets();

		/* TODO: don't refresh always; require users to call
		   InvalidateSockets() */
		refresh = true;
	}

	if (refresh) {
		refresh = false;
		Prepare();
	}
}
