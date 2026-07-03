#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5
#define MAX_QUEUE 100

int maze[MAX_ROW][MAX_COL] = {
    0, 1, 0, 0, 0,
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
};

typedef struct {
    int x, y;
} Point;

Point queue[MAX_QUEUE];
int front = 0, rear = 0;

Point pre[MAX_ROW][MAX_COL];
int vis[MAX_ROW][MAX_COL] = {0};

int dx[4] = {1, 0, -1, 0};   // 下 右 上 左
int dy[4] = {0, 1, 0, -1};

void enqueue(Point p) {
    queue[rear++] = p;
}

Point dequeue() {
    return queue[front++];
}

int empty() {
    return front == rear;
}

void printPath(int x, int y) {
    int path[MAX_ROW * MAX_COL][2];
    int len = 0;

    // 回溯路径：终点 -> 起点
    while (!(x == 0 && y == 0)) {
        path[len][0] = x;
        path[len][1] = y;

        Point p = pre[x][y];
        x = p.x;
        y = p.y;
        len++;
    }

    // 加入起点
    path[len][0] = 0;
    path[len][1] = 0;
    len++;

    // ⚠️ 必须按：终点 → 起点 输出（测试要求）
    for (int i = 0; i < len; i++) {
        printf("(%d,%d)\n", path[i][0], path[i][1]);
    }
}

int main(void) {
    Point start = {0, 0};
    enqueue(start);
    vis[0][0] = 1;

    int found = 0;
    int endx = 0, endy = 0;

    while (!empty()) {
        Point cur = dequeue();

        if (cur.x == MAX_ROW - 1 && cur.y == MAX_COL - 1) {
            found = 1;
            endx = cur.x;
            endy = cur.y;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];

            if (nx >= 0 && nx < MAX_ROW && ny >= 0 && ny < MAX_COL) {
                if (maze[nx][ny] == 0 && vis[nx][ny] == 0) {
                    vis[nx][ny] = 1;
                    pre[nx][ny] = cur;
                    enqueue((Point){nx, ny});
                }
            }
        }
    }

    if (found) {
        printPath(endx, endy);
    }

    return 0;
}