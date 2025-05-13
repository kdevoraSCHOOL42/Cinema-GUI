#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <memory>
#include <stdexcept>

/// Класс "Киносеанс"
class MovieSession {
private:
    std::tm sessionDateTime;
    MovieSession* previousSession;
    MovieSession* nextSession;

public:
    MovieSession(std::tm dateTime)
        : sessionDateTime(dateTime), previousSession(nullptr), nextSession(nullptr) {}

    std::tm getDateTime() const { return sessionDateTime; }
    void setDateTime(const std::tm& dateTime) { sessionDateTime = dateTime; }

    MovieSession* getPrevious() const { return previousSession; }
    void setPrevious(MovieSession* prev) { previousSession = prev; }

    MovieSession* getNext() const { return nextSession; }
    void setNext(MovieSession* next) { nextSession = next; }
};

/// Класс "Расписание сеансов фильма" — двусвязный список
class SessionSchedule {
private:
    MovieSession* headSession;

public:
    // Copy constructor
    SessionSchedule(const SessionSchedule& other) : headSession(nullptr) {
        MovieSession* current = other.headSession;
        while (current) {
            addSession(current->getDateTime());
            current = current->getNext();
        }
    }

    // Assignment operator
    SessionSchedule& operator=(const SessionSchedule& other) {
        if (this != &other) {
            // Clean up existing sessions
            MovieSession* current = headSession;
            while (current) {
                MovieSession* toDelete = current;
                current = current->getNext();
                delete toDelete;
            }
            headSession = nullptr;

            // Copy sessions from the other schedule
            current = other.headSession;
            while (current) {
                addSession(current->getDateTime());
                current = current->getNext();
            }
        }
        return *this;
    }

    void insertOrdered(MovieSession* newSession) {
        std::tm newSessionTime = newSession->getDateTime(); // Create a copy to avoid modifying the original
        std::tm headSessionTime = headSession ? headSession->getDateTime() : std::tm();
        if (!headSession) headSessionTime = std::tm();
        if (!headSession) headSessionTime = std::tm();
        if (!headSession || std::mktime(&newSessionTime) < std::mktime(&headSessionTime)) {
            newSession->setNext(headSession);
            if (headSession) headSession->setPrevious(newSession);
            headSession = newSession;
            return;
        }

        MovieSession* current = headSession;
        while (current->getNext()) {
            std::tm nextSessionTime = current->getNext()->getDateTime(); // Create a copy
            if (std::mktime(&nextSessionTime) >= std::mktime(&newSessionTime)) break;
            current = current->getNext();
        }

        newSession->setNext(current->getNext());
        if (current->getNext()) current->getNext()->setPrevious(newSession);
        current->setNext(newSession);
        newSession->setPrevious(current);
    }

public:
    SessionSchedule() : headSession(nullptr) {}

    ~SessionSchedule() {
        MovieSession* current = headSession;
        while (current) {
            MovieSession* toDelete = current;
            current = current->getNext();
            delete toDelete;
        }
    }

    void addSession(const std::tm& dateTime) {
        MovieSession* newSession = new MovieSession(dateTime);
        insertOrdered(newSession);
    }

    bool removeSession(const std::tm& dateTime) {
        MovieSession* current = headSession;

        while (current) {
            std::tm currentDateTime = current->getDateTime(); // Create a copy
            std::tm targetDateTime = dateTime;               // Create a copy
            if (std::difftime(std::mktime(&currentDateTime), std::mktime(&targetDateTime)) == 0) {
                if (current->getPrevious()) current->getPrevious()->setNext(current->getNext());
                else headSession = current->getNext();
                if (current->getNext()) current->getNext()->setPrevious(current->getPrevious());

                delete current;
                return true;
            }
            current = current->getNext();
        }
        return false;
    }

    bool hasSession(const std::tm& dateTime) const {
        MovieSession* current = headSession;
        while (current) {
            std::tm currentDateTime = current->getDateTime(); // Create a copy
            std::tm targetDateTime = dateTime;               // Create a copy
            if (std::difftime(std::mktime(&currentDateTime), std::mktime(&targetDateTime)) == 0)
                return true;
            current = current->getNext();
        }
        return false;
    }

    int getSessionCount() const {
        int count = 0;
        MovieSession* current = headSession;
        while (current) {
            count++;
            current = current->getNext();
        }
        return count;
    }

    std::vector<std::tm> getAllSessionTimes() const {
        std::vector<std::tm> times;
        MovieSession* current = headSession;
        while (current) {
            times.push_back(current->getDateTime());
            current = current->getNext();
        }
        return times;
    }
};

