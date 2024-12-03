#include <zephyr/kernel.h>
#include <zmk/event_manager.h>
#include <zmk/events/sensor_event.h>
#include <zmk/mouse.h>
#include <math.h>

// 移動速度調整用関数
static void adjust_mouse_speed(int16_t *x, int16_t *y) {
    int16_t movement_size = abs(*x) + abs(*y);
    float speed_multiplier = 1.0;

    if (movement_size > 60) {
        speed_multiplier = 3.0;
    } else if (movement_size > 30) {
        speed_multiplier = 1.5;
    } else if (movement_size > 5) {
        speed_multiplier = 1.0;
    } else if (movement_size > 4) {
        speed_multiplier = 0.9;
    } else if (movement_size > 3) {
        speed_multiplier = 0.7;
    } else if (movement_size > 2) {
        speed_multiplier = 0.5;
    } else if (movement_size > 1) {
        speed_multiplier = 0.2;
    }

    *x = (int16_t)(*x * speed_multiplier);
    *y = (int16_t)(*y * speed_multiplier);
}

// センサーイベントハンドラ
static int sensor_event_handler(const struct zmk_event_header *eh) {
    const struct sensor_event *event = cast_sensor_event(eh);

    int16_t x = event->data.x;
    int16_t y = event->data.y;

    // マウススピード調整関数を適用
    adjust_mouse_speed(&x, &y);

    zmk_mouse_move(x, y);
    return 0;
}

// イベントリスナーに登録
ZMK_LISTENER(sensor_speed_adjust, sensor_event_handler);
ZMK_SUBSCRIPTION(sensor_speed_adjust, sensor_event);
