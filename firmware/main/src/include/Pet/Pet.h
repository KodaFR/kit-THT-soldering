class Pet {
    public:

        int health;
        int hunger;
        int age; // (ms)
        int culture;
        int happyness;
        int creativity;
        int level;
        int money;


    Pet(int talent_learn, int talent_experiment, int talent_play);
    ~Pet();

    void learn(int points);
    bool eat(int points);
    void play(int points);
    void experiment(int points);
    void update();

    void save_stats();

    private: // Randomly generated stats at booting

        const int learn_coef;
        const int experiment_coef;
        const int play_coef;


};