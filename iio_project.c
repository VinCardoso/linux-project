#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>      /* For platform devices */
#include <linux/interrupt.h>            /* For IRQ */
#include <linux/of.h>                   /* For DT*/
#include <linux/iio/iio.h>    			/* mandatory */
#include <linux/iio/sysfs.h>  			/* mandatory since sysfs are used */
#include <linux/iio/events.h> 			/* For advanced users, to manage iio events */
#include <linux/iio/buffer.h>  			/* mandatory to use triggered buffers */

#include <linux/delay.h>

#define FAKE_VOLTAGE_CHANNEL(num)   \
	{                               \
	    .type = IIO_VOLTAGE,        \
	    .indexed = 1,               \
	    .channel = (num),           \
	    .address = (num),           \
	    .info_mask_separate = BIT(IIO_CHAN_INFO_RAW),           \
	    .info_mask_shared_by_type = BIT(IIO_CHAN_INFO_SCALE)    \
	}

struct my_private_data {
	struct i2c_client *client;
	struct mutex lock;
	u16 mtreg;
};

static int read_raw(struct iio_dev *indio_dev,
			    struct iio_chan_spec const *channel, int *val,
			    int *val2, long mask)
{

	int ret, tmp;
	char buffer[2];
	struct my_private_data *data = iio_priv(indio_dev);
	// const struct bh1750_chip_info *chip_info = data->chip_info;

	switch (mask) {
	case IIO_CHAN_INFO_RAW:
		switch (channel->type) {
		case IIO_VOLTAGE:
			mutex_lock(&data->lock);
			ret = i2c_master_recv(data->client, buffer, 4);

			printk(KERN_INFO "buff0: %d \n",(int )buffer[0]);
			printk(KERN_INFO "buff1: %d \n",(int )buffer[1]);

			if (channel->channel==0){
				ressult= (buffer[1]<<8) +buffer[0];
				*val = result
			}

			if (channel->channel==1){
				result= (buffer[3]<<8) +buffer[2];
				*val = result;
			}



			// *val = (int )buffer[0];
			// *val2 = 2;

			mutex_unlock(&data->lock);
			if (ret < 0)
				return ret;

			return IIO_VAL_INT;
		default:
			return -EINVAL;
		}
	case IIO_CHAN_INFO_SCALE:
		// tmp = chip_info->mtreg_to_scale / data->mtreg;
		*val = tmp / 1000000;
		*val2 = tmp % 1000000;
		return IIO_VAL_INT_PLUS_MICRO;
	default:
		return -EINVAL;
	}






	return 0;
}

static int write_raw(struct iio_dev *indio_dev,
			    struct iio_chan_spec const *chan,
			    int val, int val2, long mask)
{
	return 0;
}

static const struct iio_chan_spec fake_channels[] = {
	FAKE_VOLTAGE_CHANNEL(0),
	FAKE_VOLTAGE_CHANNEL(1),
};

static const struct iio_info fake_iio_info = {
	.read_raw = read_raw,
	.write_raw		= write_raw,
	.driver_module = THIS_MODULE,
};

static int my_pdrv_probe (struct i2c_client *client,const struct i2c_device_id *id)
{
	struct iio_dev *indio_dev;
	struct my_private_data *data;

	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(*data));
	if (!indio_dev) {
		dev_err(&client->dev, "iio allocation failed!\n");
		return -ENOMEM;
	}

	data = iio_priv(indio_dev);
	data->client = client;

	mutex_init(&data->lock);
	indio_dev->dev.parent = &client->dev;
	indio_dev->info = &fake_iio_info;
	indio_dev->name = KBUILD_MODNAME;
	indio_dev->modes = INDIO_DIRECT_MODE;
	indio_dev->channels = fake_channels;
	indio_dev->num_channels = ARRAY_SIZE(fake_channels);
	// indio_dev->available_scan_masks = 0xF;
	iio_device_register(indio_dev);

	return 0;
};

static int my_pdrv_remove(struct i2c_client *client)
{
	struct iio_dev *indio_dev = i2c_get_clientdata(client);
	iio_device_unregister(indio_dev);
	return 0;
};

static const struct i2c_device_id iio_project_ids[] = {
    {"iio-project", 0 },
    { /* sentinel */ }
};

MODULE_DEVICE_TABLE(i2c, iio_project_ids);

static struct i2c_driver mypdrv = {
	.probe      = my_pdrv_probe,
	.remove     = my_pdrv_remove,
	.id_table = iio_project_ids,
	.driver     = {
		.name     = "iio-project",
		// .owner    = THIS_MODULE,
	},
};

module_i2c_driver(mypdrv);


MODULE_AUTHOR("Vinicius Cardoso");
MODULE_LICENSE("GPL");
