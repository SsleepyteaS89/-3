#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

// ==================== СТРУКТУРЫ ДАННЫХ ====================

typedef struct Node {
    char data[100];        // Строка символов
    struct Node* next;
    struct Node* prev;     // Для циклической очереди
} Node;

typedef struct {
    Node* front;    // Начало очереди
    Node* rear;     // Конец очереди
    int size;       // Текущий размер очереди
} CircularQueue;

#define SIZE_MENU 9

// ==================== ПРОТОТИПЫ ФУНКЦИЙ ====================

void Menu(const char** list_menu, int point);
void enqueue(CircularQueue* queue);          // Добавить элемент в очередь
void dequeue(CircularQueue* queue);          // Удалить элемент из очереди
void is_empty(CircularQueue* queue);         // Проверка пустоты очереди
void count_elements(CircularQueue* queue);   // Подсчет элементов
void find_element(CircularQueue* queue);     // Поиск символа в очереди
void delete_queue(CircularQueue* queue);     // Удаление всей очереди
void display_queue(CircularQueue* queue);    // Отображение содержимого очереди
void create_queue(CircularQueue* queue);     // Создание/инициализация очереди
void about_program();                       // Информация о программе
void ClearInputBuffer();

// ==================== МЕНЮ ====================

void Menu(const char** list_menu, int point) {
    printf("============================================================\n");
    printf("           ЛАБОРАТОРНАЯ РАБОТА №4 - ЦИКЛИЧЕСКАЯ ОЧЕРЕДЬ     \n");
    printf("                    (Вариант 10)                            \n");
    printf("============================================================\n");
    for (int i = 0; i < SIZE_MENU; i++) {
        if (point == i) {printf("  → %-50s \n", list_menu[i]);} 
        else {printf("    %-50s \n", list_menu[i]);}
    }
    printf("============================================================\n");
    printf("  Управление: ↑↓ - выбор, Enter - выполнить, Esc - выход    \n");
    printf("============================================================\n");
}

// ==================== ГЛАВНАЯ ФУНКЦИЯ ====================

int main() {
    short is_exit = 0;
    int point = 0

    // Подключение русского языка
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    const char** list_menu = malloc(SIZE_MENU * sizeof(char*)); // Выделение памяти для меню
    list_menu[0] = "Создать циклическую очередь";
    list_menu[1] = "Добавить элемент в очередь (Enqueue)";
    list_menu[2] = "Удалить элемент из очереди (Dequeue)";
    list_menu[3] = "Полное очищение очереди";
    list_menu[4] = "Проверить, пуста ли очередь";
    list_menu[5] = "Подсчитать элементы в очереди";
    list_menu[6] = "Найти символ в очереди";
    list_menu[7] = "Отобразить содержимое очереди";
    list_menu[8] = "Выход";
    
    CircularQueue queue = {NULL, NULL, 0}; // Инициализация циклической очереди
    
    while (!is_exit) {
        system("cls");
        Menu(list_menu, point);
        int key = _getch();
        
        switch(key) {
            case 72:    // стрелка вверх
                if (point == 0) point = SIZE_MENU - 1;
                else point--;
                break;
            case 80:    // стрелка вниз
                if (point == SIZE_MENU - 1) point = 0;
                else point++;
                break;
            case 13:    // Enter - выбор пункта
                system("cls");
                switch(point) {
                    case 0: create_queue(&queue); break;
                    case 1: enqueue(&queue); break;
                    case 2: dequeue(&queue); break;
                    case 3: delete_queue(&queue); break;
                    case 4: is_empty(&queue); break;
                    case 5: count_elements(&queue); break;
                    case 6: find_element(&queue); break;
                    case 7: display_queue(&queue); break;
                    case 8: is_exit = 1; break;
                }
                if (point != 8) {
                    printf("\n\nНажмите любую клавишу для возврата в меню...");
                    _getch();
                }
                break;
            case 27:    // Escape - выход
                is_exit = 1; 
                break;
        }
    }
    
    delete_queue(&queue); // Очистка памяти очереди перед выходом
    free(list_menu); // Освобождение памяти меню
    
    system("cls");
    about_program();
    printf("============================================================\n");
    printf("          ПРОГРАММА ЗАВЕРШЕНА                               \n");
    printf("============================================================\n");
    printf("  Очередь удалена, память освобождена.                      \n");
    printf("  Спасибо, что воспользовались программой!                  \n");
    printf("============================================================\n");   
    return 0;
}

// ==================== ФУНКЦИИ ОЧЕРЕДИ ====================

// Создание/инициализация очереди
void create_queue(CircularQueue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    printf("Циклическая очередь создана (инициализирована).\n");
}

