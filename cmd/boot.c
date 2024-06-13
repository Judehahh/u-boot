// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2000-2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 */

/*
 * Misc boot support
 */
#include <common.h>
#include <command.h>
#include <net.h>
#include <asm/io.h>

#ifdef CONFIG_CMD_GO

/* Allow ports to override the default behavior */
__attribute__((weak))
unsigned long do_go_exec(ulong (*entry)(int, char * const []), int argc,
				 char *const argv[])
{
	return entry (argc, argv);
}

static int do_go(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
	ulong	addr, rc;
	int     rcode = 0;

	if (argc < 2)
		return CMD_RET_USAGE;

	addr = hextoul(argv[1], NULL);

	printf ("## Starting application at 0x%08lX ...\n", addr);

	/*
	 * pass address parameter as argv[0] (aka command name),
	 * and all remaining args
	 */
	rc = do_go_exec ((void *)addr, argc - 1, argv + 1);
	if (rc != 0) rcode = 1;

	printf ("## Application terminated, rc = 0x%lX\n", rc);
	return rcode;
}

/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	go, CONFIG_SYS_MAXARGS, 1,	do_go,
	"start application at address 'addr'",
	"addr [arg ...]\n    - start application at address 'addr'\n"
	"      passing 'arg' as arguments"
);

#endif

static int boot_small(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
	ulong	addr;

	if (argc < 2)
		return CMD_RET_USAGE;

	addr = hextoul(argv[1], NULL);

	printf("## Booting small core at 0x%08lX ...\n", addr);

	writel(addr, (void*)0x91102100ULL); //cpu0_hart_rstvec 设置小核的解复位向量，复位后程序执行位置

	writel(0x10001000, (void*)0x91101004ULL); //清 done bit
	writel(0x10001, (void*)0x91101004ULL); //设置 reset bit
	writel(0x10000, (void *)0x91101004ULL); //清 reset bit
	return 0;
}

U_BOOT_CMD(
	bootsmall, CONFIG_SYS_MAXARGS, 1, boot_small,
	"start application from small core at address 'addr'",
	"addr [arg ...]\n    - start application from small core at address 'addr'\n"
	"      passing 'arg' as arguments"
);

U_BOOT_CMD(
	reset, 2, 0,	do_reset,
	"Perform RESET of the CPU",
	"- cold boot without level specifier\n"
	"reset -w - warm reset if implemented"
);

#ifdef CONFIG_CMD_POWEROFF
U_BOOT_CMD(
	poweroff, 1, 0,	do_poweroff,
	"Perform POWEROFF of the device",
	""
);
#endif
