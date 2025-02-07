int main()
{
    int* ptr = new int(10);
    if (ptr == nullptr)
    {
        return 1;
    }

    delete ptr;

    int var = 5;
    int& ref = var;

    int* arr = new int[5];
    for (int i = 0; i < 5; ++i)
    {
        arr[i] = i * 2;
    }

    for (int i = 0; i < 5; ++i)
    {
        int a = *(arr + i);
    }

    int x = 5, y = 3;
    int z = x & y;

    int i = 0;
    start:
    if (i < 5)
    {
        i++;
        goto start;
    }

    delete[] arr;

    return 0;
}
