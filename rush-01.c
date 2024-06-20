#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>

#define SIZE 4

// دالة لتحويل سلسلة نصية إلى عدد صحيح
int my_atoi(const char* str)
{
    int num = 0;
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            num = num * 10 + (str[i] - '0');
        }
        else
        {
            return -1; // إذا كانت السلسلة تحتوي على حرف غير رقمي
        }
        i++;
    }
    return num;
}

// دالة للتحقق من صحة الصف
int is_valid_row(int** grid, int row)
{
    int seen[SIZE + 1] = { 0 };
    int col = 0;
    while (col < SIZE)
    {
        int num = grid[row][col];
        if (num < 1 || num > SIZE || seen[num])
            return 0;
        seen[num] = 1;
        col++;
    }
    return 1;
}

// دالة للتحقق من صحة العمود
int is_valid_col(int** grid, int col)
{
    int seen[SIZE + 1] = { 0 };
    int row = 0;
    while (row < SIZE)
    {
        int num = grid[row][col];
        if (num < 1 || num > SIZE || seen[num])
            return 0;
        seen[num] = 1;
        row++;
    }
    return 1;
}

// دالة للتحقق من الرؤية من اتجاه معين
int is_visible(int** grid, int index, int hint, int row_mode, int reverse)
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
int is_valid_grid(int** grid, int top_hints[SIZE], int bottom_hints[SIZE], int left_hints[SIZE], int right_hints[SIZE])
{
    int i = 0;
    while (i < SIZE)
    {
        if (!is_valid_row(grid, i) || !is_valid_col(grid, i))
            return 0;
        if (!is_visible(grid, i, left_hints[i], 1, 0) ||
            !is_visible(grid, i, right_hints[i], 1, 1) ||
            !is_visible(grid, i, top_hints[i], 0, 0) ||
            !is_visible(grid, i, bottom_hints[i], 0, 1))
            return 0;
        i++;
    }
    return 1;
}

// دالة لطباعة الشبكة فقط بدون التلميحات
void print_grid(int** grid)
{
    int i = 0, j;
    char num_str[4];

    // طباعة الجدول بدون التلميحات
    i = 0;
    while (i < SIZE)
    {
        j = 0;
        while (j < SIZE)
        {
            snprintf(num_str, sizeof(num_str), " %d", grid[i][j]);
            write(1, num_str, 2); // طباعة الرقم
            j++;
        }
        write(1, "\n", 1); // إنهاء الصف
        i++;
    }
}

// دالة للتحقق من إمكانية وضع الرقم في الموقع المحدد
int can_place_number(int** grid, int row, int col, int num)
{
    int i = 0;
    while (i < SIZE)
    {
        if (grid[row][i] == num || grid[i][col] == num)
            return 0;
        i++;
    }
    return 1;
}

// دالة لحل اللغز باستخدام طريقة البحث عن الحلول الممكنة
int solve(int** grid, int top_hints[SIZE], int bottom_hints[SIZE], int left_hints[SIZE], int right_hints[SIZE], int row, int col)
{
    if (row == SIZE)
        return is_valid_grid(grid, top_hints, bottom_hints, left_hints, right_hints);

    if (col == SIZE)
        return solve(grid, top_hints, bottom_hints, left_hints, right_hints, row + 1, 0);

    int num = 1;
    while (num <= SIZE)
    {
        if (can_place_number(grid, row, col, num))
        {
            grid[row][col] = num;
            if (solve(grid, top_hints, bottom_hints, left_hints, right_hints, row, col + 1))
                return 1;
            grid[row][col] = 0;
        }
        num++;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc != 4 * SIZE + 1) {
        write(1, "Error\n", 6);
        return 1;
    }

    // حجز ذاكرة للشبكة باستخدام malloc
    int** grid = (int**)malloc(SIZE * sizeof(int*));
    int i = 0;
    while (i < SIZE)
    {
        grid[i] = (int*)malloc(SIZE * sizeof(int));
        int j = 0;
        while (j < SIZE)
        {
            grid[i][j] = 0; // قيمة ابتدائية لخلايا الجدول
            j++;
        }
        i++;
    }

    // تعريف التلميحات وقراءة التلميحات من argv باستخدام my_atoi
    int top_hints[SIZE];
    int bottom_hints[SIZE];
    int left_hints[SIZE];
    int right_hints[SIZE];

    i = 0;
    while (i < SIZE)
    {
        top_hints[i] = my_atoi(argv[1 + i]);
        bottom_hints[i] = my_atoi(argv[1 + SIZE + i]);
        left_hints[i] = my_atoi(argv[1 + 2 * SIZE + i]);
        right_hints[i] = my_atoi(argv[1 + 3 * SIZE + i]);
        i++;
    }

    // حل اللغز
    if (solve(grid, top_hints, bottom_hints, left_hints, right_hints, 0, 0))
    {
        // طباعة الشبكة بعد إيجاد الحل
        print_grid(grid);
    }
    else
    {
        write(1, "Error\n", 6);
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