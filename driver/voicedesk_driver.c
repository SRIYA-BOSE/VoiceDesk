#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "voicedesk"
#define CLASS_NAME  "voicedesk"
#define BUFFER_SIZE 256

static dev_t voicedesk_dev;
static struct cdev voicedesk_cdev;
static struct class *voicedesk_class;
static struct device *voicedesk_device;

static char device_buffer[BUFFER_SIZE];
static size_t buffer_size;
static DEFINE_MUTEX(voicedesk_mutex);

static int voicedesk_open(struct inode *inode, struct file *file)
{
    pr_info("voicedesk: device opened\n");
    return 0;
}

static int voicedesk_release(struct inode *inode, struct file *file)
{
    pr_info("voicedesk: device closed\n");
    return 0;
}

static ssize_t voicedesk_read(
    struct file *file,
    char __user *user_buffer,
    size_t count,
    loff_t *offset)
{
    size_t bytes_to_copy;

    if (*offset >= buffer_size)
        return 0;

    mutex_lock(&voicedesk_mutex);

    bytes_to_copy = min(count, buffer_size - (size_t)*offset);

    if (copy_to_user(user_buffer,
                     device_buffer + *offset,
                     bytes_to_copy)) {
        mutex_unlock(&voicedesk_mutex);
        return -EFAULT;
    }

    *offset += bytes_to_copy;

    mutex_unlock(&voicedesk_mutex);

    pr_info("voicedesk: read %zu bytes\n", bytes_to_copy);

    return bytes_to_copy;
}

static ssize_t voicedesk_write(
    struct file *file,
    const char __user *user_buffer,
    size_t count,
    loff_t *offset)
{
    size_t bytes_to_copy = min(count, (size_t)(BUFFER_SIZE - 1));

    mutex_lock(&voicedesk_mutex);

    memset(device_buffer, 0, BUFFER_SIZE);

    if (copy_from_user(device_buffer,
                       user_buffer,
                       bytes_to_copy)) {
        mutex_unlock(&voicedesk_mutex);
        return -EFAULT;
    }

    device_buffer[bytes_to_copy] = '\0';
    buffer_size = bytes_to_copy;

    mutex_unlock(&voicedesk_mutex);

    pr_info("voicedesk: received command: %s\n", device_buffer);

    return bytes_to_copy;
}

static const struct file_operations voicedesk_fops = {
    .owner = THIS_MODULE,
    .open = voicedesk_open,
    .release = voicedesk_release,
    .read = voicedesk_read,
    .write = voicedesk_write,
};

static int __init voicedesk_init(void)
{
    int ret;

    buffer_size = 0;
    memset(device_buffer, 0, BUFFER_SIZE);

    ret = alloc_chrdev_region(&voicedesk_dev, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("voicedesk: failed to allocate device number\n");
        return ret;
    }

    cdev_init(&voicedesk_cdev, &voicedesk_fops);
    voicedesk_cdev.owner = THIS_MODULE;

    ret = cdev_add(&voicedesk_cdev, voicedesk_dev, 1);
    if (ret < 0) {
        pr_err("voicedesk: failed to add cdev\n");
        unregister_chrdev_region(voicedesk_dev, 1);
        return ret;
    }

    voicedesk_class = class_create(CLASS_NAME);
    if (IS_ERR(voicedesk_class)) {
        ret = PTR_ERR(voicedesk_class);
        cdev_del(&voicedesk_cdev);
        unregister_chrdev_region(voicedesk_dev, 1);
        return ret;
    }

    voicedesk_device = device_create(
        voicedesk_class,
        NULL,
        voicedesk_dev,
        NULL,
        DEVICE_NAME
    );

    if (IS_ERR(voicedesk_device)) {
        ret = PTR_ERR(voicedesk_device);
        class_destroy(voicedesk_class);
        cdev_del(&voicedesk_cdev);
        unregister_chrdev_region(voicedesk_dev, 1);
        return ret;
    }

    pr_info(
        "voicedesk: driver loaded, major=%d minor=%d\n",
        MAJOR(voicedesk_dev),
        MINOR(voicedesk_dev)
    );

    return 0;
}

static void __exit voicedesk_exit(void)
{
    device_destroy(voicedesk_class, voicedesk_dev);
    class_destroy(voicedesk_class);
    cdev_del(&voicedesk_cdev);
    unregister_chrdev_region(voicedesk_dev, 1);

    pr_info("voicedesk: driver unloaded\n");
}

module_init(voicedesk_init);
module_exit(voicedesk_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VoiceDesk Team");
MODULE_DESCRIPTION("VoiceDesk Linux character device driver");
MODULE_VERSION("1.0");
