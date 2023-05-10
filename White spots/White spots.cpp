#include <iostream>

enum class move_direction {e_left, e_right, e_bottom, e_top, e_none};

void find_spot(int r, int c, move_direction prev_move_dir, bool** field, bool** found_spots, int N, int M, int cnt = 0)
{
    if (field[r][c] == 1 && cnt < N * M)
    {
        cnt++;
        found_spots[r][c] = 1;
        if (r > 0 && prev_move_dir != move_direction::e_top && found_spots[r - 1][c] == 0)
        {
            find_spot(r - 1, c, move_direction::e_bottom, field, found_spots, N, M, cnt);
        }
        if (r < N - 1 && prev_move_dir != move_direction::e_bottom && found_spots[r + 1][c] == 0)
        {
            find_spot(r + 1, c, move_direction::e_top, field, found_spots, N, M, cnt);
        }
        if (c > 0 && prev_move_dir != move_direction::e_left && found_spots[r][c - 1] == 0)
        {
            find_spot(r, c - 1, move_direction::e_right, field, found_spots, N, M, cnt);
        }
        if (c < M - 1 && prev_move_dir != move_direction::e_right && found_spots[r][c + 1] == 0)
        {
            find_spot(r, c + 1, move_direction::e_left, field, found_spots, N, M, cnt);
        }
    }
}

int main()
{
    int spots_count = 0;

    int N, M;
    std::cin >> N >> M;

    bool** found_spots = new bool*[N];
    bool** field = new bool* [N];
    for (int i = 0; i < N; i++)
    {
        found_spots[i] = new bool[M];
        field[i] = new bool[M];
        for (int j = 0; j < M; j++)
        {
            found_spots[i][j] = 0;
            std::cin >> field[i][j];
        }
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (field[i][j] == 1 && found_spots[i][j] == 0)
            {
                find_spot(i, j, move_direction::e_none, field, found_spots, N, M);
                spots_count++;
            }
        }
    }

    std::cout << spots_count;
}


