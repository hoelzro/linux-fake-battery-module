#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/power_supply.h>

static int __init
fake_battery_init(void)
{
    printk(KERN_INFO "loaded fake_battery module\n");
    return 0;
}

static void __exit
fake_battery_exit(void)
{
    printk(KERN_INFO "unloaded fake_battery module\n");
}

module_init(fake_battery_init);
module_exit(fake_battery_exit);
