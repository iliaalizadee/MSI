#include <linux/types.h>
#include <linux/device.h>


#define SUPPORTED_FIRMWARES_NUM 1


#define ATTR_NUM 13
#define FAN_SPEED_CONFIG_NUM 6
#define FAN_RPM_BLOCK_NUM 2


#define BATTERY_THRESHOLD_OFFSET 0x80


#define FAN_RPM_BASE 0x74B30


#define ATTR_RW 0644
#define ATTR_RO 0444


#define CPU_FAN_SPEED_CONFIG_ADDR 0x72
#define GPU_FAN_SPEED_CONFIG_ADDR 0x8A
#define CPU_FAN_SPEED_ADDR 0x71
#define GPU_FAN_SPEED_ADDR 0x89
#define CPU_FAN_RPM_ADDR 0xCC
#define GPU_FAN_RPM_ADDR 0xCA
#define CPU_TEMP_ADDR 0x68
#define GPU_TEMP_ADDR 0x80 
#define PERFORMANCE_MODE_ADDR 0xF2
#define FAN_MODE_ADDR 0xF4
#define BATTERY_THRESHOLD_ADDR 0xEF
#define COOLER_BOOST_ADDR 0x98
#define BACKLIGHT_LED_LEVEL_ADDR 0xF3


#define LOW_PERFORMANCE 0xC2
#define MEDIUM_PERFORMANCE 0xC1
#define HIGH_PERFORMANCE 0xC0
#define TURBO_PERFORMANCE 0xC4
#define AUTO_PERFORMANCE 0x80


#define AUTO_FAN 0x0D
#define ADVANCED_FAN 0x8D


#define CB_OFF 0x02
#define CB_ON 0x82


#define BACKLIGHT_OFF 0x80
#define BACKLIGHT_LOW 0x81
#define BACKLIGHT_MEDIUM 0x82
#define BACKLIGHT_HIGH 0x83


ssize_t show_cpu_fan_speed_config(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t show_gpu_fan_speed_config(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t show_cpu_fan_speed(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t show_gpu_fan_speed(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t show_cpu_fan_rpm(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t show_gpu_fan_rpm(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t show_cpu_temp(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t show_gpu_temp(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t show_performance_mode(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t show_fan_mode(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t show_battery_threshold(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t show_cooler_boost(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t show_backlight_led(struct device *dev, struct device_attribute *attr, char *buf);


ssize_t store_cpu_fan_speed_config(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t store_gpu_fan_speed_config(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t store_performance_mode(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t store_fan_mode(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t store_battery_threshold(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t store_cooler_boost(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);


void release_ec(struct device *dev);
