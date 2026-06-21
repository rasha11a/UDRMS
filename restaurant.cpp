#include "restaurant.h"
#include <iostream>

// Constructors

Restaurant::Restaurant()
    : name("Unknown"), breakfast(), lunch(), dinner(), breakfastServed(0), lunchServed(0), dinnerServed(0) {}

Restaurant::Restaurant(const std::string &name)
    : name(name), breakfast(), lunch(), dinner(), breakfastServed(0), lunchServed(0), dinnerServed(0) {}

// Getters

std::string Restaurant::getName() const { return name; }
Meal Restaurant::getBreakfast() const { return breakfast; }
Meal Restaurant::getLunch() const { return lunch; }
Meal Restaurant::getDinner() const { return dinner; }

int Restaurant::getBreakfastServed() const { return breakfastServed; }
int Restaurant::getLunchServed() const { return lunchServed; }
int Restaurant::getDinnerServed() const { return dinnerServed; }
int Restaurant::getTotalMealsToday() const
{
    return breakfastServed + lunchServed + dinnerServed;
}

// Setters

void Restaurant::setName(const std::string &n) { name = n; }

void Restaurant::setBreakfast(const std::string &main, const std::string &salad, const std::string &fruit, const std::string &juice, const std::string &soup)
{
    breakfast = Meal(main, salad, fruit, juice, soup);
}

void Restaurant::setLunch(const std::string &main, const std::string &salad, const std::string &fruit, const std::string &juice, const std::string &soup)
{
    lunch = Meal(main, salad, fruit, juice, soup);
}

void Restaurant::setDinner(const std::string &main, const std::string &salad, const std::string &fruit, const std::string &juice, const std::string &soup)
{
    dinner = Meal(main, salad, fruit, juice, soup);
}

// Operations
void Restaurant::serveBreakfast() { breakfastServed++; }
void Restaurant::serveLunch() { lunchServed++; }
void Restaurant::serveDinner() { dinnerServed++; }

void Restaurant::resetDailyCount()
{
    breakfastServed = 0;
    lunchServed = 0;
    dinnerServed = 0;
}

// Helpers

bool Restaurant::hasMenu() const
{
    return breakfast.isSet() && lunch.isSet() && dinner.isSet();
}

// Display
void Restaurant::displayMeal(const std::string &mealName, const Meal &meal) const
{
    std::cout << "  [ " << mealName << " ]\n";
    std::cout << "  Main dish : " << meal.mainDish << "\n";
    std::cout << "  Salad     : " << meal.salad << "\n";
    std::cout << "  Fruit     : " << meal.fruit << "\n";
    std::cout << "  Juice     : " << meal.juice << "\n";
    if (!meal.soup.empty())
        std::cout << "  Soup      : " << meal.soup << "\n";
}

void Restaurant::displayMenu() const
{
    std::cout << "=============================\n";
    std::cout << "  Restaurant : " << name << "\n";
    std::cout << "=============================\n";
    displayMeal("BREAKFAST", breakfast);
    std::cout << "-----------------------------\n";
    displayMeal("LUNCH", lunch);
    std::cout << "-----------------------------\n";
    displayMeal("DINNER", dinner);
    std::cout << "=============================\n";
}

void Restaurant::displayStats() const
{
    std::cout << "=============================\n";
    std::cout << "  Meals served today\n";
    std::cout << "  Breakfast : " << breakfastServed << "\n";
    std::cout << "  Lunch     : " << lunchServed << "\n";
    std::cout << "  Dinner    : " << dinnerServed << "\n";
    std::cout << "  Total     : " << getTotalMealsToday() << "\n";
    std::cout << "=============================\n";
}