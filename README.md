#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>   // для _getch()
#include <windows.h>

// ---------------- ФУНКЦИЯ ----------------
// f(x) = 2x^3 - 2x^2 - 3x + 16
double f(double x) {
    return 2*x*x*x - 2*x*x - 3*x + 16;
}

// ---------------- БЕЗОПАСНЫЙ ВВОД ----------------
double safeInput(const char *msg) {
    double val;
    int status;

    while (1) {
        printf("%s", msg);
        status = scanf("%lf", &val);

        if (status == 1) {
            while (getchar() != '\n'); // очистка буфера
            return val;
        }

        // Если ввели буквы или символы
        printf("Ошибка: нужно ввести ЦИФРЫ!\n");
        while (getchar() != '\n'); // очистка мусора
    }
}

// ---------------- ПРОВЕРКА ВСЕГО ИНТЕРВАЛА ----------------
// Возвращает 1 — если f(x) > 0 на всём интервале
// Возвращает 0 — если есть отрицательные участки
int checkPositiveOnInterval(double a, double b) {
    double step = (b - a) / 1000.0; // мелкий шаг для проверки

    for (double x = a; x <= b; x += step) {
        if (f(x) <= 0)
            return 0;
    }
    return 1;
}

// ---------------- ЛЕВЫЕ ПРЯМОУГОЛЬНИКИ ----------------
double leftRect(double a, double b, double h) {
    double sum = 0.0;

    for (double x = a; x < b; x += h) {
        if (f(x) > 0)     // считаем только положительную часть
            sum += f(x);
    }

    return sum * h;
}

// ---------------- ПОГРЕШНОСТЬ ----------------

double Calcul_Accuracy(double a, double b) {
  double zero_fun = -1.87;  // Изменено: добавил минус
  if (a <= zero_fun && b >= zero_fun){
    return (pow(b,4))/2.0 + (2.0/3.0)*pow(b,3) + (3.0/2.0)*pow(b,2) + 16*(b)
      - (pow(zero_fun,4))/2.0 - (2.0/3.0)*pow(zero_fun,3) - (3.0/2.0)*pow(zero_fun,2) - 16*(zero_fun);}
  else if (a <= zero_fun && b <= zero_fun) return 0;
  else if (a >= zero_fun && b >= zero_fun){
    return (pow(b,4))/2.0 + (2.0/3.0)*pow(b,3) + (3.0/2.0)*pow(b,2) + 16*(b) -
      (pow(a,4))/2.0 - (2.0/3.0)*pow(a,3) - (3.0/2.0)*pow(a,2) - 16*(a);}
  return 0;
}

double DetermineAccuracy(double a, double b,double h) {
    double pr=leftRect(a,b,h);
    double H_L=Calcul_Accuracy(a,b);
    double accuracy;
    printf("Наше значение %lf\n",pr);
    printf("Абсолютно значение: %lf\n",H_L);

    if (pr<1e-15){printf("Погрешность установлена: 0");accuracy=0.0;}
    else{
    accuracy=(fabs((pr)-H_L)/H_L)*100;
    printf("Погрешность ≈ : %lf %\n", accuracy);
    }
    return accuracy;
}

// ---------------- ВВОД ПРЕДЕЛОВ ----------------
void inputLimits(double *a, double *b, double *h) {
    printf("Допустимые значения [-10^9, 10^9]");

    *a = safeInput("Введите нижний предел a: ");
    while (*a>1e10 || *a<-1e10){printf("Смотрите на границы внимательно\n");
        *a = safeInput("Введите нижний предел a: ");}

    *b = safeInput("Введите верхний предел b: ");
    while (*b>1e10 || *b<-1e10){printf("Смотрите на границы внимательно\n"); 
        *b = safeInput("Введите верхний предел b: ");}

    if (*b <= *a) {
        printf("Ошибка: b должно быть больше a! Мы это сейчас исправим.\n");
        double tt;
        tt=*a;
        *a=*b;
        *b=tt;
    }

    *h = safeInput("Введите шаг h (>0.00001): ");
    while (*h <= 0.00001) {
        printf("Ошибка: шаг должен быть > 0.00001!\n");
        *h = safeInput("Введите шаг h (>0.00001): ");
    }
}

// ---------------- ОТРИСОВКА МЕНЮ ----------------
void showMenu(int selected, double a, double b, double h) {
    system("cls");

    printf("====================================\n");
    printf("  ВЫЧИСЛЕНИЕ ПЛОЩАДИ ФИГУРЫ\n");
    printf("====================================\n");
    printf("Функция: 2x^3 - 2x^2 - 3x + 16\n");
    printf("Текущие параметры:\na=%.3f, b=%.3f, h=%.5f\n", a, b, h);
    printf("====================================\n");

    const char *items[] = {
        "Ввести пределы интегрирования",
        "Вычислить площадь",
        "Оценить погрешность",
        "Выход"
    };

    for (int i = 0; i < 4; i++) {
        if (i == selected)
            printf(" > %s <\n", items[i]);
        else
            printf("   %s\n", items[i]);
    }

    printf("====================================\n");
    printf("↑ ↓ - выбор, Enter - выполнить\n");
}

// ---------------- MAIN ----------------
int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    double a = 0.0, b = 1.0, h = 0.1;
    int selected = 0;

    while (1) {
        showMenu(selected, a, b, h);

        int key = _getch();

            if (key == 72) { // вверх
                selected--;
                if (selected < 0) selected = 3;
            }
            else if (key == 80) { // вниз
                selected++;
                if (selected > 3) selected = 0;
            }
        else {
        if (key == 13) { // Enter
            system("cls");

            switch (selected) {
                case 0:
                    inputLimits(&a, &b, &h);
                    break;

                case 1: {
                    if (!checkPositiveOnInterval(a, b)) {
                        printf("ВНИМАНИЕ: На интервале [%g, %g] функция уходит ниже оси OX!\n", a, b);
                        a=-1.87;
                        printf("Нижняя граница будет смещена до f(x)=0: %g\n", a);
                        printf("Площадь будет вычислена только на положительных участках.\n\n");
                    }
                    
                    double area = leftRect(a, b, h);
                    printf("Площадь = %.8f\n", area);
                    break;
                }
                
                case 2: {
                    if (!checkPositiveOnInterval(a, b)) {
                        printf("Предупреждение: функция отрицательна на части интервала.\n");
                    }

                    double err = DetermineAccuracy(a,b,h);
                    break;
                }

                case 3:
                    printf("Выход...\n");
                    return 0;
            }

            printf("\nНажмите любую клавишу...");
            _getch();
        printf("%d", selected);
    }}

}}
