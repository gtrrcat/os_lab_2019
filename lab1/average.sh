#!/bin/bash

count=$#
echo "Количество аргументов: $count"

if [ $count -eq 0 ]; then
    echo "Ошибка: нет входных данных."
    exit 1
fi

# Проверка, что все аргументы - числа
for num in "$@"; do
    if ! [[ "$num" =~ ^-?[0-9]+$ ]]; then
        echo "Ошибка: \"$num\" не является целым числом."
        exit 1
    fi
done

# Вычисление суммы
sum=0
for num in "$@"; do
    sum=$((sum + num))
done

# Вычисление среднего арифметического (целочисленное деление)
average=$((sum / count))
remainder=$((sum % count))

echo "Среднее арифметическое (целая часть): $average"
if [ $remainder -ne 0 ]; then
    echo "Остаток от деления: $remainder/$count"
fi