#!/bin/bash


count=$#
echo "Количество аргументов: $count"


if [ $count -eq 0 ]; then
    echo "Ошибка: нет входных данных."
    exit 1
fi


sum=0
# Проверка и обработка каждого аргумента
#for num in "$@"; do
    # Проверка, является ли аргумент числом (целым или дробным)
    #if ! [[ "$num" =~ ^-?[0-9]+(\.[0-9]+)?$ ]]; then
        #echo "Ошибка: \"$num\" не является числом."
       # exit 1
   # fi

    # Если число — удаляем ведущие нули, добавляем к сумме
   # num=$(echo "$num" | sed 's/^0*//')
   # [ -z "$num" ] && num=0
   # sum=$(echo "$sum + $num" | bc)
#done

for num in "$@"; do
    sum=$((sum + num))
done


average=$(echo "scale=2; $sum / $count" | bc)
echo "Среднее арифметическое: $average"
