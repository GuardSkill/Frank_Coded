#include <linux/kernel.h>
#include <linux/module.h>
#include <mach/gpio.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <plat/gpio-cfg.h>
#include <linux/irqreturn.h>
#include <asm-generic/gpio.h>
#include <linux/interrupt.h>
#include <linux/delay.h>


typedef struct key {
	char name[10];
	unsigned int gpio;
	int value;
	int irq;
	
} KEY ;


static KEY g_keybuf[] = {
		{"key1", EXYNOS4_GPX3(2), 1, 0},
		{"key2", EXYNOS4_GPX3(3), 1, 0},
		{"key3", EXYNOS4_GPX3(4), 1, 0},
		{"key4", EXYNOS4_GPX3(5), 1, 0},
};

static int ispress = 0;

static irqreturn_t key_handler(int irq, void * arg)
{
	ispress =1;

	return IRQ_HANDLED;
}

static ssize_t lsy_read(struct file * pfile, char __user * buff, size_t count, loff_t * offset)
{
	int i = 0;
	int ret = 0;
	char buf[2] = {0};

	while(ispress == 0)
	{
		msleep(10);
	}
	ispress = 0;
	
	for(i = 0; i < 4; i++)
	{
		g_keybuf[i].value = gpio_get_value(g_keybuf[i].gpio);
		if(g_keybuf[i].value == 0)
		{
			buf[0] = (char)(i+1);
			ret = copy_to_user(buff, buf, 4);
		}
	}
	
	return  min(strlen(buf), count);
}


static struct file_operations g_fops = {
		.owner 		=	THIS_MODULE,
		.read 		= 	lsy_read,
};


static struct miscdevice g_miscdev = {
	.minor 		= 	MISC_DYNAMIC_MINOR,
	.name 		= 	"lsy_button",
	.fops 		= 	&g_fops,

};


static int __init hello_init(void)
{
	int i = 0;
	misc_register(&g_miscdev);

	for(i = 0; i < 4; i++)
	{	
		gpio_request(g_keybuf[i].gpio, g_keybuf[i].name);
		s3c_gpio_cfgpin(g_keybuf[i].gpio, S3C_GPIO_INPUT);
		g_keybuf[i].irq = gpio_to_irq(g_keybuf[i].gpio);
		
		request_irq(g_keybuf[i].irq, key_handler,
				IRQF_SHARED | IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
				g_keybuf[i].name, &g_keybuf[i]);
	}

	return 0;
}

static void __exit hello_exit(void)
{
	int i =0;
	misc_deregister(&g_miscdev);

	for(i = 0; i < 4; i++)
	{
		free_irq(g_keybuf[i].irq, &g_keybuf[i]);
		gpio_free(g_keybuf[i].gpio);
	}
	
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");


