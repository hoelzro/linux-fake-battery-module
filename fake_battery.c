#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/power_supply.h>

#include <asm/uaccess.h>

static ssize_t
control_device_read(struct file *file, char *buffer, size_t count, loff_t *ppos)
{
    static char *message = "fake battery information!";
    size_t message_len = strlen(message);

    if(count < message_len) {
        return -EINVAL;
    }

    if(*ppos != 0) {
        return 0;
    }

    if(copy_to_user(buffer, message, message_len)) {
        return -EINVAL;
    }

    *ppos = message_len;

    return message_len;
}

static struct file_operations control_device_ops = {
    .owner = THIS_MODULE,
    .read = control_device_read,
};

static struct miscdevice control_device = {
    MISC_DYNAMIC_MINOR,
    "fake_battery",
    &control_device_ops,
};

static int __init
fake_battery_init(void)
{
    int result;

    result = misc_register(&control_device);
    if(result) {
        printk(KERN_ERR "Unable to register misc device!");
        return result;
    }
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
