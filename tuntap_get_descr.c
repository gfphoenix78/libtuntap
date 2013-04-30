/*
 * Copyright (c) 2012-2013 Tristan Le Guern <leguern AT medu DOT se>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "tuntap.h"
#include "tuntap_private.h"

#include <sys/types.h>
#include <sys/ioctl.h>

#if defined HAVE_NET_IF_TUN_H
# include <net/if_tun.h>
#endif

#include <stdlib.h>
#include <string.h>

static char *
tuntap_sys_get_descr(struct device *dev) {
#if defined OpenBSD /* TODO: FreeBSD */
	struct ifreq ifr;
	char buf[4096];
	void *descr;

	descr = buf;
	(void)memset(&ifr, 0, sizeof ifr);
	(void)strlcpy(ifr.ifr_name, dev->if_name, sizeof ifr.ifr_name);
	ifr.ifr_data = descr;

	if (ioctl(dev->ctrl_sock, SIOCGIFDESCR, &ifr) == -1) {
		tuntap_log(TUNTAP_LOG_ERR,
		    "Can't get the interface description");
		return NULL;
	}
	return (char *)descr;
#else /* For now: DragonFlyBSD, Linux and Darwin */
	(void)dev;
	tuntap_log(TUNTAP_LOG_NOTICE,
	    "Your system does not support tuntap_get_descr()");
	return NULL;
#endif
}

char *
tuntap_get_descr(struct device *dev) {
	return tuntap_sys_get_descr(dev);
}

