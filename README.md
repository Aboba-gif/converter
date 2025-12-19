# Gaussian Blur Converter

Утилита для сглаживания (размытия) изображений с помощью фильтра Гаусса.  
Внутри используется:

- C++17
- OpenCV
- cxxopts
- собственный формат хранения изображения (AoS: `ImageAoS`, `Pixel`)

---

## Установка и сборка

Требуется:

- [Conan 2.x](https://conan.io/)
- CMake 3.16+
- C++ компилятор (g++/clang++/MSVC)

### 1. Установка зависимостей через Conan

Из корня проекта:

```bash
conan install . --output-folder=build --build=missing
```

Это скачает/соберёт OpenCV и cxxopts и сгенерирует файлы для CMake в `build/`.

### 2. Генерация файлов сборки CMake

```bash
cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE=build/build/Release/generators/conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release
```

Обратите внимание: путь к `conan_toolchain.cmake` должен совпадать с реально сгенерированным в вашей среде (после `conan install .` проверьте содержимое `build/` и при необходимости поправьте путь).

### 3. Сборка проекта

```bash
cmake --build build
```

После этого бинарник будет находиться по пути:

```text
build/converter
```

---

## Запуск

Пример запуска утилиты:

```bash
./build/converter -i "test_image/lenna.png" -o "test_image/blur_10.png" -r 10
```

Где:

- `-i, --input` — путь к входному изображению (обязательно).
- `-o, --output` — путь к выходному изображению (опционально).
  - Если не указан, выходной файл будет сформирован автоматически (суффикс `_blurred`).
- `-r, --radius` — радиус фильтра Гаусса (целое число > 0).

Примеры:

```bash
# Размытие lenna.png с радиусом 5
./build/converter -i "test_image/lenna.png" -o "test_image/blur_5.png" -r 5

# Размытие lenna.png с радиусом 10
./build/converter -i "test_image/lenna.png" -o "test_image/blur_10.png" -r 10

# Без явного указания выходного файла — имя будет сгенерировано автоматически
./build/converter -i "test_image/lenna.png" -r 3
# Пример: test_image/lenna_blurred.png
```

Справка:

```bash
./build/converter --help
```

---

## Пример результата (до / после)

В репозитории есть папка `test_image/` с примером входного и выходных изображений:

- `test_image/lenna.png` — исходное изображение (до обработки),
- `test_image/blur_5.png` — результат при радиусе `r = 5`,
- `test_image/blur_10.png` — результат при радиусе `r = 10`.

### Исходное изображение

```markdown
![Original image](test_image/lenna.png)
```

### Результат размытия (r = 5)

```markdown
![Blurred image (r=5)](test_image/blur_5.png)
```

### Результат размытия (r = 10)

```markdown
![Blurred image (r=10)](test_image/blur_10.png)
```

---

## Кратко о внутреннем устройстве

- Изображения читаются и записываются через OpenCV (`cv::imread`, `cv::imwrite`).
- Внутри изображение хранится как `ImageAoS` (Array of Structures):
  - каждый пиксель — структура `Pixel { uint8_t b, g, r; }` (BGR).
- Фильтр Гаусса реализован вручную:
  - вычисление ядра Гаусса `compute_gaussian_kernel(radius)`,
  - применение ядра к каждому каналу `apply_kernel_to_pixel(...)`,
  - проход по всему изображению `apply_gaussian_blur(...)`.
- Время выполнения измеряется и выводится в миллисекундах.
