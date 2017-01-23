
void bug();

int main(int argc, char *argv[])
{
    bug();
    return 0;
}

void bug()
{
    int n = 3;
    int* pn = &n;
    delete pn;
}
