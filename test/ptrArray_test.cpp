void test3()
{
    int a = 10, b = 20, c = 30;
    int* arr[3] = {&a, &b, &c};

    *arr[0] = 50;  // deref prt in array
    *arr[1] = 60;
    *arr[2] = 70;
}

int main()
{
    test3();
    return 0;
}
