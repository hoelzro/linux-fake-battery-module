/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Based heavily on https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/drivers/power/test_power.c?id=refs/tags/v4.2.6 */

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

static ssize_t
control_device_write(struct file *file, const char *buffer, size_t count, loff_t *ppos)
{
    char kbuffer[1024]; // limited by kernel frame size, 1K should be enough
    char *buffer_cursor;
    char *newline;
    int iter_count = 0;

    int status;

    if(*ppos != 0) {
        printk(KERN_ERR "writes to /dev/fake_battery must be completed in a single system call\n");
        return -EINVAL;
    }

    if(count > 1024) {
        count = 1024;
    }

    status = copy_from_user(kbuffer, buffer, count);

    if(status != 0) {
        printk(KERN_ERR "bad copy_from_user\n");
        return -EINVAL;
    }

    buffer_cursor = kbuffer;

    while((newline = memchr(buffer_cursor, '\n', count))) {
        *newline = '\0';
        printk(KERN_INFO "got line: %s\n", buffer_cursor);

        count         -= (newline - buffer_cursor) + 1;
        buffer_cursor  = newline + 1;
    }

    return count;
}

static struct file_operations control_device_ops = {
    .owner = THIS_MODULE,
    .read = control_device_read,
    .write = control_device_write,
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
    misc_deregister(&control_device);
    printk(KERN_INFO "unloaded fake_battery module\n");
}

module_init(fake_battery_init);
module_exit(fake_battery_exit);
