#include <linux/acpi.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include "msi.h"


char *supported_firmwares[SUPPORTED_FIRMWARES_NUM] = {
    "E17F4IMS.108"
};


struct platform_device ec_device = {
    .name = "ec",
    .id_auto = true,
    .dev.release = release_ec,
    .dev.removable = DEVICE_FIXED
};


struct device_attribute ec_attrs[ATTR_NUM] = {
    {
        .attr = {
            .name = "cpu_fan_speed_config",
            .mode = ATTR_RW
        },
        .show = show_cpu_fan_speed_config,
        .store = store_cpu_fan_speed_config
    },
    {
        .attr = {
            .name = "gpu_fan_speed_config",
            .mode = ATTR_RW
        },
        .show = show_gpu_fan_speed_config,
        .store = store_gpu_fan_speed_config
    },
    {  
        .attr = {
            .name = "cpu_fan_speed",
            .mode = ATTR_RO
        },
        .show = show_cpu_fan_speed
    },
    {
        .attr = {
            .name = "gpu_fan_speed",
            .mode = ATTR_RO
        },
        .show = show_gpu_fan_speed
    },
    {
        .attr = {
            .name = "cpu_fan_rpm",
            .mode = ATTR_RO
        },
        .show = show_cpu_fan_rpm
    },
    {
        .attr = {
            .name = "gpu_fan_rpm",
            .mode = ATTR_RO
        },
        .show = show_gpu_fan_rpm
    },
    {
        .attr = {
            .name = "cpu_temp",
            .mode = ATTR_RO
        },
        .show = show_cpu_temp
    },
    {
        .attr = {
            .name = "gpu_temp",
            .mode = ATTR_RO
        },
        .show = show_gpu_temp
    },
    {
        .attr = {
            .name = "performance_mode",
            .mode = ATTR_RW
        },
        .show = show_performance_mode,
        .store = store_performance_mode
    },
    {
        .attr = {
            .name = "fan_mode",
            .mode = ATTR_RW
        },
        .show = show_fan_mode,
        .store = store_fan_mode
    },
    {
        .attr = {
            .name = "battery_threshold",
            .mode = ATTR_RW
        },
        .show = show_battery_threshold,
        .store = store_battery_threshold
    },
    {
        .attr = {
            .name = "cooler_boost",
            .mode = ATTR_RW
        },
        .show = show_cooler_boost,
        .store = store_cooler_boost
    },
    {
        .attr = {
            .name = "backlight_led",
            .mode = ATTR_RO
        },
        .show = show_backlight_led
    }
};


void release_ec(struct device *dev){
    printk("EC: release invoked");
}


ssize_t show_cpu_fan_speed_config(struct device *dev, struct device_attribute *attr, char *buf){
    u8 fan_config_output[FAN_SPEED_CONFIG_NUM];
    char result[32];

    for (int j = 0; j < FAN_SPEED_CONFIG_NUM; j++)
        ec_read(CPU_FAN_SPEED_CONFIG_ADDR + j*0x01, &fan_config_output[j]);
    
    snprintf(result, sizeof(result), "%d %d %d %d %d %d\n",
     fan_config_output[0],
      fan_config_output[1],
       fan_config_output[2],
        fan_config_output[3],
         fan_config_output[4],
          fan_config_output[5]
    );

    return sysfs_emit(buf, result);
}


ssize_t store_cpu_fan_speed_config(struct device *dev, struct device_attribute *attr, const char *buf, size_t count){
    int input[8];

    sscanf(buf, " %d %d %d %d %d %d ",
     &input[0],
      &input[1],
       &input[2],
        &input[3],
         &input[4],
          &input[5]
    );

    for (int j = 0; j < FAN_SPEED_CONFIG_NUM; j++){
        if (input[j] < 0 || input[j] > 100){
            printk("EC: invalid cpu_fan_speed value");
            return count;
        }
    }

    for (int j = 0; j < FAN_SPEED_CONFIG_NUM; j++){
        ec_write(CPU_FAN_SPEED_CONFIG_ADDR + j*0x01, input[j]);
    }

    return count;
}


ssize_t show_gpu_fan_speed_config(struct device *dev, struct device_attribute *attr, char *buf){
    u8 fan_config_output[FAN_SPEED_CONFIG_NUM];
    char result[32];

    for (int j = 0; j < FAN_SPEED_CONFIG_NUM; j++)
        ec_read(GPU_FAN_SPEED_CONFIG_ADDR + j*0x01, &fan_config_output[j]);

    snprintf(result, sizeof(result), "%d %d %d %d %d %d\n",
     fan_config_output[0],
      fan_config_output[1],
       fan_config_output[2],
        fan_config_output[3],
         fan_config_output[4],
          fan_config_output[5]
    );
    
    return sysfs_emit(buf, result);  
}


