#include <linux/kernel.h>
#include <linux/module.h>

static int __init hello_init(void)
{
    printk("Hello, LSY!\n");
    return 0;
}

static void __exit hello_exit(void)
{
    printk("Good-bye,LSY\n");
    return 0;
}

module_init(hello_init);
module_exit(hello_exit);



MODULE_LICENSE("GPL");

