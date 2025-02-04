void test10()
{
    int a = 10, b = 20;
    int* p = &a;
    int& ref = a;
    
    *p = b;      // ptr deref
    p++;         // ptr arith
    int x = a & b;  // bitwise operation AND, no flaggning expected
}

int main()
{
    test10();
    return 0;
}