/// Класс "Кинофильм"
class Movie {
private:
    std::string title;
    int durationMinutes;
    SessionSchedule schedule;

public:
    Movie(const std::string& title, int durationMinutes)
        : title(title), durationMinutes(durationMinutes) {}

    // Copy constructor for deep copy
    Movie(const Movie& other)
        : title(other.title), durationMinutes(other.durationMinutes), schedule(other.schedule) {}

    // Assignment operator for deep copy
    Movie& operator=(const Movie& other) {
        if (this != &other) {
            title = other.title;
            durationMinutes = other.durationMinutes;
            schedule = other.schedule;
        }
        return *this;
    }

    std::string getTitle() const { return title; }
    void setTitle(const std::string& newTitle) { title = newTitle; }

    int getDurationMinutes() const { return durationMinutes; }
    void setDurationMinutes(int minutes) { durationMinutes = minutes; }

    int getTotalSessionCount() const { return schedule.getSessionCount(); }
    int getTotalScreenTime() const { return schedule.getSessionCount() * durationMinutes; }

    void addSessionToSchedule(const std::tm& dateTime) { schedule.addSession(dateTime); }
    bool removeSessionFromSchedule(const std::tm& dateTime) { return schedule.removeSession(dateTime); }
    bool hasSessionAt(const std::tm& dateTime) const { return schedule.hasSession(dateTime); }
    std::vector<std::tm> getSessionTimes() const { return schedule.getAllSessionTimes(); }
};

/// Класс "Расписание кинотеатра" — кольцевая очередь фильмов
class CinemaSchedule {
private:
    int capacity;
    std::vector<std::unique_ptr<Movie> > movieArray;
    int headIndex;
    int tailIndex;
    int movieCount;

public:
    CinemaSchedule(int maxMovies)
        : capacity(maxMovies), movieArray(maxMovies), headIndex(0), tailIndex(0), movieCount(0) {}

    bool isFull() const { return movieCount == capacity; }
    bool isEmpty() const { return movieCount == 0; }

    bool addMovie(Movie&& movie) {
        if (isFull()) return false;
        movieArray[tailIndex] = std::make_unique<Movie>(std::move(movie));
        tailIndex = (tailIndex + 1) % capacity;
        movieCount++;
        return true;
    }

    bool removeMovie(const std::string& title) {
        if (isEmpty()) return false;

        for (int i = 0; i < movieCount; ++i) {
            int index = (headIndex + i) % capacity;
            if (movieArray[index]->getTitle() == title) {
                movieArray[index].reset();

                for (int j = i; j < movieCount - 1; ++j) {
                    int currentIndex = (headIndex + j) % capacity;
                    int nextIndex = (headIndex + j + 1) % capacity;
                    movieArray[currentIndex] = std::move(movieArray[nextIndex]);
                }

                tailIndex = (tailIndex - 1 + capacity) % capacity;
                movieCount--;
                return true;
            }
        }
        return false;
    }

    int getMovieCount() const { return movieCount; }
    int getCapacity() const { return capacity; }

    Movie* getMovieAt(int index) const {
        if (index < 0 || index >= movieCount)
            throw std::out_of_range("Invalid movie index");
        return movieArray[(headIndex + index) % capacity].get();
    }
};

// Функция для считывания времени
std::tm readTime() {
    std::tm time = {};
    int year, month, day, hour, minute;

    while (true) {
        std::cout << "Введите год (например, 2025): ";
        std::cin >> year;
        if (year >= 1900) break;
        std::cout << "Неверный год. Попробуйте снова.\n";
    }

    while (true) {
        std::cout << "Введите месяц (1-12): ";
        std::cin >> month;
        if (month >= 1 && month <= 12) break;
        std::cout << "Неверный месяц. Попробуйте снова.\n";
    }

    while (true) {
        std::cout << "Введите день (1-31): ";
        std::cin >> day;
        if (day >= 1 && day <= 31) break;
        std::cout << "Неверный день. Попробуйте снова.\n";
    }

    while (true) {
        std::cout << "Введите час (0-23): ";
        std::cin >> hour;
        if (hour >= 0 && hour <= 23) break;
        std::cout << "Неверный час. Попробуйте снова.\n";
    }

    while (true) {
        std::cout << "Введите минуту (0-59): ";
        std::cin >> minute;
        if (minute >= 0 && minute <= 59) break;
        std::cout << "Неверная минута. Попробуйте снова.\n";
    }

    time.tm_year = year - 1900;
    time.tm_mon = month - 1;
    time.tm_mday = day;
    time.tm_hour = hour;
    time.tm_min = minute;
    return time;
}

