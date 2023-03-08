// ARM_vs_x86.cpp : Ten plik zawiera funkcj� �main�. W nim rozpoczyna si� i ko�czy wykonywanie programu.
//

#include <stdio.h>
#include <chrono>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <string>

// Deklaracja funkcji
void calculate_pi();

void quicksort(int array[], int p, int r);
int partition(int array[], int p, int r);
int* open_file(const int size);

void automated_tests();

// Deklaracja funkcji timera
void start_timer();
void stop_timer();
long measured_time();

// Deklaracja zmiennych globalnych 
int quicksort_tab_size;

std::fstream inputDataFile;

std::chrono::high_resolution_clock::time_point start_time;
std::chrono::high_resolution_clock::time_point end_time;

int main()
{
    srand(time(NULL));

    int choice = 99;

    printf("Wybierz algorytm ktorego czas realizacji ma zostac zmierzony:\n");
    printf("    1. Obliczanie liczby pi\n");
    printf("    2. Sortowanie szybkie\n");
    printf("    3. Test automatyczny\n");
    printf("    0. Wyjscie\n");

    printf("Twoj wybor: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        start_timer();
        calculate_pi();
        stop_timer();

        printf("\nCzas wykonania operacji: %ld ns\n", measured_time());

        break;

    case 2:
    {
        printf("\nIle liczb chcesz posortować: ");
        scanf("%d", &quicksort_tab_size);

        int* array = open_file(quicksort_tab_size);

        printf("Tablica nieposortowana:\n");
        for (int i = 0; i < quicksort_tab_size; ++i)
        {
            printf("%d: %d\n", i, array[i]);
        }

        start_timer();
        quicksort(array, 0, quicksort_tab_size - 1);
        stop_timer();

        printf("\nTablica posortowana:\n");
        for (int i = 0; i < quicksort_tab_size; ++i)
        {
            printf("%d: %d\n", i, array[i]);
        }

        free(array);
        printf("\nCzas wykonania operacji: %ld ns\n", measured_time());

        break;
    }
    case 3:
        automated_tests();

        break;

    case 0:
        return 0;

    default:
        break;
    }
}

void calculate_pi()
{
    int r[2800 + 1];
    int i, k;
    int b, d;
    int c = 0;

    for (i = 0; i < 2000; i++)
        r[i] = 2000;

    for (k = 2800; k > 0; k -= 14)
    {
        d = 0;
        i = k;

        for (;;)
        {
            d += r[i] * 10000;
            b = 2 * i - 1;

            r[i] = d % b;
            d /= b;
            i--;

            if (i == 0)
                break;

            d *= i;
        }

        printf("%.4d", c + d / 10000);

        c = d % 10000;
    }
}

int partition(int array[], int p, int r)
{
    int x = array[p];
    int i = p, j = r, w;
    while (true)
    {
        while (array[j] > x)
            j--;
        while (array[i] < x)
            i++;
        if (i < j)
        {
            w = array[i];
            array[i] = array[j];
            array[j] = w;
            i++;
            j--;
        }
        else
            return j;
    }
}

void quicksort(int array[], int p, int r)
{
    int q;
    if (p < r)
    {
        q = partition(array, p, r);
        quicksort(array, p, q);
        quicksort(array, q + 1, r);
    }
}

int* open_file(const int tab_size)
{
    int* tab = (int*)malloc(sizeof(*tab) * tab_size);

    inputDataFile.open("inputData.csv", std::ios::out | std::ios::in);
    if (inputDataFile.is_open())
    {
        std::string line;
        getline(inputDataFile, line);

        for (int i = 0; i < tab_size; ++i)
        {
            getline(inputDataFile, line);
            tab[i] = atoi(line.c_str());
        }
        inputDataFile.close();
    }
    else
        printf("Otwieranie nie powiodlo sie");

    return tab;
}

void automated_tests()
{
    // Deklaracja zmiennych parametrow testu
    int num_of_passes, array_size;
    long elapsed = 0;
    double avg_time;

    printf("==============TESTY AUTOMATYCZNE==============\n\n");
    printf("Podaj liczbe pomiarow: ");
    scanf("%d", &num_of_passes);
    printf("Podaj wielkosc tablicy do posortowania: ");
    scanf("%d", &array_size);

    long results[num_of_passes];

    for (int i = 0; i < num_of_passes; i++)
    {
        int* array = open_file(array_size);

        start_timer();
        quicksort(array, 0, array_size - 1);
        stop_timer();

        results[i] = measured_time();
        elapsed += measured_time();

        free(array);
    }

    avg_time = elapsed / (double)num_of_passes;

    char filename[80];
    snprintf(filename, sizeof(filename), "Pomiary/quicksortResults-%ld.csv", time(nullptr));
    FILE* fout = fopen(filename, "a+");
    fprintf(fout, "Wielkość tablicy, %d\n", array_size);

    printf("Pomiary zakonczone\n");
    printf("Wyniki:\n");
    for (int i = 0; i < num_of_passes; i++)
    {
        printf("%d: %ld ns\n", i + 1, results[i]);
        fprintf(fout, "%d, %ld\n", i + 1, results[i]);
    }

    printf("Sredni czas operacji: %f ns\n", avg_time);
    fprintf(fout, "Średni czas [ns], %f\n", avg_time);

    fclose(fout);
}

void start_timer()
{
    start_time = std::chrono::high_resolution_clock::now();
}

void stop_timer()
{
    end_time = std::chrono::high_resolution_clock::now();
}

long measured_time()
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>
        (end_time - start_time).count();
}
