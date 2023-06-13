void setup();
void loop();

int notmain(void)
{
    setup();
    for (;;) {
        loop();
    }
}
