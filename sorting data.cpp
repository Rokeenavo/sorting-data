#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

void create_file()                                              // Создание файла с случайными числами
{
    std::ofstream file("data.txt");

    if (!file)
    {
        std::cout << "Не удалось открыть файл." << std::endl;
        return;
    }

    std::random_device rd;                                      // Создание объекта random_device для генерации случайных чисел
    std::mt19937 gen(rd());                                     // Создание генератора случайных чисел с использованием random_device в качестве источника энтропии
    std::uniform_int_distribution<long> dis(1, 1000000);        // Создание распределения равномерных целых чисел в заданном диапазоне

    // Запись случайных чисел в файл
    for (long i = 0; i < 1000000; ++i)
    {
        long num = dis(gen);
        file << num << "\n";
    }

    file.close();
    std::cout << "Файл успешно создан." << std::endl;
}

// Merge Sort // Сортировка слиянием

void merge(std::vector<long>& arr, long l, long m, long r)
{
    long n1 = m - l + 1;
    long n2 = r - m;

    std::vector<long> L(n1), R(n2);

    for (long i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (long j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    long i = 0, j = 0;
    long k = l;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<long>& arr)
{
    long n = arr.size();

    for (long curr_size = 1; curr_size <= n - 1; curr_size = 2 * curr_size)
    {
        for (long left_start = 0; left_start < n - 1; left_start += 2 * curr_size)
        {
            long mid = std::min(left_start + curr_size - 1, n - 1);
            long right_end = std::min(left_start + 2 * curr_size - 1, n - 1);

            merge(arr, left_start, mid, right_end);
        }
    }
}

// Quick Sort // Быстрая сортировка

int partition(std::vector<long>& arr, int low, int high)
{
    long pivot = arr[high];                                     // Функция разделения массива для быстрой сортировки
    int i = low - 1;

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(std::vector<long>& arr, int low, int high)
{
    if (low < high)                                                 // Рекурсивная сортировка
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Heap Sort // Сортировка кучей
void heapify(std::vector<long>& arr, long n, long i)
{
    long largest = i;                                               // Преобразование массива в кучу
    long left = 2 * i + 1;
    long right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i)
    {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<long>& arr)                           // Сортировка кучей
{
    long n = arr.size();

    for (long i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (long i = n - 1; i > 0; i--)
    {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void writeToFile(const std::string& filename, const std::vector<long>& data)
{
    std::ofstream file(filename);
    if (file.is_open())
    {
        for (long value : data)
        {
            file << value << "\n";
        }
        file.close();
    }
    else
    {
        std::cout << "Не удалось открыть файл для записи." << std::endl;
    }
}

void measureAndWriteTime(const std::string& sortType, std::vector<long>& data)
{
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;
    std::chrono::duration<double> time;

    if (sortType == "Merge")
    {
        start = std::chrono::high_resolution_clock::now();
        mergeSort(data);
        end = std::chrono::high_resolution_clock::now();
    }
    else if (sortType == "Quick")
    {
        start = std::chrono::high_resolution_clock::now();
        quickSort(data, 0, data.size() - 1);
        end = std::chrono::high_resolution_clock::now();
    }
    else if (sortType == "Heap")
    {
        start = std::chrono::high_resolution_clock::now();
        heapSort(data);
        end = std::chrono::high_resolution_clock::now();
    }

    time = end - start;

    std::cout << sortType << " Sort занял " << time.count() << " секунд" << std::endl;

    std::string filename = "data_" + sortType + "_sorted.txt";
    writeToFile(filename, data);
}

int main()
{
    setlocale(LC_ALL, "Russian");
    create_file();

    std::ifstream file("data.txt");
    std::vector<long> data;
    long value;
    while (file >> value)
    {
        data.push_back(value);
    }

    std::vector<long> dataMerge = data;
    std::vector<long> dataQuick = data;
    std::vector<long> dataHeap = data;

    measureAndWriteTime("Merge", dataMerge);
    measureAndWriteTime("Quick", dataQuick);
    measureAndWriteTime("Heap", dataHeap);

    return 0;
}