// Основное консольное меню
void displayMenu() {
    std::cout << "\nМеню:\n";
    std::cout << "1. Добавить фильм\n";
    std::cout << "2. Добавить сеанс\n";
    std::cout << "3. Удалить фильм\n";
    std::cout << "4. Удалить сеанс\n";
    std::cout << "5. Просмотреть все фильмы\n";
    std::cout << "6. Просмотреть сеансы фильма\n";
    std::cout << "0. Выйти\n";
}

int main() {
    CinemaSchedule cinema(5);
    int choice;

    while (true) {
        displayMenu();
        std::cout << "Ваш выбор: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string title;
            int duration;
            std::cout << "Введите название фильма: ";
            std::cin.ignore();
            std::getline(std::cin, title);
            std::cout << "Введите продолжительность фильма в минутах: ";
            std::cin >> duration;
            Movie movie(title, duration);
            if (cinema.addMovie(std::move(movie))) {
                std::cout << "Фильм добавлен!\n";
            } else {
                std::cout << "Расписание заполнено, невозможно добавить фильм.\n";
            }
            break;
        }
        case 2: {
            std::string movieTitle;
            std::cout << "Введите название фильма: ";
            std::cin.ignore();
            std::getline(std::cin, movieTitle);

            bool movieFound = false;
            for (int i = 0; i < cinema.getMovieCount(); ++i) {
                Movie* movie = cinema.getMovieAt(i);
                if (movie->getTitle() == movieTitle) {
                    movieFound = true;
                    std::cout << "Добавление сеанса для фильма " << movieTitle << ".\n";
                    std::tm sessionTime = readTime();
                    movie->addSessionToSchedule(sessionTime);
                    std::cout << "Сеанс добавлен.\n";
                    break;
                }
            }
            if (!movieFound) {
                std::cout << "Фильм не найден.\n";
            }
            break;
        }
        case 3: {
            std::string movieTitle;
            std::cout << "Введите название фильма для удаления: ";
            std::cin.ignore();
            std::getline(std::cin, movieTitle);

            if (cinema.removeMovie(movieTitle)) {
                std::cout << "Фильм удалён.\n";
            } else {
                std::cout << "Фильм не найден.\n";
            }
            break;
        }
        case 4: {
            std::string movieTitle;
            std::cout << "Введите название фильма для удаления сеанса: ";
            std::cin.ignore();
            std::getline(std::cin, movieTitle);

            bool movieFound = false;
            for (int i = 0; i < cinema.getMovieCount(); ++i) {
                Movie* movie = cinema.getMovieAt(i);
                if (movie->getTitle() == movieTitle) {
                    movieFound = true;
                    std::cout << "Удаление сеанса для фильма " << movieTitle << ".\n";
                    std::tm sessionTime = readTime();
                    if (movie->removeSessionFromSchedule(sessionTime)) {
                        std::cout << "Сеанс удалён.\n";
                    } else {
                        std::cout << "Сеанс не найден.\n";
                    }
                    break;
                }
            }
            if (!movieFound) {
                std::cout << "Фильм не найден.\n";
            }
            break;
        }
        case 5: {
            std::cout << "Список фильмов:\n";
            for (int i = 0; i < cinema.getMovieCount(); ++i) {
                Movie* movie = cinema.getMovieAt(i);
                std::cout << "Фильм: " << movie->getTitle() << ", продолжительность: " << movie->getDurationMinutes() << " минут\n";
            }
            break;
        }
        case 6: {
            std::string movieTitle;
            std::cout << "Введите название фильма для просмотра сеансов: ";
            std::cin.ignore();
            std::getline(std::cin, movieTitle);

            bool movieFound = false;
            for (int i = 0; i < cinema.getMovieCount(); ++i) {
                Movie* movie = cinema.getMovieAt(i);
                if (movie->getTitle() == movieTitle) {
                    movieFound = true;
                    std::cout << "Сеансы фильма " << movieTitle << ":\n";
                    std::vector<std::tm> sessionTimes = movie->getSessionTimes();
                    for (const std::tm& time : sessionTimes) {
                        std::cout << std::put_time(&time, "%Y-%m-%d %H:%M") << "\n";
                    }
                    break;
                }
            }
            if (!movieFound) {
                std::cout << "Фильм не найден.\n";
            }
            break;
        }
        case 0:
            std::cout << "Выход...\n";
            return 0;
        default:
            std::cout << "Неверный выбор, попробуйте снова.\n";
        }
    }
}