ssize_t store_gpu_fan_speed_config(struct device *dev, struct device_attribute *attr, const char *buf, size_t count){
    int input[8];

    sscanf(buf, " %d %d %d %d %d %d ",
     &input[0],
      &input[1],
       &input[2],
        &input[3],
         &input[4],
          &input[5]
    );

    for (int j = 0; j < FAN_SPEED_CONFIG_NUM; j++){
        if (input[j] < 0 || input[j] > 100){
            printk("EC: invalid gpu_fan_speed value");
            return count;
        }
    }

    for (int j = 0; j < FAN_SPEED_CONFIG_NUM; j++){
        ec_write(GPU_FAN_SPEED_CONFIG_ADDR + j*0x01, input[j]);
    }

    return count;
}


ssize_t show_cpu_fan_speed(struct device *dev, struct device_attribute *attr, char *buf){
    u8 output;
    ec_read(CPU_FAN_SPEED_ADDR, &output);
    return sysfs_emit(buf, "%d\n", (int)output);
}


ssize_t show_gpu_fan_speed(struct device *dev, struct device_attribute *attr, char *buf){
    u8 output;
    ec_read(GPU_FAN_SPEED_ADDR, &output);
    return sysfs_emit(buf, "%d\n", (int)output);
}


ssize_t show_cpu_fan_rpm(struct device *dev, struct device_attribute *attr, char *buf){
    u8 fan_rpm_output[FAN_RPM_BLOCK_NUM];

    for (int j = 0; j < FAN_RPM_BLOCK_NUM; j++)
        ec_read(CPU_FAN_RPM_ADDR + j*0x01, &fan_rpm_output[j]);

    int value = fan_rpm_output[0]*256 + fan_rpm_output[1];

    if (value != 0)
        return sysfs_emit(buf, "%d\n", (int)(FAN_RPM_BASE/value));

    return sysfs_emit(buf, "0\n");
}


ssize_t show_gpu_fan_rpm(struct device *dev, struct device_attribute *attr, char *buf){
    u8 fan_rpm_output[FAN_RPM_BLOCK_NUM];

    for (int j = 0; j < FAN_RPM_BLOCK_NUM; j++)
        ec_read(GPU_FAN_RPM_ADDR + j*0x01, &fan_rpm_output[j]);

    int value = fan_rpm_output[0]*256 + fan_rpm_output[1];

    if (value != 0)
        return sysfs_emit(buf, "%d\n", (int)(FAN_RPM_BASE/value));

    return sysfs_emit(buf, "0\n");
}


ssize_t show_cpu_temp(struct device *dev, struct device_attribute *attr, char *buf){
    u8 output;
    ec_read(CPU_TEMP_ADDR, &output);
    return sysfs_emit(buf, "%d\n", (int)output);
}


ssize_t show_gpu_temp(struct device *dev, struct device_attribute *attr, char *buf){
    u8 output;
    ec_read(GPU_TEMP_ADDR, &output);
    return sysfs_emit(buf, "%d\n", (int)output);
}


ssize_t show_performance_mode(struct device *dev, struct device_attribute *attr, char *buf){
    u8 output;

    ec_read(PERFORMANCE_MODE_ADDR, &output);

    switch (output){
        case LOW_PERFORMANCE:
            return sysfs_emit(buf, "low\n");

        case MEDIUM_PERFORMANCE:
            return sysfs_emit(buf, "medium\n");

        case HIGH_PERFORMANCE:
            return sysfs_emit(buf, "high\n");

        case TURBO_PERFORMANCE:
            return sysfs_emit(buf, "turbo\n");

        case AUTO_PERFORMANCE:
            return sysfs_emit(buf, "auto\n");

        default:
            return sysfs_emit(buf, "error\n");
    } 
}


ssize_t store_performance_mode(struct device *dev, struct device_attribute *attr, const char *buf, size_t count){
    char input[8];

    sscanf(buf, " %7s ", input);

    if (!strcmp(input, "low")){
        ec_write(PERFORMANCE_MODE_ADDR, LOW_PERFORMANCE);

    }else if (!strcmp(input, "medium")){
        ec_write(PERFORMANCE_MODE_ADDR, MEDIUM_PERFORMANCE);

    }else if (!strcmp(input, "high")){
        ec_write(PERFORMANCE_MODE_ADDR, HIGH_PERFORMANCE);

    }else if (!strcmp(input, "turbo")){
        ec_write(PERFORMANCE_MODE_ADDR, TURBO_PERFORMANCE);

    }else if (!strcmp(input, "auto")){
        ec_write(PERFORMANCE_MODE_ADDR, AUTO_PERFORMANCE);

    }else{
        printk("EC: invalid performance_mode value");
    }

    return count;
}


