#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "drivers.h"
#include "m.h"

void m_trap_entry(void);
void __attribute__((noreturn)) m_trap_exit(void);

static struct m_exit_driver *m_exit_drivers = NULL;

void __attribute__((constructor(1000))) m_init(void)
{
	memset(&m_trap_context, 0, sizeof m_trap_context);

	write_csr(mtvec, (uintptr_t)m_trap_entry);
	write_csr(mie, MIP_MEIP);

	m_register_all_drivers();

	M_LOG("early init ok\n");

	unsigned jump_address;

	asm volatile (
		"la   %0, in_user_mode\n"
		"csrw mepc, %0\n"
		"csrw mstatus, zero\n"
		"mret\n"
		"in_user_mode:\n"
		: "=r" (jump_address)
	);
}

void __attribute__((noreturn)) m_die(unsigned code)
{
	M_INFO("cpu halted\n");

	for (struct m_exit_driver *driver = m_exit_drivers; driver; driver = driver->next)
		driver->exit(driver, code);

	while (1)
		asm volatile ("wfi");
}

void m_exit_driver_register(struct m_exit_driver *driver)
{
	driver->next = m_exit_drivers;
	m_exit_drivers = driver;
}

