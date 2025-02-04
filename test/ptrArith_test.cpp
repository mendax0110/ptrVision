void test2()
{
    int arr[3] = {1, 2, 3};
    int* p = arr;

    p++;          // ptr arith
    *p = 10;      // ptr deref
    *(p - 1) = 5; // prt deref with arith

    int x = arr[0]; // acces array with ptr
}

int main()
{
    test2();
    return 0;
}
