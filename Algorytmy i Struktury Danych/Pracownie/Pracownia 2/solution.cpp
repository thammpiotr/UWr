#include <iostream>

// ======================= STRUKTURY =======================
struct MultTableElem {
    long long value;
    int row;
    int col;

    MultTableElem(long long val = 0, int row = 0, int col = 0) : value(val), row(row), col(col) {}
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
bool isVisited(MultTableElem* visited, int size, int row, int col) {
    for(int i = 0; i < size; i++) {
        if(visited[i].row == row && visited[i].col == col) {
            return true;
        }
    }
    return false;
}

long long* findKLargestElems(int M, int k) {
    long long* result = new long long[k];
    // Ograniczamy podwajanie pojemnosci kopca
    MaxHeap heap(3*k);

    // Sledzenie dodanych wartosci aby zapobiec duplikatom
    bool* seen = new bool[M*M + 1]();

    // Sledzenie odwiedzonych wartosci aby zapobiec sprawdzaniu tej samej pozycji
    MultTableElem* visited = new MultTableElem[3*k];
    int visitedSize = 0;

    // Zaczynam od najwiekszej wartosci - M x M 
    heap.insert((long long)M * M, M, M);
    visited[visitedSize++] = MultTableElem(M*M, M, M);

    for (int found = 0; found < k && heap.getSize() > 0; ) {
        MultTableElem max = heap.findMax();
        heap.deleteMax();
        long long value = max.value;
        int row = max.row;
        int col = max.col;

        if(!seen[value]) {
            seen[value] = true;
            result[found++] = value;

            //Sprawdzaznie gornego elementu
            if (row > 1) {
                long long nextValue = (long long)(row - 1) * col;
                if (!isVisited(visited, visitedSize, row - 1, col)) {
                    heap.insert(nextValue, row - 1, col);
                    visited[visitedSize++] = MultTableElem(nextValue, row-1, col);
                }
            }
            //Sprawdzaznie lewego elementu
            if (col > 1) {
                long long nextValue = (long long)row * (col - 1);
                if(!isVisited(visited, visitedSize, row, col-1)) {
                    heap.insert(nextValue, row, col - 1);
                    visited[visitedSize++] = MultTableElem(nextValue, row, col - 1);
                }
            }
        }
    }
    delete[] seen;
    delete[] visited;
    return result;
}

// ======================= MAIN =======================
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    int M, k;
    std::cin >> M >> k;

    long long* result = findKLargestElems(M, k);

    for(int i = 0; i < k; i++) {
        std::cout << result[i] << "\n";
    }

    delete[] result;
    return 0;
}