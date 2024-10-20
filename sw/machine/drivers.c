#include "config.h"
#include "drivers.h"
#include "m.h"

void m_dev_tohost_init(void);
void m_dev_altera_jtag_uart_init(void);

void m_register_all_drivers(void)
{
#ifdef M_DEV_TOHOST
	m_dev_tohost_init();
#endif

#ifdef M_DEV_ALTERA_JTAG_UART
	m_dev_altera_jtag_uart_init();
#endif
}

