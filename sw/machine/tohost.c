#include <limits.h>
#include <stdint.h>
#include <string.h>

#include "config.h"
#include "drivers.h"
#include "m.h"

#ifdef M_DEV_TOHOST

struct m_device_tohost
{
	struct m_exit_driver    exit_driver;
	struct m_console_driver console_driver;
};

volatile unsigned tohost;

#define HTIF_CALL_WRITE 64
#define HTIF_FD_STDOUT  1

static unsigned m_tohost_call(unsigned which, unsigned arg0, unsigned arg1, unsigned arg2)
{
	volatile uint64_t buffer[8] __attribute__((aligned(64)));
	buffer[0] = which;
	buffer[1] = arg0;
	buffer[2] = arg1;
	buffer[3] = arg2;

	__sync_synchronize();
	tohost = (uintptr_t)buffer;
	__sync_synchronize();

	return (unsigned)buffer[0];
}

static void m_dev_tohost_print_char(struct m_console_driver *self, char c)
{
	(void) self;
	m_tohost_call(HTIF_CALL_WRITE, HTIF_FD_STDOUT, (unsigned) &c, sizeof c);
}

static void m_dev_tohost_print_str(struct m_console_driver *self, const char *str, size_t length)
{
	(void) self;
	m_tohost_call(HTIF_CALL_WRITE, HTIF_FD_STDOUT, (unsigned) str, length);
}

static void m_dev_tohost_exit(struct m_exit_driver *self, unsigned code)
{
	(void) self;

	__sync_synchronize();
	tohost = (code << 1) | 1;
	__sync_synchronize();
}

static struct m_device_tohost m_dev_tohost =
{
	.exit_driver = {
		.exit = m_dev_tohost_exit,
	},

	.console_driver = {
		.print_char = m_dev_tohost_print_char,
		.print_str  = m_dev_tohost_print_str,
	},
};

void m_dev_tohost_init(void)
{
	m_exit_driver_register(&m_dev_tohost.exit_driver);
	m_console_driver_register(&m_dev_tohost.console_driver);
}

#endif // M_DEV_TOHOST
