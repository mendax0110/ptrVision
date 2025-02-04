struct MyStruct
{
    int x;
    int y;
};

void test7()
{
    MyStruct s = {1, 2};
    MyStruct* p = &s;

    p->x = 10;   // ptr to member
    int z = p->y; // deref a ptr to struct member
}

int main()
{
    test7();
    return 0;
}
