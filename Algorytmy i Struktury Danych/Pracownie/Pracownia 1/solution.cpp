#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <deque>

// ====================== FUNKCJE POMOCNICZNE ======================
void createSidewalk(
    const int bricks[][3], 
    std::unordered_map<int, std::vector<int>>& connectionMap, 
    int* parent, 
    bool& isSidewalkFound, 
    int& lastElementIdx,
    int numberOfBricks) {

    bool* checked = new bool[numberOfBricks]();
    std::queue<int> queue;

    // Znajduje kostki początkowe (l === 0)
    for (int i = 0; i < numberOfBricks; i++) {
        if(bricks[i][0] == 0) {
            queue.push(i);
            checked[i] = true;
            parent[i] = -1;
        }
    }

    while (!queue.empty() && !isSidewalkFound) {
        int brickIdx = queue.front();
        queue.pop();

        // Pobiera wartość r
        int connectVal = bricks[brickIdx][2];

        // Sprawdzenie czy to kostka końcowa (r === 0)
        if (connectVal == 0) {
            isSidewalkFound = true;
            lastElementIdx = brickIdx;
            break;
        }

        // Jezeli nie jest to kostka koncowa, znajduje wszystkie mozliwe nastepniki
        if(connectionMap.count(connectVal)) {
            for(int nextBrick : connectionMap[connectVal]) {
                if (!checked[nextBrick]) {
                    checked[nextBrick] = true;
                    queue.push(nextBrick);
                    parent[nextBrick] = brickIdx;
                }
            }
        }
    }
    delete[] checked;
}

void returnSidewalk(int lastElementIdx, const int* parent, const int bricks[][3]) {
    std::deque<int> sidewalk;

    // Na podstawie mapy rodzicow, tworzy chodnik
    for (int brickIdx = lastElementIdx; brickIdx != -1; brickIdx = parent[brickIdx]) {
        sidewalk.push_front(brickIdx);
    }

    std::cout << sidewalk.size() << "\n";

    for (int i : sidewalk) {
        std::cout << bricks[i][0] << " " << bricks[i][1] << " " << bricks[i][2] << "\n";
    }
}

// ====================== MAIN ======================
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Wczytanie danych i inicjalizacja
    int numberOfBricks;
    std::cin >> numberOfBricks;

    int (*bricks)[3] = new int[numberOfBricks][3];
    std::unordered_map<int, std::vector<int>> connectionMap;
    int* parent = new int[numberOfBricks];

    for(int i = 0; i < numberOfBricks; i++) {
        parent[i] = -1;
    }

    bool isSidewalkFound = false;
    int lastElementIdx = -1;

    for (int i = 0; i < numberOfBricks; i++) {
        std::cin >> bricks[i][0] >> bricks[i][1] >> bricks[i][2];
        connectionMap[bricks[i][0]].push_back(i);
    }

    createSidewalk(bricks, connectionMap, parent, isSidewalkFound, lastElementIdx, numberOfBricks);

    if (isSidewalkFound) {
        returnSidewalk(lastElementIdx, parent, bricks);
    } else {
        std::cout << "BRAK\n";
    }

    delete[] bricks;
    delete[] parent;

    return 0;
}