ssize_t show_fan_mode(struct device *dev, struct device_attribute *attr, char *buf){
    u8 output;

    ec_read(FAN_MODE_ADDR, &output);

    switch (output){
        case AUTO_FAN:
            return sysfs_emit(buf, "auto\n");

        case ADVANCED_FAN:
            return sysfs_emit(buf, "advanced\n");

        default:
            return sysfs_emit(buf, "error\n");
    }  
}


ssize_t store_fan_mode(struct device *dev, struct device_attribute *attr, const char *buf, size_t count){
    char input[16];

    sscanf(buf, " %15s", input);

    if (!strcmp(input, "auto")){
        ec_write(FAN_MODE_ADDR, AUTO_FAN);

    }else if (!strcmp(input, "advanced")){
        ec_write(FAN_MODE_ADDR, ADVANCED_FAN);
    
    }else{
        printk("EC: invalid fan_mode value");
    }

    return count;
}


ssize_t show_battery_threshold(struct device *dev, struct device_attribute *attr, char *buf){
    u8 output;
    ec_read(BATTERY_THRESHOLD_ADDR, &output);
    return sysfs_emit(buf, "%d\n", (int)(output - BATTERY_THRESHOLD_OFFSET));
}


ssize_t store_battery_threshold(struct device *dev, struct device_attribute *attr, const char *buf, size_t count){
    int input;

    sscanf(buf, " %d ", &input);

    if (input >= 50 && input <= 100 && (input%10) == 0){
        ec_write(BATTERY_THRESHOLD_ADDR, input + BATTERY_THRESHOLD_OFFSET);
        return count;
    }
    
    printk("EC: invalid battery_threshold value");
    return count;
}


ssize_t show_cooler_boost(struct device *dev, struct device_attribute *attr, char *buf){
    u8 output;

    ec_read(COOLER_BOOST_ADDR, &output);

    switch (output){
        case CB_OFF:
            return sysfs_emit(buf, "off\n");

        case CB_ON:
            return sysfs_emit(buf, "on\n");

        default:
            return sysfs_emit(buf, "error\n");
    }  
}


ssize_t store_cooler_boost(struct device *dev, struct device_attribute *attr, const char *buf, size_t count){
    char input[8];

    sscanf(buf, " %7s ", input);

    if (!strcmp(input, "off")){
        ec_write(COOLER_BOOST_ADDR, CB_OFF);

    }else if (!strcmp(input, "on")){
        ec_write(COOLER_BOOST_ADDR, CB_ON);

    }else{
        printk("EC: invalid cooler_boost value");
    }

    return count;
}


ssize_t show_backlight_led(struct device *dev, struct device_attribute *attr, char *buf){
    u8 output;

    ec_read(BACKLIGHT_LED_LEVEL_ADDR, &output);

    switch (output){
        case BACKLIGHT_OFF:
            return sysfs_emit(buf, "off\n");

        case BACKLIGHT_LOW:
            return sysfs_emit(buf, "low\n");

        case BACKLIGHT_MEDIUM:
            return sysfs_emit(buf, "medium\n");

        case BACKLIGHT_HIGH:
            return sysfs_emit(buf, "high\n");

        default:
            return sysfs_emit(buf, "error\n");
    }  
}


static int __init init_mod(void){
    for(int i = 0; i < SUPPORTED_FIRMWARES_NUM; i++){
        if(!strcmp(dmi_get_system_info(DMI_BIOS_VERSION), supported_firmwares[i])){
            platform_device_register(&ec_device);

            for (int j = 0; j < ATTR_NUM; j++)
                device_create_file(&ec_device.dev, &ec_attrs[j]);

            printk("EC: module registered");
            return 0;
        }
    }
    
    printk("EC: failed to load module. This device is not supported");
    return -EINVAL;
}


static void __exit exit_mod(void){
    for (int j = 0; j < ATTR_NUM; j++)
        device_remove_file(&ec_device.dev, &ec_attrs[j]);
    
    platform_device_unregister(&ec_device);

    printk("EC: module unregistered");
}


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ilia Alizadeh <iliaalizadee@gmail.com>");
MODULE_DESCRIPTION("MSI embedded controller module");
MODULE_VERSION("1.00");

module_init(init_mod);
module_exit(exit_mod);
