#include <iostream>
#include <vector>

// Observer interface
class Observer {
public:
    virtual void update(float temperature, float airPressure, float windDirection, float windSpeed) = 0;
    virtual ~Observer() {}
};

// Subject (Observable) class
class WMSystem {
public:
    // Constructors, destructor, etc.

    // Attach an observer
    void attachObserver(Observer* observer) {
        observers.push_back(observer);
    }

    // Detach an observer
    void detachObserver(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    // Setters for sensor readings
    void setTemperature(float temp) {
        temperature = temp;
        notifyObservers();
    }

    void setAirPressure(float pres) {
        airPressure = pres;
        notifyObservers();
    }

    void setWind(float dir, float speed) {
        windDirection = dir;
        windSpeed = speed;
        notifyObservers();
    }

private:
    // Private members
    float temperature;
    float airPressure;
    float windDirection;
    float windSpeed;
    std::vector<Observer*> observers;

    // Notify all observers when data changes
    void notifyObservers() {
        for (Observer* observer : observers) {
            observer->update(temperature, airPressure, windDirection, windSpeed);
        }
    }
};

// Concrete observer (DisplayStation)
class DisplayStation : public Observer {
public:
    // Constructors, destructor, etc.

    // Implementation of the update method
    void update(float temperature, float airPressure, float windDirection, float windSpeed) override {
        displayTemperature(temperature);
        displayPressure(airPressure);
        displayWind(windDirection, windSpeed);
    }

    // Display methods
    void displayTemperature(float temp) {
        std::cout << "Temperature: " << temp << std::endl;
    }

    void displayPressure(float pres) {
        std::cout << "Air Pressure: " << pres << std::endl;
    }

    void displayWind(float dir, float speed) {
        std::cout << "Wind: Direction " << dir << ", Speed " << speed << std::endl;
    }
};

// Concrete observer (LogStation)
class LogStation : public Observer {
public:
    // Constructors, destructor, etc.

    // Implementation of the update method
    void update(float temperature, float airPressure, float windDirection, float windSpeed) override {
        logData(temperature, airPressure, windDirection, windSpeed);
    }

    // Log data method
    void logData(float temperature, float airPressure, float windDirection, float windSpeed) {
        std::cout << "Logging Data - Temperature: " << temperature
                  << ", Air Pressure: " << airPressure
                  << ", Wind: Direction " << windDirection
                  << ", Speed " << windSpeed << std::endl;
    }
};

int main() {
    // Example usage
    WMSystem weatherSystem;

    DisplayStation display1;
    DisplayStation display2;
    LogStation logStation;

    weatherSystem.attachObserver(&display1);
    weatherSystem.attachObserver(&display2);
    weatherSystem.attachObserver(&logStation);

    weatherSystem.setTemperature(25.5);
    weatherSystem.setAirPressure(1013.2);
    weatherSystem.setWind(180.0, 15.0);

    // Detach an observer (e.g., display2)
    weatherSystem.detachObserver(&display2);

    weatherSystem.setTemperature(27.0);
    weatherSystem.setAirPressure(1012.5);
    weatherSystem.setWind(200.0, 20.0);

    return 0;
}
