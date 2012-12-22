int bar(int k)
{
    if(k == 0)
    {
        return 0;
    }

    return k + bar(k - 1);
}

int foo(int k)
{
    int p = k;
    int result = p;

    while(p > 0)
    {
        p = p - 1;
        result = result + p;
    }

    int result2 = k * (1 + k) / 2;
    int result3 = bar(k);

    if(result == result2)
    {
        if(result2 == result3)
        {
            return -result;
        }
    }

    return -1;
}

