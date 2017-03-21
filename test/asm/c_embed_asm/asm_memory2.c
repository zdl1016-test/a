int main(int argc, char* argv[])
{
    int i = 0;
    int * p = (int*)&i;
    *p = 9999;
    if (*p==9999)
    {
        return 2;
    }
    return 0;
}
