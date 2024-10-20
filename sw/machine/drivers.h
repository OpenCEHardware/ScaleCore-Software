#ifndef M_DRIVERS_H
#define M_DRIVERS_H

#include <stddef.h>

struct m_exit_driver
{
	struct m_exit_driver *next; // Intrusive linked list

	void (*exit)(struct m_exit_driver *self, unsigned code);
};

struct m_console_driver
{
	struct m_console_driver *next; // Intrusive linked list

	void (*print_char)(struct m_console_driver *self, char c);
	void (*print_str) (struct m_console_driver *self, const char *str, size_t length);
};

void m_exit_driver_register   (struct m_exit_driver *driver);
void m_console_driver_register(struct m_console_driver *driver);

void m_register_all_drivers(void);

#endif
