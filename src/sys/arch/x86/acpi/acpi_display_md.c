#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: acpi_cpu_md.c,v 1.34 2010/08/25 05:07:43 jruoho Exp $");

#include <sys/param.h>
#include <sys/device.h>
#include <sys/kmem.h>
#include <sys/queue.h>
#include <sys/systm.h>

#include <dev/wsfb/genfbvar.h>

#include <machine/acpi_display_md.h>

struct acpidisp_md_handler {
	device_t			 ah_device;
	struct genfb_parameter_callback	 ah_gpc;
	struct {
		int	 bus;
		int	 device;
		int	 function;
	}				 ah_pci;

	TAILQ_ENTRY(acpidisp_md_handler) ah_list;
};

static TAILQ_HEAD(, acpidisp_md_handler) acpidisp_md_all_handlers =
    TAILQ_HEAD_INITIALIZER(acpidisp_md_all_handlers);

int
acpidisp_md_out_register(device_t dv, int bus, int device, int function,
    int (*get)(void *, int *), int (*set)(void *, int), int (*upd)(void *, int))
{
	struct acpidisp_md_handler *ah;

	ah = kmem_zalloc(sizeof(*ah), KM_SLEEP);
	if (ah == NULL)
		return ENOMEM;

	ah->ah_device = dv;
	ah->ah_gpc.gpc_cookie = dv;
	ah->ah_gpc.gpc_set_parameter = set;
	ah->ah_gpc.gpc_get_parameter = get;
	ah->ah_gpc.gpc_upd_parameter = upd;
	ah->ah_pci.bus = bus;
	ah->ah_pci.device = device;
	ah->ah_pci.function = function;

	TAILQ_INSERT_TAIL(&acpidisp_md_all_handlers, ah, ah_list);

	return 0;
}

int
acpidisp_md_out_deregister(device_t dv)
{
	struct acpidisp_md_handler *ah;

	TAILQ_FOREACH(ah, &acpidisp_md_all_handlers, ah_list) {

		if (ah->ah_device != dv)
			continue;
		TAILQ_REMOVE(&acpidisp_md_all_handlers, ah, ah_list);
		/* XXX Need to tell users of this handler. */
		kmem_free(ah, sizeof(*ah));
		return 0;
	}

	return EINVAL;
}

struct genfb_parameter_callback *
acpidisp_md_out_find(int bus, int device, int function)
{
	struct acpidisp_md_handler *ah;

	TAILQ_FOREACH(ah, &acpidisp_md_all_handlers, ah_list) {

		if (ah->ah_pci.bus == bus &&
		    ah->ah_pci.device == device &&
		    ah->ah_pci.function == function)
			return &ah->ah_gpc;
	}

	return NULL;
}