// Добавление элемента в очередь (в конец)
void enqueue(CircularQueue* queue) {
    printf("================================================\n");
    printf("        ДОБАВЛЕНИЕ ЭЛЕМЕНТА В ОЧЕРЕДЬ           \n");
    printf("================================================\n\n");
    
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Ошибка выделения памяти!\n");
        return;
    }
    
    printf("Введите строку символов (макс. 99 символов): ");
    fgets(new_node->data, sizeof(new_node->data), stdin);
    
    // Удаление символа новой строки
    size_t len = strlen(new_node->data);
    if (len > 0 && new_node->data[len - 1] == '\n') {
        new_node->data[len - 1] = '\0';
    }
    
    // Проверка на пустую строку
    if (strlen(new_node->data) == 0) {
        strcpy(new_node->data, "(пустая строка)");
    }
    
    // Настройка связей для нового узла
    if (queue->front == NULL) { // Если очередь пуста
        new_node->next = new_node;
        new_node->prev = new_node;
        queue->front = new_node;
        queue->rear = new_node;
    } else { // Если в очереди уже есть элементы
        new_node->next = queue->front;
        new_node->prev = queue->rear;
        queue->rear->next = new_node;
        queue->front->prev = new_node;
        queue->rear = new_node;
    }
    
    queue->size++;
    printf("\nСтрока '%s' успешно добавлена в очередь (в конец).\n", new_node->data);
    printf("Текущий размер очереди: %d\n", queue->size);
}

// Удаление элемента из очереди (из начала)!!!
void dequeue(CircularQueue* queue) {
    printf("================================================\n");
    printf("        УДАЛЕНИЕ ЭЛЕМЕНТА ИЗ ОЧЕРЕДИ            \n");
    printf("================================================\n\n");
    
    if (queue->front == NULL) {
        printf("Очередь пуста! Нечего удалять.\n");
    } else {
        printf("Удаляемый элемент (первый в очереди): '%s'\n", queue->front->data);
        
        if (queue->front == queue->rear) { // Если в очереди только один элемент
            free(queue->front);
            queue->front = NULL;
            queue->rear = NULL;
        } else { // Если в очереди несколько элементов
            Node* temp = queue->front;
            queue->front = queue->front->next;
            queue->front->prev = queue->rear;
            queue->rear->next = queue->front;
            free(temp);
        }
        
        queue->size--;
        printf("Элемент успешно удален из очереди.\n");
        printf("Текущий размер очереди: %d\n", queue->size);
        
        if (queue->front != NULL) {
            printf("Теперь первый элемент: '%s'\n", queue->front->data);
        }

}

// Проверка пустоты очереди
void is_empty(CircularQueue* queue) {
    printf("================================================\n");
    printf("              ПУСТА ЛИ ОЧЕРЕДЬ?                 \n");
    printf("================================================\n\n");
    
    if (queue->front == NULL) {
        printf("Да, очередь ПУСТА.\n");
    } else {
        printf("Нет, очередь НЕ ПУСТА.\n");
        printf("Первый элемент: '%s'\n", queue->front->data);
        printf("Размер очереди: %d элементов\n", queue->size);
    }
}

// Подсчет элементов в очереди
void count_elements(CircularQueue* queue) {
    printf("================================================\n");
    printf("         СТАТИСТИКА ОЧЕРЕДИ                     \n");
    printf("================================================\n\n");
    
    printf("Количество элементов в очереди: %d\n", queue->size);
    
    // Подсчет общего количества символов
    if (queue->front != NULL) {
        Node* current = queue->front;
        int total_chars = 0;
        int node_count = 0;
        
        do {
            total_chars += strlen(current->data);
            node_count++;
            current = current->next;
        } while (current != queue->front && node_count < queue->size);
        
        printf("Общее количество символов: %d\n", total_chars);
        printf("Средняя длина строки: %.2f символов\n", 
               queue->size > 0 ? (float)total_chars / queue->size : 0);
    } else {
        printf("Общее количество символов: 0\n");
    }
}

// Поиск символа в очереди!!!!
void find_element(CircularQueue* queue) {
    printf("================================================\n");
    printf("           ПОИСК СИМВОЛА В ОЧЕРЕДИ              \n");
    printf("================================================\n\n");
    
    if (queue->front == NULL) {
        printf("Очередь пуста. Поиск невозможен.\n");
        return;
    }
    
    char target;
    printf("Введите символ для поиска: ");
    target = getchar();
    int c = getchar();
    if (c != '\n') {
        if (c != EOF) ClearInputBuffer();
    }
    
    Node* current = queue->front;
    int position = 1;
    int found_anywhere = 0;
    
    printf("\nРезультаты поиска символа '%c':\n", target);
    printf("========================================\n");
     do {
         char* str = current->data;
        int found_in_element = 0;
        
       for (int i = 0; str[i] != '\0'; i++) {
            if (str[i] == target) {
                if (!found_in_element) {
                    printf("Элемент %d ('%s'): позиции ", position, str);
                    found_in_element = 1;
                    found_anywhere = 1;
                }
                printf("%d", i + 1);
                
                // Проверяем следующий символ
                if (str[i + 1] != '\0' && str[i + 1] == target) {
                    printf(", ");
                } else {
                    // Ищем следующее вхождение в этой же строке
                    int next_pos = i + 1;
                    while (str[next_pos] != '\0') {
                        if (str[next_pos] == target) {
                            printf(", ");
                            break;
                        }
                        next_pos++;
                    }
                }
            }
        }
        
        if (found_in_element) printf("\n");
        position++;
        current = current->next;
        
    } while (current != queue->front && position <= queue->size);
    
    if (!found_anywhere) {
        printf("Символ '%c' не найден ни в одном элементе очереди.\n", target);
    }
}

// Удаление всей очереди!!!!
void delete_queue(CircularQueue* queue) {
    printf("================================================\n");
    printf("         ПОЛНОЕ УДАЛЕНИЕ ОЧЕРЕДИ                \n");
    printf("================================================\n\n");
    
    if (queue->front == NULL) {
        printf("Очередь уже пуста.\n");
    } else {
        int count = 0;
        Node* current = queue->front;
        
        // Разрываем циклические связи для корректного удаления
        if (queue->front != queue->rear) {
            queue->rear->next = NULL;
            queue->front->prev = NULL;
        }
        
        // Удаляем все узлы
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
            count++;
        }
        
        queue->front = NULL;
        queue->rear = NULL;
        queue->size = 0;
        
        printf("Удалено %d элементов.\n", count);
        printf("Очередь полностью очищена.\n");
    }
}

