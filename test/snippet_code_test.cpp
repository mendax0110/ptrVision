int function()
{
    // ptr arithmetic
    int arr[5] = {1, 2, 3, 4, 5};
    int *ptr = arr;
    ptr += 2;
    return *ptr;
}

int main()
{
    //different ptr arithmetic
    int urr[3] = {1, 2, 3};
    int *lol = urr;
    lol += 1;
    return *lol;

    $CODE function();
    return 0;
}