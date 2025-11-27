#include <board.h>
#include <drivers/framework.h>
#include <lib/simplefb.h>
#include <lib/debug.h>

/* This is a generic uniLoader port for Xiaomi Blossom devices:
 * Realme c15 c11 c12 
 * 
 *
 *
 * Works with R vendor only */

#define UART_BASE 0x11002000
#define UART_LSR_BASE	0x14
#define UART_LSR_DR 	0x01 	/* Data ready */
#define UART_LSR_THRE 	0x20 	/* TX holding register empty */

void uart_putc(char ch)
{
	while (!(readl((void *)(UART_BASE + UART_LSR_BASE)) & UART_LSR_THRE))
		;

	writel(ch, (void *)UART_BASE);
}

void uart_puts(const char *s)
{
	while (*s != '\0') {
		uart_putc(*s);
		s++;
	}
}

static struct video_info rmx2180_fb = {
	.format = FB_FORMAT_ARGB8888,
	.width = 720,
	.height = 1600,
	.stride = 4,
	.address = (void *)0x7bd10000
};

int rmx2180_drv(void)
{
	REGISTER_DRIVER("simplefb", simplefb_probe, &rmx2180_fb);
	return 0;
}

struct board_data board_ops = {
	.name = "realme-rmx2180",
	.ops = {
		.drivers_init = rmx2180_drv,
	},
	.quirks = 0
};
