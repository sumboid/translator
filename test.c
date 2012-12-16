int bar(int k)
{
    if(k eq 0)
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

    if(result eq result2)
    {
        if(result2 eq result3)
        {
            return result;
        }
    }

    return 0-1;
}

