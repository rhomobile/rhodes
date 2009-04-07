void runAllLogTests();

#ifndef OS_SYMBIAN
int main(int argc, char* argv[]) {
    runAllLogTests();

    return 0;
}
#endif
