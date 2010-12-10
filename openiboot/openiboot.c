#include "openiboot.h"
#include "platform.h"
#include "tasks.h"
#include "util.h"

mainfn_t OpenIBootMain = NULL;

void OpenIBootConsole()
{
	init_modules();
	bufferPrintf(	"  ___                   _ ____              _   \r\n"
					" / _ \\ _ __   ___ _ __ (_) __ )  ___   ___ | |_ \r\n"
					"| | | | '_ \\ / _ \\ '_ \\| |  _ \\ / _ \\ / _ \\| __|\r\n"
					"| |_| | |_) |  __/ | | | | |_) | (_) | (_) | |_ \r\n"
					" \\___/| .__/ \\___|_| |_|_|____/ \\___/ \\___/ \\__|\r\n"
					"      |_|                                       \r\n"
					"\r\n"
					"version: %s\r\n", OPENIBOOT_VERSION_STR);
	DebugPrintf("                    DEBUG MODE\r\n");
}

#ifdef CONFIG_S5L8720
//TODO: REMOVEME HACKHACKHACK (boot from an installed openiboot on ipt2g if volup is pressed)

#include "lcd.h"
#include "images.h"
#include "actions.h"
#include "framebuffer.h"
#include "timer.h"
#include "buttons.h"
#include "gpio.h"
#include "nor.h"

void load_iboot() {
	framebuffer_clear();
	bufferPrintf("Loading iOS...");
	Image* image = images_get(fourcc("ibox"));
	if(image == NULL)
		image = images_get(fourcc("ibot"));
	void* imageData;
	images_read(image, &imageData);
	chainload((uint32_t)imageData);
}
#endif

void OpenIBootStart()
{
	platform_init();

	OpenIBootMain = &OpenIBootConsole;
	init_boot_modules();

	OpenIBootMain();

#ifdef CONFIG_S5L8720
	//TODO: REMOVEME HACKHACKHACK (boot from an installed openiboot on ipt2g if volup is pressed)	
	while(1) {
		task_yield();
		if (!gpio_pin_state(BUTTONS_VOLUP)) {
			load_iboot();
		}
	}
#endif

	tasks_run(); // Runs forever.
}

void OpenIBootShutdown()
{
	exit_modules();
	platform_shutdown();
}
