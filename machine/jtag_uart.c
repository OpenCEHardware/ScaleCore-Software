#include <stddef.h>

#include "config.h"
#include "drivers.h"
#include "m.h"

#ifdef M_DEV_ALTERA_JTAG_UART

#define JTAG_UART_DATA_RVALID    (1 << 15)
#define JTAG_UART_DATA_RAVAIL    0xffff0000
#define JTAG_UART_CONTROL_RE     (1 << 0)
#define JTAG_UART_CONTROL_AC     (1 << 10)
#define JTAG_UART_CONTROL_WSPACE 0xffff0000

struct jtag_uart_regs
{
	volatile unsigned data;
	volatile unsigned control;
};

struct m_device_altera_jtag_uart
{
	volatile struct jtag_uart_regs *regs;

	struct m_console_driver console_driver;
};

static void m_dev_altera_jtag_uart_print_char(struct m_console_driver *self, char c)
{
	struct m_device_altera_jtag_uart *jtag_uart = container_of(self, struct m_device_altera_jtag_uart, console_driver);

	while (!(jtag_uart->regs->control & JTAG_UART_CONTROL_WSPACE));
	jtag_uart->regs->data = (unsigned char) c;
}

static struct m_device_altera_jtag_uart m_dev_altera_jtag_uart =
{
	.regs = (volatile struct jtag_uart_regs *) M_DEV_ALTERA_JTAG_UART_BASE,

	.console_driver = {
		.print_char = m_dev_altera_jtag_uart_print_char,
	},
};

void m_dev_altera_jtag_uart_init(void)
{
	// Enable RX (input) interrupts
	m_dev_altera_jtag_uart.regs->control = JTAG_UART_CONTROL_RE;

	m_console_driver_register(&m_dev_altera_jtag_uart.console_driver);
}

#endif // M_DEV_ALTERA_JTAG_UART
