
class HumanVerifier {
public:
    bool smile= false;
    bool screen = false;
    bool square_object = false;
    int last_update = 0;
    int clock = 0;

    void saw_smile()
    {
        this->smile = true;
        this->update_time();
    }

    void saw_screen()
    {
        this->screen = true;
        this->update_time();
    }

    void saw_sqare()
    {
        this->square_object = true;
        this->update_time();
    }

    bool is_human()
    {
        return this->smile && !this->screen && !this->square_object;
    }

    void tick()
    {
        clock++;
        if (clock > last_update + 100) {
            this->smile = false;
            this->screen = false;
            this->square_object = false;
        }
        if (clock > 1000000) {
            last_update = 0;
            clock = 0;
        }
    }

private:
    void update_time()
    {
        last_update = clock;
    }
};