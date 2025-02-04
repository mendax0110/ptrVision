void test1()
{
    int* p = nullptr;
    
    // deref a null pointer
    *p = 10;  // flag warning/error
}

int main()
{
    test1();
    return 0;
}
