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
            // Podawajam pojemnosc gdy kopiec jest pelny, tworze nowy kopiec
            if (size == capacity) {
                capacity *= 2;
                MultTableElem* newHeap = new MultTableElem[capacity];
                for(int i = 0; i < size; i++) {
                    newHeap[i] = heap[i];
                }
                delete[] heap;
                heap = newHeap;
            }
            heap[size] = MultTableElem(value, row, col);
            size++;
            moveUp(size - 1);
        }

        void deleteMax() {
            heap[0] = heap[size - 1];
            size--;
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
    // Ograniczamy podwajanie pojemnosci kopca
    MaxHeap heap(100005);

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

        // OBSERWACJA: Jezeli jestesmy na przrkatnej to nie musimy wstawiac dwoch elementow do kopca, wiemy ze dwojka "dzieci" jest sobie rowna
        if (col > 1) {
            heap.insert((long long)row * (col - 1), row, col - 1);
        }
        if (col != row && row > 1) {
            heap.insert((long long)col * (row - 1), row-1, col);
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