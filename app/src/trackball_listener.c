#include <zephyr/kernel.h>
#include <device.h>
#include <drivers/sensor.h>
#include <zmk/event_manager.h>
#include <zmk/layer.h>

// トラックボールの移動距離や時間を設定
#define AUTO_MOUSE_DISTANCE_THRESHOLD 10000
#define AUTO_MOUSE_TIME_THRESHOLD_MS 2000

static int total_movement = 0;
static int64_t last_movement_time = 0;

void trackball_event_handler(const struct device *dev) {
    int16_t dx = sensor_get_dx(dev);
    int16_t dy = sensor_get_dy(dev);

    total_movement += abs(dx) + abs(dy);

    if (total_movement >= AUTO_MOUSE_DISTANCE_THRESHOLD) {
        int64_t now = k_uptime_get();
        if ((now - last_movement_time) > AUTO_MOUSE_TIME_THRESHOLD_MS) {
            zmk_layer_activate(4);  // Auto Mouse Layerを有効化
            last_movement_time = now;
        }
        total_movement = 0;
    }
}
