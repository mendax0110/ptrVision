void foo(int &x, int y)
{
    x = x + y;
}

int main()
{
    int a = 5;
    int b = 10;
    
    // Bitwise AND in an expression
    int result = a & b;  // Bitwise AND
    
    // Reference passed to function
    foo(a, b);  // Passing reference to function
}
