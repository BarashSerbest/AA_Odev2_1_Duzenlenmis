#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define A_SIZE 20
#define G_SIZE 10
#define MAX_W 10

//Max deger icin sonsuz sayi
#define INF INT_MAX

void generate(int a[], int size);
void function1(int a[], int size);
int function2(int a[], int size);
void function3(int g[][G_SIZE], int d[][G_SIZE], int size);
void print1(int a[], int size);
void print2(int g[][G_SIZE], int size);
void print3(int g[][G_SIZE], int size, int t);

int main() {
    int a[A_SIZE];
    int g[G_SIZE][G_SIZE] = {
    { 0, 10,  3,  0,  0,  5,  0, 17,  0, 22},
    {10,  0,  5,  0,  2,  0, 13,  0,  0,  0},
    { 3,  5,  0,  2,  0,  4,  0, 21,  0, 11},
    { 0,  0,  2,  0,  7,  0,  6,  0,  0,  0},
    { 0,  2,  0,  7,  0,  6,  0,  0, 19,  0},
    { 5,  0,  4,  0,  6,  0,  9,  3,  0,  0},
    { 0, 13,  0,  6,  0,  9,  0,  4,  0,  0},
    {17,  0, 21,  0,  0,  3,  4,  0,  8,  0},
    { 0,  0,  0,  0, 19,  0,  0,  8,  0,  5},
    {22,  0, 11,  0,  0,  0,  0,  0,  5,  0}
    };
    int d[G_SIZE][G_SIZE];
    int t;

    // Rastgele sayi uretici icin seed
    srand(time(NULL));

    // Calisma suresinin hesaplanmasi
    clock_t start, end;
    double time;

    start = clock();

    generate(a, A_SIZE);
    printf("A:\n");
    print1(a, A_SIZE);

    function1(a, A_SIZE);
    printf("\nFunction1 Sonuc:\n");
    print1(a, A_SIZE);

    t = function2(a, A_SIZE);
    printf("\nFunction2 Sonuc: %d\n", t);

    printf("\nG:\n");
    print2(g, G_SIZE);

    function3(g, d, G_SIZE);
    printf("\nFunction3 Sonuc:\n");
    print2(d, G_SIZE);

    printf("\n%d icin sonuc:\n", t);
    print3(d, G_SIZE, t);

    end = clock();
    time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nCalisma Suresi: %f sn\n", time);

    return 0;
}

void generate(int a[], int size) {
    for (int i = 0; i < size; i++) {
        a[i] = rand() % (2 * MAX_W) - MAX_W;
    }
}
// Function 1 => Quick Sort algoritmasý kullanýlarak deðiþtirildi
void function1(int a[], int low, int high) {
    if (low < high) {
        int pivot = partition(a, low, high);
        function1(a, low, pivot - 1);
        function1(a, pivot + 1, high);
    }
}

int partition(int a[], int low, int high) {
    int pivot = a[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (a[j] <= pivot) {
            i++;
            swap(&a[i], &a[j]);
        }
    }

    swap(&a[i + 1], &a[high]);
    return i + 1;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
//Function 2 => Bu fonksiyonun zaman karmaþýklýðýný O(n) zaman karmaþýklýðýna getirebilmek en uzun artan alt dizinin (Longest Increasing Subarray) bulunmasý saðlandý. 
int function2(int a[], int size) {
    int maxSum = a[0];
    int currentSum = a[0];
    int startIndex = 0;
    int endIndex = 0;

    for (int i = 1; i < size; i++) {
        if (a[i] > currentSum + a[i]) {
            currentSum = a[i];
            startIndex = i;
            endIndex = i;
        } else {
            currentSum = currentSum + a[i];
            endIndex = i;
        }

        if (currentSum > maxSum) {
            maxSum = currentSum;
        }
    }

    // En uzun artan alt dizinin baþlangýç ve bitiþ indekslerini yazdýrýyoruz:
    printf("En uzun artan alt dizi: [%d, %d]\n", startIndex, endIndex);

    return maxSum;
}

// Function 3 => Bu fonksiyon zaman karmaþýklýðýný O(n^3)'ün altýna indirmek için Dijkstra Algoritmasý ile düzenlendi.
void function3(int g[][G_SIZE], int d[][G_SIZE], int size) {
    int visited[G_SIZE];
    int distance[G_SIZE];

    // Baþlangýç noktasý
    int start = 0;

    // visited ve distance dizilerini baþlangýç deðerleriyle deðiþtirme
    for (int i = 0; i < size; i++) {
        visited[i] = 0;
        distance[i] = INF;
    }

    // Baþlangýç noktasýnýn mesafesini 0 olarak ayarlama
    distance[start] = 0;

    // Dijkstra algoritmasý
    for (int i = 0; i < size - 1; i++) {
        // En kýsa mesafedeki düðümü bul
        int minDistance = INF;
        int minIndex = -1;

        for (int j = 0; j < size; j++) {
            if (!visited[j] && distance[j] < minDistance) {
                minDistance = distance[j];
                minIndex = j;
            }
        }

        // Bulunan düðümü ziyaret etti olarak iþaretleme
        visited[minIndex] = 1;

        // En kýsa mesafedeki düðümü kullanarak komþularýnýn mesafelerini güncelleme
        for (int k = 0; k < size; k++) {
            if (!visited[k] && g[minIndex][k] != 0 && distance[minIndex] != INF &&
                distance[minIndex] + g[minIndex][k] < distance[k]) {
                distance[k] = distance[minIndex] + g[minIndex][k];
            }
        }
    }

    // Elde edilen en kýsa mesafeleri d dizisine aktarma
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == start) {
                d[start][j] = distance[j];
            } else if (i != j && g[i][j] != 0) {
                d[i][j] = distance[j] - distance[i] + g[i][j];
            } else {
                d[i][j] = INF;
            }
        }
    }
}

void print1(int a[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", a[i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
}

void print2(int g[][G_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (g[i][j] == INF) {
                printf("INF ");
            }
            else {
                printf("%3d ", g[i][j]);
            }
        }
        printf("\n");
    }
}

void print3(int d[][G_SIZE], int size, int t) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i != j && d[i][j] < t) {
                printf("%c -> %c: %d\n", 'A' + i, 'A' + j, d[i][j]);
            }
        }
    }
}
