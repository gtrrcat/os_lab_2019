#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

pid_t *child_pids = NULL;
int pnum = -1;

void alarmFunc(int a) {
    if (child_pids != NULL) {
        for (int i = 0; i < pnum; i++) {
            if (child_pids[i] > 0) {
                kill(child_pids[i], SIGKILL);
            }
        }
    }
}

int main(int argc, char **argv) {
    int seed = -1;
    int array_size = -1;
    bool with_files = false;
    int timeout = -1;

    while (true) {
        static struct option options[] = {
            {"seed", required_argument, 0, 0},
            {"array_size", required_argument, 0, 0},
            {"pnum", required_argument, 0, 0},
            {"timeout", required_argument, 0, 0},
            {0, 0, 0, 0}
        };

        int option_index = 0;
        int c = getopt_long(argc, argv, "f", options, &option_index);

        if (c == -1) break;

        switch (c) {
            case 0:
                switch (option_index) {
                    case 0:
                        seed = atoi(optarg);
                        if (seed <= 0) {
                            printf("seed is a positive number\n");
                            return 1;
                        }
                        break;
                    case 1:
                        array_size = atoi(optarg);
                        if (array_size <= 0) {
                            printf("array_size is a positive number\n");
                            return 1;
                        }
                        break;
                    case 2:
                        pnum = atoi(optarg);
                        if (pnum <= 0) {
                            printf("pnum is a positive number\n");
                            return 1;
                        }
                        break;
                    case 3:
                        timeout = atoi(optarg);
                        if (timeout < 0) {
                            printf("timeout must be a non-negative number\n");
                            return 1;
                        }
                        break;
                    default:
                        printf("Index %d is out of options\n", option_index);
                }
                break;
            case '?':
                break;
            default:
                printf("getopt returned character code 0%o?\n", c);
        }
    }

    if (optind < argc) {
        printf("Has at least one no option argument\n");
        return 1;
    }

    if (seed == -1 || array_size == -1 || pnum == -1) {
        printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" [--timeout \"num\"]\n",
               argv[0]);
        return 1;
    }

    // Выделяем память для массива
    int *array = malloc(sizeof(int) * array_size);
    if (array == NULL) {
        printf("Memory allocation failed for array\n");
        return 1;
    }
    
    GenerateArray(array, array_size, seed);
    int active_child_processes = 0;

    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    int fd[2];
    if (!with_files) {
        if (pipe(fd) == -1) {
            printf("Pipe creation failed\n");
            free(array);
            return 1;
        }
    }
    
    // Выделяем память для child_pids после проверки pnum
    child_pids = malloc(sizeof(pid_t) * pnum);
    if (child_pids == NULL) {
        printf("Memory allocation failed for child_pids\n");
        free(array);
        return 1;
    }
    
    // Инициализируем массив child_pids
    for (int i = 0; i < pnum; i++) {
        child_pids[i] = -1;
    }

    // Устанавливаем обработчик сигнала только после инициализации child_pids
    signal(SIGALRM, alarmFunc);

    for (int i = 0; i < pnum; i++) {
        pid_t child_pid = fork();
        if (child_pid >= 0) {
            active_child_processes += 1;
            child_pids[i] = child_pid;
            if (child_pid == 0) {
                // Дочерний процесс
                struct MinMax min_max;

                // Вычисляем границы
                int begin = i * (array_size / pnum);
                int end = (i + 1) * (array_size / pnum);
                if (i == pnum - 1) {
                    end = array_size;
                }

                // Проверяем корректность границ
                if (begin >= array_size || end > array_size || begin < 0) {
                    printf("Invalid bounds: begin=%d, end=%d, array_size=%d\n", 
                           begin, end, array_size);
                    exit(1);
                }

                min_max = GetMinMax(array, begin, end);

                if (with_files) {
                    // Увеличиваем размер буфера для имени файла
                    char filename[25];
                    snprintf(filename, sizeof(filename), "result_%d.txt", i);
                    FILE *file = fopen(filename, "w");
                    if (file == NULL) {
                        printf("Failed to open file %s\n", filename);
                        exit(1);
                    }
                    fprintf(file, "%d %d\n", min_max.min, min_max.max);
                    fclose(file);
                } else {
                    write(fd[1], &min_max.min, sizeof(min_max.min));
                    write(fd[1], &min_max.max, sizeof(min_max.max));
                }
                exit(0);
            }
        } else {
            printf("Fork failed!\n");
            // Убиваем все созданные дочерние процессы при ошибке
            for (int j = 0; j < i; j++) {
                if (child_pids[j] > 0) {
                    kill(child_pids[j], SIGTERM);
                }
            }
            free(array);
            free(child_pids);
            return 1;
        }
    }

    // Закрываем записывающий конец pipe в родительском процессе
    if (!with_files) {
        close(fd[1]);
    }

    if (timeout > 0) {
        alarm(timeout);
    }

    while (active_child_processes > 0) {
        wait(NULL);
        active_child_processes -= 1;
    }

    // Отменяем alarm, если он еще не сработал
    if (timeout > 0) {
        alarm(0);
    }

    struct MinMax min_max;
    min_max.min = INT_MAX;
    min_max.max = INT_MIN;

    for (int i = 0; i < pnum; i++) {
        int min = INT_MAX;
        int max = INT_MIN;

        if (with_files) {
            // Увеличиваем размер буфера для имени файла
            char filename[25];
            snprintf(filename, sizeof(filename), "result_%d.txt", i);
            FILE *file = fopen(filename, "r");
            if (file != NULL) {
                fscanf(file, "%d %d", &min, &max);
                fclose(file);
            }
        } else {
            read(fd[0], &min, sizeof(min));
            read(fd[0], &max, sizeof(max));
        }
        
        if (min < min_max.min) min_max.min = min;
        if (max > min_max.max) min_max.max = max;
    }

    if (!with_files) {
        close(fd[0]);
    }

    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);

    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

    free(array);
    free(child_pids);

    printf("Min: %d\n", min_max.min);
    printf("Max: %d\n", min_max.max);
    printf("Elapsed time: %fms\n", elapsed_time);
    fflush(NULL);
    return 0;
}