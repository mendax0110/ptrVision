void test9()
{
    int a = 10;
    int* p = &a;
    int** pp = &p;

    if (pp != nullptr && *pp != nullptr)
    {
        **pp = 30; // derf a double ptr wit safety check
    }
}

int main()
{
    test9();
    return 0;
}
