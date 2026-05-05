# Vulkan Geometry Android App

Минимальный Android-проект с NativeActivity и рендерингом сложной геометрической фигуры (икосаэдра) через Vulkan.

## Что внутри
- `app/src/main/AndroidManifest.xml` — запуск `NativeActivity`.
- `app/src/main/cpp/main.cpp` — цикл приложения и Vulkan-рендерер.
- `app/src/main/cpp/CMakeLists.txt` — сборка native-библиотеки.
- `app/build.gradle.kts` и `app/src/main/cpp` — подключение NDK + Vulkan.

## Сборка
1. Установите Android Studio (с NDK и CMake).
2. Откройте папку проекта.
3. Синхронизируйте Gradle и соберите `app`.

> Пример ориентирован на демонстрацию структуры и базового Vulkan-пайплайна.
