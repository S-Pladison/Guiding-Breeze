#ifndef GUIDING_BREEZE_SRC_CORE_SCREEN_H
#define GUIDING_BREEZE_SRC_CORE_SCREEN_H

#include <glm/ext/vector_int2.hpp>
#include <sys/types.h>

#include <vector>

namespace gb::Screen {

/**
 * @brief Данные о разрешении экрана.
 */
struct Resolution {
public:
  size_t width;        //< Ширина экрана в пикселях
  size_t height;       //< Высота экрана в пикселях
  size_t refresh_rate; //< Частота обновления (Гц)
};

/**
 * @brief Режим отображения.
 */
enum class DisplayMode : u_short {
  Windowed,   //< Оконный режим
  Borderless, //< Оконный режим без границ
  Fullscreen  //< Полноэкранный режим
};

/**
 * @brief Получить текущее разрешение экрана.
 * 
 * @return const Resolution& Текущее разрешение.
 */
const Resolution& GetResolution();

/**
 * @brief Изменяет разрешение экрана.
 * 
 * @param requested_resolution Запрашиваемое разрешение экрана.
 * @param mode Режим отображения.
 * @note Если соответствующее разрешение не поддерживается, используется ближайшее.
 */
void SetResolution(const Resolution& requested_resolution, DisplayMode mode);

/**
 * @brief Изменяет разрешение экрана.
 * 
 * @param width Запрашиваемая ширина разрешения экрана.
 * @param height Запрашиваемая высота разрешения экрана.
 * @param refresh_rate Запрашиваемая частота обновления экрана.
 * @param mode Режим отображения.
 * @note Если соответствующее разрешение не поддерживается, используется ближайшее.
 */
inline void SetResolution(size_t width, size_t height, size_t refresh_rate, DisplayMode mode) {
  Resolution resolution;
  resolution.width = width;
  resolution.height = height;
  resolution.refresh_rate = refresh_rate;

  return SetResolution(resolution, mode);
}

/**
 * @brief Получить информацию обо всех доступных разрешениях экрана.
 * 
 * @return const std::vector<Resolution>& Вектор доступных разрешений.
 */
const std::vector<Resolution>& GetAvailableResolutions();

}

#endif // GUIDING_BREEZE_SRC_CORE_SCREEN_H