# computational_geometry
Лабораторные работы по курсу вычислительной геометрии

## Лабораторная работа №1
### Задача о пересечении ортогональных отрезков

* Сборка:
  * cd ortho_segments
  * cmake -B build
  * cd build && make -j4

* Запуск большого теста:
  * ./ortho_segments -i ../segments_full.txt -o segments_out.txt
  либо для вывода в стандартный поток
  * ./ortho_segments -i ../segments_full.txt

## Лабораторная работа №2
### Задача о минимальной опорной прямой

* Сборка:
  * cd minimal_support_line
  * cmake -B build
  * cd build && make -j4

* Запуск теста:
  * ./minimal_support_line -i ../points.txt
Вывод производится в стандартный поток

