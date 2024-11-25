#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>

#include "m.h"

static struct m_file *m_lookup_path(const char *path)
{
	// This is a special file required by the semihosting spec
	// "Feature bits are reported using a sequence of bytes, which are accessed by
	// using the SYS_OPEN (0x01) call with the special path name :semihosting-features."
	if (!strcmp(path, ":semihosting-features"))
		return &m_semihosting_features_file;

	//TODO: implement a proper filesystem
	return NULL;
}

static struct m_file_descriptor *m_resolve_fd(int fd)
{
	struct m_file_descriptor *desc = NULL;
	if (fd >= 0 && fd < M_OPEN_MAX) {
		desc = &m_current_process->open_files[fd];
		if (!desc->file)
			desc = NULL;
	}

	return desc;
}

int m_sys_open(const char *path, int mode)
{
	int fd;
	struct m_file *file;
	struct m_file_descriptor *desc;

	(void) mode; //TODO: check open for read vs open for write

	file = m_lookup_path(path);
	if (!file)
		return -ENOENT;

	for (fd = 0; fd < M_OPEN_MAX; ++fd) {
		desc = &m_current_process->open_files[fd];
		if (!desc->file) {
			desc->file = file;
			desc->offset = 0;

			return fd;
		}
	}

	return -EMFILE;
}

int m_sys_close(int fd)
{
	struct m_file_descriptor *desc = m_resolve_fd(fd);
	if (!desc)
		return -EBADF;

	desc->file = NULL;
	return 0;
}

int m_sys_flen(int fd)
{
	struct m_file_descriptor *desc = m_resolve_fd(fd);
	if (!desc)
		return -EBADF;
	else if (!desc->file->ops->flen)
		return -ENOSYS;

	return desc->file->ops->flen(desc);
}

int m_sys_seek(int fd, off_t offset)
{
	int ret;
	struct m_file_descriptor *desc = m_resolve_fd(fd);

	if (!desc)
		return -EBADF;
	else if (desc->file->ops->seek)
		return desc->file->ops->seek(desc, offset);
	else if (!desc->file->ops->flen)
		return -EINVAL;

	ret = desc->file->ops->flen(desc);
	if (ret < 0)
		return ret;
	else if (offset > ret)
		return -EINVAL;

	desc->offset = offset;
	return 0;
}

int m_sys_istty(int fd)
{
	struct m_file_descriptor *desc = m_resolve_fd(fd);
	if (!desc)
		return -EBADF;
	else if (!desc->file->ops->istty)
		return 0;

	return desc->file->ops->istty(desc);
}

int m_sys_read(int fd, void *buf, size_t length)
{
	struct m_file_descriptor *desc = m_resolve_fd(fd);
	if (!desc)
		return -EBADF;
	else if (!desc->file->ops->read)
		return -ENOSYS;

	return desc->file->ops->read(desc, buf, length);
}
