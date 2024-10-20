#include <string.h>

#include "drivers.h"
#include "m.h"

static struct m_console_driver *m_console_drivers = NULL;

static void m_default_print_str(struct m_console_driver *self, const char *str, size_t length)
{
	(void) length;
	while (*str)
		self->print_char(self, *str++);
}

void m_print_chr(char c)
{
	for (struct m_console_driver *driver = m_console_drivers; driver; driver = driver->next)
		driver->print_char(driver, c);
}

void m_print_str(const char *str)
{
	size_t length = strlen(str);
	for (struct m_console_driver *driver = m_console_drivers; driver; driver = driver->next)
		driver->print_str(driver, str, length);
}

void m_print_hex_bits(unsigned value, int bits)
{
	static const char HEX_DIGITS[16] = "0123456789abcdef";

	char buffer[2 * sizeof value + 1];

	int index = 2 * sizeof value;
	do {
		index -= 2;

		unsigned lo = value & 0x0f;
		unsigned hi = (value >> 4) & 0x0f;

		buffer[index] = HEX_DIGITS[hi];
		buffer[index + 1] = HEX_DIGITS[lo];

		value >>= 8;
	} while (index > 0);

	buffer[2 * sizeof value] = '\0';

	int nibbles = (bits + 3) / 4;

	int offset = 2 * sizeof value - nibbles;
	if (offset < 0 || offset > 2 * sizeof value)
		offset = 0;

	m_print_str(buffer + offset);
}

void m_console_driver_register(struct m_console_driver *driver)
{
	if (!driver->print_str)
		driver->print_str = m_default_print_str;
	driver->next = m_console_drivers;
	m_console_drivers = driver;
}
