#include <stdio.h>
#include <string.h>

#define MAX_ROW 5
#define MAX_COL 5

int maze[MAX_ROW][MAX_COL] = {
    {0, 1, 0, 0, 0},
    {0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0},
    {0, 0, 0, 1, 0}
};

int visited[MAX_ROW][MAX_COL];
int path[MAX_ROW * MAX_COL][2];
int path_len = 0;
int found = 0;

// 下、右、上、左（必须）
int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};

void dfs(int x, int y) {
    if (found) return;

    if (x < 0 || x >= MAX_ROW || y < 0 || y >= MAX_COL)
        return;

    if (maze[x][y] == 1 || visited[x][y])
        return;

    visited[x][y] = 1;

    path[path_len][0] = x;
    path[path_len][1] = y;
    path_len++;

    if (x == MAX_ROW - 1 && y == MAX_COL - 1) {
        found = 1;
        return;
    }

    for (int i = 0; i < 4; i++) {
        dfs(x + dx[i], y + dy[i]);
        if (found) return;   // ⭐关键剪枝
    }

    visited[x][y] = 0;
    path_len--;
}

int main(void) {
    memset(visited, 0, sizeof(visited));

    dfs(0, 0);

    printf("找到通路！\n");

    // ⚠️ 必须反向输出（测试要求）
    for (int i = path_len - 1; i >= 0; i--) {
        printf("(%d, %d)\n", path[i][0], path[i][1]);
    }

    return 0;
}