double f(int a, double c)
{
    int b = 4;
    return a ^ (c * 2 - 4 * (32 - b));
}

void main()
{
    double v = 24;
    double u = f(25, v);
    return;
}
