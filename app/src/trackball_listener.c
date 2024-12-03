#include <zephyr/kernel.h>
#include <device.h>
#include <drivers/sensor.h>
#include <zmk/layer.h>
#include <zmk/event_manager.h>

// 動作時間の閾値（ミリ秒）
#define MOUSE_LAYER_ACTIVATION_TIME_MS 2000  // 2秒
#define AUTO_MOUSE_LAYER 4                  // 有効化するレイヤー番号

static int64_t movement_start_time = 0;     // 動き始めた時間を記録
static bool is_movement_detected = false;

// トラックボールの動作を確認する関数
static bool trackball_is_moving(const struct device *dev) {
    int16_t dx = 0, dy = 0;

    // トラックボールの移動量を取得
    dx = sensor_get_dx(dev);
    dy = sensor_get_dy(dev);

    return (dx != 0 || dy != 0);  // 動きがあるか確認
}

// トラックボールイベントのハンドラ
void trackball_event_handler(const struct device *dev) {
    int64_t current_time = k_uptime_get();

    if (trackball_is_moving(dev)) {
        if (!is_movement_detected) {
            // 動き始めた時間を記録
            movement_start_time = current_time;
            is_movement_detected = true;
        }

        // 動きが2秒以上続いたらレイヤーを有効化
        if ((current_time - movement_start_time) >= MOUSE_LAYER_ACTIVATION_TIME_MS) {
            zmk_layer_activate(AUTO_MOUSE_LAYER);
        }
    } else {
        // 動きが止まった場合
        is_movement_detected = false;
        zmk_layer_deactivate(AUTO_MOUSE_LAYER);  // レイヤーを無効化
    }
}
