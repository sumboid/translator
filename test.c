int foo(int k)
{
    int p = k;
    int result = p;

    while(p > 0)
    {
        p = p - 1;
        result = result + p;
    }
    return result;
}
