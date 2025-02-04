void test4()
{
    int a = 10;
    void* p = &a;

    int* int_ptr = static_cast<int*>(p);  // ptr cast
    *int_ptr = 20;                        // ptr deref after cast
}

int main()
{
    test4();
    return 0;
}
