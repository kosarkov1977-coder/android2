#include <android/log.h>
#include <android/native_activity.h>
#include <android_native_app_glue.h>
#include <vulkan/vulkan.h>

#include <array>
#include <cmath>
#include <vector>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "VulkanGeometry", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "VulkanGeometry", __VA_ARGS__)

struct Vertex {
    float x, y, z;
};

static std::vector<Vertex> createIcosahedron(float r) {
    const float t = (1.0f + std::sqrt(5.0f)) * 0.5f;
    std::vector<Vertex> v = {
        {-1,  t,  0}, {1,  t,  0}, {-1, -t,  0}, {1, -t,  0},
        {0, -1,  t}, {0,  1,  t}, {0, -1, -t}, {0,  1, -t},
        {t,  0, -1}, {t,  0,  1}, {-t,  0, -1}, {-t,  0,  1}
    };
    for (auto& p : v) {
        float len = std::sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
        p.x = p.x / len * r;
        p.y = p.y / len * r;
        p.z = p.z / len * r;
    }
    return v;
}

class VulkanRenderer {
public:
    bool init(ANativeWindow* window) {
        (void)window;
        VkApplicationInfo appInfo{VK_STRUCTURE_TYPE_APPLICATION_INFO};
        appInfo.pApplicationName = "VulkanGeometry";
        appInfo.apiVersion = VK_API_VERSION_1_1;

        VkInstanceCreateInfo ci{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
        ci.pApplicationInfo = &appInfo;

        if (vkCreateInstance(&ci, nullptr, &instance_) != VK_SUCCESS) {
            LOGE("vkCreateInstance failed");
            return false;
        }

        vertices_ = createIcosahedron(0.75f);
        LOGI("Vulkan initialized, vertices=%zu", vertices_.size());
        return true;
    }

    void drawFrame(float seconds) {
        // Здесь должен быть полный pipeline + swapchain.
        // Для демо — логируем «вращение» фигуры во времени.
        const float angle = std::fmod(seconds * 45.0f, 360.0f);
        LOGI("Drawing icosahedron at angle=%.2f", angle);
    }

    void shutdown() {
        if (instance_ != VK_NULL_HANDLE) {
            vkDestroyInstance(instance_, nullptr);
            instance_ = VK_NULL_HANDLE;
        }
    }

private:
    VkInstance instance_ = VK_NULL_HANDLE;
    std::vector<Vertex> vertices_;
};

static void handleCmd(android_app* app, int32_t cmd) {
    auto* renderer = reinterpret_cast<VulkanRenderer*>(app->userData);
    if (cmd == APP_CMD_INIT_WINDOW && app->window) {
        renderer->init(app->window);
    }
}

void android_main(android_app* app) {
    app_dummy();

    VulkanRenderer renderer;
    app->userData = &renderer;
    app->onAppCmd = handleCmd;

    bool running = true;
    float t = 0.0f;

    while (running) {
        int events;
        android_poll_source* source;
        while (ALooper_pollAll(16, nullptr, &events, reinterpret_cast<void**>(&source)) >= 0) {
            if (source) source->process(app, source);
            if (app->destroyRequested) {
                running = false;
                break;
            }
        }

        if (app->window) {
            t += 0.016f;
            renderer.drawFrame(t);
        }
    }

    renderer.shutdown();
}
