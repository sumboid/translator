int foo(int k)
{
    if(k eq 0)
    {
        return 0;
    }

    return k + foo(k - 1);
}

int main()
{
    return foo(5);
}
