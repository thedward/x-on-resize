/*
 * Copyright © 2011 Keith Packard <keithp@keithp.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

struct output_info {
	struct output_info	*next;
	RROutput		output;
	XRROutputInfo		*info;
};

static struct output_info	*output_info;

static struct output_info *
find_output_info (RROutput output)
{
	struct output_info	*oi;

	for (oi = output_info; oi; oi = oi->next)
		if (oi->output == output)
			return oi;
	return NULL;
}

static void
clear_output_info (RROutput output)
{
	struct output_info	*oi, **prev;

	for (prev = &output_info; (oi = *prev); prev = &(oi->next))
		if (oi->output == output) {
			XRRFreeOutputInfo (oi->info);
			*prev = oi->next;
			free (oi);
			break;
		}
}

/*
 * Check to see if the monitor attached to an output
 * is the same
 */
static int
same_monitor(XRROutputInfo *a, XRROutputInfo *b)
{
	int	m;

	if (a->connection != b->connection)
		return 0;
	if (a->nmode != b->nmode)
		return 0;
	if (a->npreferred != b->npreferred)
		return 0;
	for (m = 0; m < a->nmode; m++)
		if (a->modes[m] != b->modes[m])
			return 0;
	return 1;
}

static int
check_output (Display *dpy, XRRScreenResources *resources, RROutput output)
{
	XRROutputInfo		*info;
	struct output_info	*oi;

	info = XRRGetOutputInfo (dpy, resources, output);
	if (!info) {
		clear_output_info(output);
		return 0;
	}
	oi = find_output_info(output);
	if (oi) {
		int	same = same_monitor(oi->info, info);
		XRRFreeOutputInfo(oi->info);
		oi->info = info;
		return same;
	}
	oi = calloc(1, sizeof (struct output_info));
	oi->output = output;
	oi->info = info;
	oi->next = output_info;
	output_info = oi;
	return 0;
}
	

int
main (int argc, char **argv)
{
	Display		*dpy;
	int		event_base, error_base;
	int		major, minor;
	XEvent		ev;
	XRRNotifyEvent	*nev;
	char		*config = NULL;
	char		*resize = NULL;
	char		*display = NULL;
	int		c, o;
	int		start = 0;
	XRRScreenResources	*resources;

	static struct option opts[] = {
		{ "config", 1, NULL, 'c' },
		{ "resize", 1, NULL, 'r' },
		{ "start", 0, NULL, 's' },
		{ "display", 1, NULL, 'd' },
		{ "help", 0, NULL, 'h' },
		{ 0, 0, NULL, 0 }
	};

	while ((c = getopt_long(argc, argv, "c:r:d:hs", opts, NULL)) != -1) {
		switch (c) {
		case 'c':
			config = optarg;
			break;
		case 'r':
			resize = optarg;
			break;
		case 'd':
			display = optarg;
			break;
		case 's':
			start = 1;
			break;
		case 'h':
		default:
			fprintf(stderr, "Usage: %s --display <display> --config <config> --resize <resize> --start\n", argv[0]);
			exit(1);
			break;
		}
	}

	dpy = XOpenDisplay(display);
	if (!dpy) {
		fprintf(stderr, "XOpenDisplay %s failed\n", XDisplayName(display));
		exit(1);
	}
	if (!XRRQueryExtension (dpy, &event_base, &error_base) ||
	    !XRRQueryVersion (dpy, &major, &minor))
	{
		fprintf (stderr, "RandR extension missing on %s\n", XDisplayName(display));
		exit (1);
	}
	XRRSelectInput(dpy, RootWindow(dpy, 0), RROutputChangeNotifyMask);
	XSelectInput(dpy, RootWindow(dpy, 0), StructureNotifyMask);

	/* Get current configuration */
	resources = XRRGetScreenResourcesCurrent(dpy, RootWindow(dpy, 0));
	for (o = 0; o < resources->noutput; o++)
		(void) check_output(dpy, resources, resources->outputs[o]);
	XRRFreeScreenResources (resources);

	if (start) {
		if (config)
			system(config);
		else
			printf("config\n");
	}
	for (;;) {
		int	configed = 0;
		int	resized = 0;

		do {
			XNextEvent(dpy, &ev);
			switch (ev.type - event_base) {
			case RRNotify:
				nev = (XRRNotifyEvent *) &ev;
				if (nev->subtype == RRNotify_OutputChange) {
					XRROutputChangeNotifyEvent *noev = (XRROutputChangeNotifyEvent *) nev;
					resources = XRRGetScreenResources(dpy, RootWindow(dpy, 0));
					if (!check_output(dpy, resources, noev->output))
						configed = 1;
					XRRFreeScreenResources (resources);
				}
				break;
			}
			switch (ev.type) {
			case ConfigureNotify:
				resized = 1;
				break;
			}
			usleep(100000);
		} while (XEventsQueued(dpy, QueuedAfterFlush));
		if (configed) {
			if (config)
				system(config);
			else
				printf ("config\n");
		}
		if (resized) {
			if (resize)
				system(resize);
			else
				printf ("resize\n");
		}
	}
}
