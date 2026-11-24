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


    Pet(float talent_learn, float talent_experiment, float talent_play);
    ~Pet();

    void learn(int points);
    bool eat(int points);
    void play(int points);
    int experiment(int points);
    void update();

    void save_stats();

    private: // Randomly generated stats at booting

        const float learn_coef;
        const float experiment_coef;
        const float play_coef;

        float learnProgress;
        float experimentProgress;
        float playProgress;

        int lastAgeMillis; //Derniere actualisation de l'age (en ms)


};