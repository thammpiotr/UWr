#include <iostream>

// ======================= STRUKTURY =======================
struct MultTableElem {
    long long value;
    int row;
    int col;

    MultTableElem(long long val = 0, int r = 0, int c = 0) : value(val), row(r), col(c) {}
};

class MaxHeap {
    private:
        MultTableElem* heap;
        int size;
        int capacity;

        void moveDown(int i) {
            int k = i;
            int j = -1;

            while (j != k) {
                j = k;
                int left = 2 * j + 1;
                int right = 2 * j + 2;

                if (left < size && heap[left].value > heap[k].value) {
                    k = left;
                }
                if(right < size && heap[right].value > heap[k].value) {
                    k = right;
                }
                if(j != k) {
                    std::swap(heap[j], heap[k]);
                }
             }
        }

        void moveUp(int i) {
            int k = i;
            int j = -1;

            while (j != k) {
                j = k;
                int parent = (j - 1) / 2;
                if (j > 0 && heap[parent].value < heap[k].value) {
                    k = parent;
                }
                if(j != k) {
                    std::swap(heap[j], heap[k]);
                }
            }
        }

    public:
        MaxHeap(int initCapacity = 0) : size(0), capacity(initCapacity) {
            heap = new MultTableElem[capacity];
        }
        ~MaxHeap() {
            delete[] heap;
        }

        void insert(long long value, int row, int col) {
            heap[size++] = MultTableElem(value, row, col);
            moveUp(size - 1);
        }

        void deleteMax() {
            heap[0] = heap[--size];
            moveDown(0);
        }

        MultTableElem findMax() {
            return heap[0];
        }

        int getSize() {
            return size;
        }
};

// ======================= FUNKCJE =======================
void printKLargestElems(int M, int k) {
    // Inicjalizacja kopca (nigdy nie przekroczymy M)
    MaxHeap heap(M);

    // Zaczynam od najwiekszej wartosci - M x M
    heap.insert((long long)M * M, M, M);

    long long lastMax = (long long) M*M + 1;

    for (int found = 0; found < k && heap.getSize() > 0; ) {
        MultTableElem max = heap.findMax();
        heap.deleteMax();
        long long value = max.value;
        int row = max.row;
        int col = max.col;

        // Unikamy duplikatow
        if (value < lastMax ) {
            std::cout << value << "\n";
            lastMax = value;
            found++;
        }

        // OBSERWACJA: Jezeli nie jestesmy na przekatnej to nie musimy wstawiac dwoch elementow do kopca.
        // Mozemy patrzec tylko na "gorny" lub w tym wypadku "dolny" trojkat tabliczki i dodawac element u gory(gorny trojkat) lub po lewej (dolny trojkat - w tym wypadku).
        // A elementy na przekatnej dodaja nam dodatkowo nastpny element na przekatnej.
        if (col > 1) {
            heap.insert((long long)(col - 1) * row, row, col-1);
        }
        if (col == row && row > 1 && col > 1) {
            heap.insert((long long)(row - 1) * (col - 1), row - 1, col - 1);
        }
    }
}

// ======================= MAIN =======================
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    int M, k;
    std::cin >> M >> k;

    printKLargestElems(M, k);

    return 0;
}