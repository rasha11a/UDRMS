#ifndef RESTAURANT_H
#define RESTAURANT_H
#include <string>

//Meal structure
// Represents one full meal with all its components
struct Meal
{
    std::string mainDish;
    std::string salad;
    std::string fruit;
    std::string juice;
    std::string soup; // optional, can be ""

    // constructor so you can create a meal in one line
    Meal(const std::string &main = "Not set", const std::string &salad = "Not set", const std::string &fruit = "Not set", const std::string &juice = "Not set", const std::string &soup = "")
        : mainDish(main), salad(salad),
        fruit(fruit), juice(juice), soup(soup) {}

    bool isSet() const { return mainDish != "Not set"; }
};

//Restaurant class
class Restaurant
{
private:
    std::string name;

    Meal breakfast; // full breakfast meal
    Meal lunch;     // full lunch meal
    Meal dinner;    // full dinner meal

    int breakfastServed;
    int lunchServed;
    int dinnerServed;

public:
    // Constructors
    Restaurant();
    Restaurant(const std::string &name);

    // Getters
    std::string getName() const;
    Meal getBreakfast() const;
    Meal getLunch() const;
    Meal getDinner() const;

    int getBreakfastServed() const;
    int getLunchServed() const;
    int getDinnerServed() const;
    int getTotalMealsToday() const;

    // Setters
    void setName(const std::string &name);

    void setBreakfast(const std::string &main, const std::string &salad, const std::string &fruit, const std::string &juice, const std::string &soup = "");

    void setLunch(const std::string &main, const std::string &salad, const std::string &fruit, const std::string &juice, const std::string &soup = "");

    void setDinner(const std::string &main, const std::string &salad, const std::string &fruit, const std::string &juice, const std::string &soup = "");

    // Operations
    void serveBreakfast();
    void serveLunch();
    void serveDinner();
    void resetDailyCount();

    // Helpers
    bool hasMenu() const;

    // Display
    void displayMeal(const std::string &mealName, const Meal &meal) const;
    void displayMenu() const;
    void displayStats() const;
};

#endif