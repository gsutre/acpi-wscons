#ifndef _X86_ACPI_DISPLAY_MD_H_
#define _X86_ACPI_DISPLAY_MD_H_

#include <sys/device.h>

int		acpidisp_md_out_register(device_t, int, int, int,
		    int (*)(void *, int *), int (*)(void *, int),
		    int (*)(void *, int));
int		acpidisp_md_out_deregister(device_t);
struct genfb_parameter_callback *
		acpidisp_md_out_find(int, int, int);

#endif /* _X86_ACPI_DISPLAY_MD_H_ */
