void test6()
{
    int a = 10;
    int* p = &a;
    int** pp = &p;

    **pp = 20; // deref a double ptr
}

int main()
{
    test6();
    return 0;
}
