#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define SIZE 4

// دالة للتحقق من صحة الصف
bool is_valid_row(int** grid, int row)
{
    bool seen[SIZE + 1] = { false };
    int col = 0;
    while (col < SIZE)
    {
        int num = grid[row][col];
        if (seen[num])
            return false;
        seen[num] = true;
        col++;
    }
    return true;
}

// دالة للتحقق من صحة العمود
bool is_valid_col(int** grid, int col)
{
    bool seen[SIZE + 1] = { false };
    int row = 0;
    while (row < SIZE)
    {
        int num = grid[row][col];
        if (seen[num])
            return false;
        seen[num] = true;
        row++;
    }
    return true;
}

// دالة للتحقق من الرؤية من اتجاه معين
bool is_visible(int** grid, int index, int hint, bool row_mode, bool reverse)
{
    int max_height = 0, visible_count = 0;
    int i = 0;
    while (i < SIZE)
    {
        int height = row_mode ? (reverse ? grid[index][SIZE - 1 - i] : grid[index][i])
            : (reverse ? grid[SIZE - 1 - i][index] : grid[i][index]);
        if (height > max_height)
        {
            max_height = height;
            visible_count++;
        }
        i++;
    }
    return visible_count == hint;
}

// دالة للتحقق من صحة الشبكة بالكامل
bool is_valid_grid(int** grid, int top_hints[SIZE], int bottom_hints[SIZE], int left_hints[SIZE], int right_hints[SIZE])
{
    int i = 0;
    while (i < SIZE)
    {
        if (!is_valid_row(grid, i) || !is_valid_col(grid, i))
            return false;
        if (!is_visible(grid, i, left_hints[i], true, false) ||
            !is_visible(grid, i, right_hints[i], true, true) ||
            !is_visible(grid, i, top_hints[i], false, false) ||
            !is_visible(grid, i, bottom_hints[i], false, true))
            return false;
        i++;
    }
    return true;
}

// دالة لطباعة الشبكة مع التلميحات
void print_skyscrapers_grid(int** grid, int top_hints[SIZE], int bottom_hints[SIZE], int left_hints[SIZE], int right_hints[SIZE])
{
    int i = 0, j;
    char buffer[32];

    // طباعة التلميحات العلوية للأعمدة
    write(1, "  ", 2); // المسافة لمحاذاة التلميحات
    j = 0;
    while (j < SIZE)
    {
        snprintf(buffer, sizeof(buffer), " %d ", top_hints[j]);
        write(1, buffer, 3);
        j++;
    }
    write(1, "\n", 1);

    // طباعة الجزء العلوي من الجدول
    write(1, "  ", 2);
    for (j = 0; j < SIZE; j++)
    {
        write(1, "---", 3);
    }
    write(1, "\n", 1);

    // طباعة الجدول مع التلميحات
    i = 0;
    while (i < SIZE)
    {
        snprintf(buffer, sizeof(buffer), "%d|", left_hints[i]);
        write(1, buffer, 2);

        j = 0;
        while (j < SIZE)
        {
            if (grid[i][j] == -842150451)
            {
                write(1, " _ ", 3); // خلية فارغة
            }
            else
            {
                snprintf(buffer, sizeof(buffer), " %d ", grid[i][j]);
                write(1, buffer, 3); // طباعة الرقم
            }
            j++;
        }

        snprintf(buffer, sizeof(buffer), "|%d\n", right_hints[i]);
        write(1, buffer, 3);
        i++;
    }

    // طباعة خط القاعدة
    write(1, "  ", 2);
    for (j = 0; j < SIZE; j++)
    {
        write(1, "---", 3);
    }
    write(1, "\n", 1);

    // طباعة التلميحات السفلية للأعمدة
    write(1, "  ", 2); // الفراغات اللازمة لمحاذاة التلميحات
    j = 0;
    while (j < SIZE)
    {
        snprintf(buffer, sizeof(buffer), " %d ", bottom_hints[j]);
        write(1, buffer, 3); // طباعة التلميحات للأعمدة
        j++;
    }
    write(1, "\n", 1);
}

int main()
{
    // حجز ذاكرة للشبكة باستخدام malloc
    int** grid = (int**)malloc(SIZE * sizeof(int*));
    int i = 0;
    while (i < SIZE)
    {
        grid[i] = (int*)malloc(SIZE * sizeof(int));
        int j = 0;
        while (j < SIZE)
        {
            grid[i][j] = -842150451; // قيمة ابتدائية لخلايا الجدول
            j++;
        }
        i++;
    }

    // تعريف التلميحات
    int top_hints[SIZE] = { 4, 3, 2, 1 };
    int bottom_hints[SIZE] = { 1, 2, 2, 2 };
    int left_hints[SIZE] = { 4, 3, 2, 1 };
    int right_hints[SIZE] = { 1, 2, 2, 2 };

    // طباعة الشبكة الأولية مع التلميحات
    print_skyscrapers_grid(grid, top_hints, bottom_hints, left_hints, right_hints);

    // قراءة الأرقام من المستخدم
    write(1, "Enter the values 4x4:\n", 23);
    i = 0;
    while (i < SIZE)
    {
        int j = 0;
        while (j < SIZE)
        {
            scanf_s("%d", &grid[i][j]);
            j++;
        }
        i++;
    }

    // طباعة الشبكة بعد إدخال الأرقام مع التلميحات
    print_skyscrapers_grid(grid, top_hints, bottom_hints, left_hints, right_hints);

    // التحقق من صحة الحل
    if (is_valid_grid(grid, top_hints, bottom_hints, left_hints, right_hints))
    {
        write(1, "correct!\n", 9);
    }
    else
    {
        write(1, "no correct!\n", 12);
    }

    // الإفراج عن الذاكرة المخصصة
    i = 0;
    while (i < SIZE)
    {
        free(grid[i]);
        i++;
    }
    free(grid);

    return 0;
}
