#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

enum EpisodesType {
    Basic = 7,
    Main = 11,
    Filler = 12,
    Additional = 14,
    Manga = 13,
    Ranobe = 18
};


template<typename T>
class State
{
private:
    T value;
public:
    State(T value) : value(value) {}
    T getValue() { return value; }
    void setValue(T value) { this->value = value; }
};

template<typename T>
class SeriesManager
{
private:

    State<T>* viewed_episodes = nullptr;
    State<T>* current_episode = nullptr;
public:

    SeriesManager(T value) {
        current_episode = new State<T>(value);
    }
    void setState(T value) {
        if (viewed_episodes != nullptr)
            delete viewed_episodes;
        viewed_episodes = new State<T>(current_episode->getValue());
        if (current_episode != nullptr)
            delete current_episode;
        current_episode = new State<T>(value);
    }
    State<T>* getState() { return current_episode; }
    State<T>* getOldState() { return viewed_episodes; }

    T getStateValue() {
        return current_episode->getValue();
    }
    T getOldStateValue() {
        return viewed_episodes->getValue();
    }


    ~SeriesManager() {
        if (current_episode != nullptr)
            delete current_episode;
        if (viewed_episodes != nullptr)
            delete viewed_episodes;
    }
};

class Message {
private:
    string text;
    EpisodesType type;

    void reset_color()
    {
        SetConsoleTextAttribute(hConsole, EpisodesType::Basic);
    }
public:
    Message() {
        text = "";
        type = EpisodesType::Basic;
    }
    Message(string text, EpisodesType type) {
        this->text = text;
        this->type = type;
    }
    void setText(string text) {
        this->text = text;
    }
    void show_line()
    {
        SetConsoleTextAttribute(hConsole, type);
        cout << text << endl;
        reset_color();
    }

    void setType(EpisodesType type) {
        this->type = type;
    }
    ~Message() {
        SetConsoleTextAttribute(hConsole, EpisodesType::Basic);
        cout << "Destructor called" << endl;
        reset_color();
    }
};
int main() {
    SeriesManager <int>* seriesManager = new SeriesManager<int>(0);
    Message * message = new Message("Filler", EpisodesType::Filler);
    cout << "Viewed episode: ";
    (seriesManager->getOldState() == nullptr ? cout << "No Viewed" : cout << seriesManager->getOldStateValue()) << " ";
    cout << endl;
    cout << "Current episode: " << seriesManager->getStateValue() << " ";
    message->show_line();

    message = new Message("Main", EpisodesType::Main);
    cout << "Next episode: " << seriesManager->getStateValue() + 1<< " ";
    message->show_line();
    cout << endl;

    seriesManager->setState(2);

    message = new Message("Filler", EpisodesType::Filler);
    cout << "Viewed episode: " << seriesManager->getOldStateValue() << " ";
    message->show_line();
    message = new Message("Ranobe", EpisodesType::Ranobe);
    cout << "Current episode: " << seriesManager->getStateValue() << " ";
    message->show_line();

    message = new Message("Additional", EpisodesType::Additional);
    cout << "Next episode: " << seriesManager->getStateValue() + 1 << " ";
    message->show_line();
    cout << endl;

    return 0;
}