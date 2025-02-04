int main()
{
    int a = 5;
    int *ptr = &a;

    // This is a multiplication of the pointer value with 2
    int b = *ptr * 2;  // Multiply value pointed by ptr (not dereference)
}