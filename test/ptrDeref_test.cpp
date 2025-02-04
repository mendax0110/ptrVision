void test5()
{
    int a = 10, b = 20;
    int* p = &a;

    *p = b;      // ptr deref
    int* q = &b; // addres of the operator

    p = q;        // ptr assign
}

int main()
{
    test5();
    return 0;
}