// Отображение содержимого очереди (FIFO - от первого к последнему)
void display_queue(CircularQueue* queue) {
    printf("================================================\n");
    printf("      СОДЕРЖИМОЕ ЦИКЛИЧЕСКОЙ ОЧЕРЕДИ (FIFO)     \n");
    printf("================================================\n\n");
    
    if (queue->front == NULL) {
        printf("Очередь пуста.\n");
    } else {
        printf("Порядок элементов (от первого к последнему):\n");
        printf("=================================================\n");
        
        Node* current = queue->front;
        int element_num = 1;
        
        do {
            printf("Элемент %d: '%s' (длина: %zu символов)\n", 
                   element_num, current->data, strlen(current->data));
            current = current->next;
            element_num++;
        } while (current != queue->front && element_num <= queue->size);
        
        printf("\nСтатистика:\n");
        printf("  • Всего элементов: %d\n", queue->size);
        printf("  • Первый элемент: '%s'\n", queue->front->data);
        printf("  • Последний элемент: '%s'\n", queue->rear->data);
        
        // Демонстрация циклической природы
        printf("\nЦиклическая структура:\n");
        printf("  • front->prev указывает на: '%s'\n", queue->front->prev->data);
        printf("  • rear->next указывает на: '%s'\n", queue->rear->next->data);
    }
}

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

void about_program() {
    printf("============================================================\n");
    printf("             ИНФОРМАЦИЯ О ПРОГРАММЕ                         \n");
    printf("============================================================\n");
    printf("  Название: Лабораторная работа №10 - Циклическая очередь   \n");
    printf("  Вариант: 10                                               \n");
    printf("  Разработчик: Хохлова Анастасия Павловна                   \n");
    printf("  Структура данных: Циклическая очередь (FIFO)              \n");
    printf("  Тип данных: Строки символов                               \n");
    printf("                                                            \n");
    printf("  Реализованные операции:                                   \n");
    printf("    1. Создание/инициализация очереди                       \n");
    printf("    2. Добавление элемента (enqueue)                        \n");
    printf("    3. Удаление элемента (dequeue)                          \n");
    printf("    4. Проверка пустоты очереди                             \n");
    printf("    5. Подсчет элементов                                    \n");
    printf("    6. Поиск символа в очереди                              \n");
    printf("    7. Отображение содержимого очереди                      \n");
    printf("                                                            \n");
    printf("  Особенности циклической очереди:                          \n");
    printf("    • Двусвязная циклическая структура                      \n");
    printf("    • FIFO (First-In-First-Out) принцип                     \n");
    printf("    • Последний элемент связан с первым                     \n");
    printf("    • Эффективные операции добавления/удаления              \n");
    printf("============================================================\n");
}

void ClearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
