#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <libbmp280.h>

#define I2C_BUS "/dev/i2c-1"
#define BMP280_DEV_PATH "/sys/bus/i2c/devices/1-0076/iio:device0/"
#define BMP280_ADDR 0x76


float read_temperature() {
	FILE *f_temp_raw = fopen(BMP280_DEV_PATH "in_temp_raw", "r");
	FILE *f_temp_scale = fopen(BMP280_DEV_PATH "in_temp_scale", "r");
	if (!f_temp_raw || !f_temp_scale) {
		if (f_temp_raw) fclose(f_temp_raw);
		if (f_temp_scale) fclose(f_temp_scale);
		perror("Failed to open sysfs files");
		return 1;
	}

	int temp_raw;
	float temp_scale;

	if (fscanf(f_temp_raw, "%d", &temp_raw) != 1 || fscanf(f_temp_scale, "%f", &temp_scale) != 1) {
		fclose(f_temp_raw);
		fclose(f_temp_scale);
		perror("Failed to read values");
		return 1;
	}

	fclose(f_temp_raw);
	fclose(f_temp_scale);

	return temp_raw * temp_scale / 1000;
}

float read_pressure() {
	FILE *f_pressure_raw = fopen(BMP280_DEV_PATH "in_pressure_raw", "r");
	FILE *f_pressure_scale = fopen(BMP280_DEV_PATH "in_pressure_scale", "r");

	int pressure_raw;
	float pressure_scale;

	if (fscanf(f_pressure_raw, "%d", &pressure_raw) != 1 || fscanf(f_pressure_scale, "%f", &pressure_scale) != 1) {
		fclose(f_pressure_raw);
		fclose(f_pressure_scale);
		perror("Failed to read values");
		return 1;
	}

	fclose(f_pressure_raw);
	fclose(f_pressure_scale);

	return pressure_raw * pressure_scale * 10;
}
