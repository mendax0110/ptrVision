void test8()
{
    void (*func_ptr)(int);
    func_ptr = [](int x) { (void)x; };  // no-op with the arg

    func_ptr(10); // call function ptr
}

int main()
{
    test8();
    return 0;
}
