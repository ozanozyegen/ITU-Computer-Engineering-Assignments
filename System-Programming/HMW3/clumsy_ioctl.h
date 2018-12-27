#ifndef __CLUMSY_H
#define __CLUMSY_H

#include <linux/ioctl.h> /* needed for the _IOW etc stuff used later */

#define CLUMSY_IOC_MAGIC  'y'
#define CLUMSY_IOCSMODE _IO(CLUMSY_IOC_MAGIC,   0)
#define CLUMSY_IOCSNUM _IO(CLUMSY_IOC_MAGIC,   1)
#define CLUMSY_IOC_MAXNR 1

#endif
