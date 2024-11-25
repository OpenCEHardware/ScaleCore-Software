#include <stdint.h>
#include <string.h>

#include "m.h"

#define ADP_Stopped_ApplicationExit 0x20026

#define SHFB_MAGIC_0 0x53
#define SHFB_MAGIC_1 0x48
#define SHFB_MAGIC_2 0x46
#define SHFB_MAGIC_3 0x42

// Extension bits for feature byte 0
#define SH_EXT_EXIT_EXTENDED (1 << 0)
#define SH_EXT_STDOUT_STDERR (1 << 1)

static const uint8_t m_semihosting_feature_bytes[] =
{
	SHFB_MAGIC_0,
	SHFB_MAGIC_1,
	SHFB_MAGIC_2,
	SHFB_MAGIC_3,
	SH_EXT_EXIT_EXTENDED,
};

static int m_semihosting_features_flen(struct m_file_descriptor *desc)
{
	(void) desc;
	return sizeof m_semihosting_feature_bytes;
}

static int m_semihosting_features_read(struct m_file_descriptor *desc, void *buf, size_t length)
{
	(void) desc;

	size_t eof = sizeof m_semihosting_feature_bytes - desc->offset;
	if (length > eof)
		length = eof;

	memcpy(buf, &m_semihosting_feature_bytes[desc->offset], length);

	desc->offset += length;
	return (int) length;
}

static const struct m_file_ops m_semihosting_features_ops =
{
	.flen = m_semihosting_features_flen,
	.read = m_semihosting_features_read,
};

struct m_file m_semihosting_features_file =
{
	.ops = &m_semihosting_features_ops,
};

static inline void m_read_semihosting_args_1(unsigned *arg1)
{
	*arg1 = *(unsigned *)*arg1;
}

static inline void m_read_semihosting_args_2(unsigned *arg1, unsigned *arg2)
{
	unsigned *base = (unsigned *)*arg1;
	*arg1 = base[0];
	*arg2 = base[1];
}

static inline void m_read_semihosting_args_3(unsigned *arg1, unsigned *arg2, unsigned *arg3)
{
	unsigned *base = (unsigned *)*arg1;
	*arg1 = base[0];
	*arg2 = base[1];
	*arg3 = base[2];
}

void m_handle_semihosting_syscall(void)
{
	unsigned call = m_trap_context.a0;
	unsigned arg1 = m_trap_context.a1;
	unsigned arg2;
	unsigned arg3;

	int ret = 0;

	if (m_trap_from_machine()) {
		M_LOG("M-to-M call, did you attempt to call libc (e.g. printf) from M-mode?\n");
		m_bad_trap();
	}

	switch (call) {
		case SEMIHOSTING_SYS_CLOSE:
			m_read_semihosting_args_1(&arg1);
			ret = m_sys_close((int) arg1);
			break;

		case SEMIHOSTING_SYS_ERRNO:
			ret = m_current_process->semihost_errno;
			break;

		case SEMIHOSTING_SYS_EXIT:
			m_die(arg1 == ADP_Stopped_ApplicationExit ? 0 : 1);
			break;

		case SEMIHOSTING_SYS_EXIT_EXTENDED:
			m_read_semihosting_args_2(&arg1, &arg2);
			m_die(arg1 == ADP_Stopped_ApplicationExit ? (int) arg2 : 1);
			break;

		case SEMIHOSTING_SYS_FLEN:
			m_read_semihosting_args_1(&arg1);
			ret = m_sys_flen((int) arg1);
			break;

		case SEMIHOSTING_SYS_ISTTY:
			m_read_semihosting_args_1(&arg1);
			ret = m_sys_istty((int) arg1);
			break;

		case SEMIHOSTING_SYS_OPEN:
			m_read_semihosting_args_3(&arg1, &arg2, &arg3);
			ret = m_sys_open((const char *) arg1, (int) arg2);
			break;

		case SEMIHOSTING_SYS_READ:
			m_read_semihosting_args_3(&arg1, &arg2, &arg3);
			ret = m_sys_read((int) arg1, (void *) arg2, (size_t) arg3);

			// "On exit, the RETURN REGISTER contains the number of bytes not filled in the buffer (buffer_length - bytes_read)"
			if (ret >= 0)
				ret = (int) ((size_t) arg3 - (size_t) ret);

			break;

		case SEMIHOSTING_SYS_SEEK:
			m_read_semihosting_args_2(&arg1, &arg2);
			ret = m_sys_seek((int) arg1, (off_t) arg2);
			break;

		case SEMIHOSTING_SYS_WRITEC:
			m_read_semihosting_args_1(&arg1);
			m_print_chr((char) arg1);
			break;

		case SEMIHOSTING_SYS_WRITE0:
			m_print_str((const char *) arg1);
			break;

		default:
			M_LOG("unknown call ");
			m_print_hex(call);
			m_print_str("\n");

			m_bad_trap();
			break;
	}

	if (ret < 0) {
		m_current_process->semihost_errno = -ret;
		m_trap_context.a0 = (unsigned) -1;
	} else {
		m_current_process->semihost_errno = 0;
		m_trap_context.a0 = (unsigned) ret;
	}

	m_trap_context.pc += 8;
}
