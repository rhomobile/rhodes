

extern "C" void Init_Development_API();

extern "C" void init_Development_HTTP_Server();

extern "C" void Init_UpdateBundle();

extern "C" void Init_LogAccess();

static bool already_initialised = false;


extern "C" void Development_Init() {
    if (!already_initialised) {
        Init_Development_API();
        init_Development_HTTP_Server();
        Init_UpdateBundle();
        Init_LogAccess();
    }
    already_initialised = true;
}

