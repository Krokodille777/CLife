#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

// Initial money and energy
double money = 100;
double energy = 100;
double health = 100;
double happiness = 50;
double experience = 50;
double luck = 10;
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(0, 100);
double probability = dis(gen);

// Inventory and items to buy
std::vector<std::string> inventory;
std::vector<std::string> itemsToBuy = {
"House", "Car", 
"Apple", "Banana", "Pineapple", "Tomato", "Orange", "Chocolate", "Milk", "Cheese", "Coffee", "EnergyDrink", "Meat", "Pizza",
 "Phone", "Laptop",
  "T-Shirt", "Pants", "Shoes", 
  "GiftBox", "FishingRod", "Worm", "HuntingRifle", "HuntingLicense",
  "CommonFish", "RareFish", "BigFish", "LegendaryFish", "Shark", 
  "Rabbit", "Fox", "Wolf", "Bear", "Hippo", "Deer", "Elephant", "Zebra", "Lion", "Tiger", "Raccoon", "Turtle", "Eagle",
  "BookC++", "ComicBook", "Mathematics", "Encyclopedia", "Poems", "Dictionary", "Novel", "CulinaryGuide", "Atlas"};
std::vector<int> prices = {
    2500, 1000, 
    10, 15, 30, 15,20, 25, 20, 35, 25, 30, 50, 65,
    1000, 2000, 
    100, 250, 50, 
    250, 75, 20, 350, 150,
    60, 120, 180, 240, 300, 
    70, 90, 110, 215, 230, 145, 320, 200, 220, 240, 85, 100, 115,
    75, 60, 45, 80, 40, 35, 45, 60, 30};  // Fixed to match itemsToBuy size
std::vector<double> earningSell = {
    1250, 500, 
    5, 7.5, 15, 7.5, 10, 12.5, 10, 17.5, 12.5, 15, 25, 32.5,
    500, 1000, 
    50, 125, 25, 
    125, 37.5, 10, 175, 75,
    30, 60, 90, 120, 150, 
    35, 45, 55, 115, 130, 75, 150, 100, 120, 140, 37.5, 50, 57.5,
    37.5, 30, 22.5, 40, 20, 17.5, 22.5, 30, 15};  // Fixed to match itemsToBuy size
std::vector<std::string> itemsToEat = {"Apple", "Banana",  "Pineapple", "Tomato", "Orange", "Chocolate", "Milk", "Cheese", "Meat", "EnergyDrink", "Coffee", "Pizza"};
std::vector<std::string> itemsToUse = {"GiftBox", "FishingRod", "Worm", "HuntingRifle", "HuntingLicense"};
std::vector<std::string> itemsToRead = {"BookC++", "ComicBook", "Mathematics", "Encyclopedia", "Poems", "Dictionary", "Novel", "CulinaryGuide", "Atlas"};
std::vector<std::string> itemsToWear = {"T-Shirt", "Pants", "Shoes"};
std::vector<std::string> Boosters = {"House", "Car"};
std::vector<std::string> Fish = {"CommonFish", "RareFish", "BigFish", "LegendaryFish", "Shark"};
std::vector<std::string> animalsToHunt = {"Rabbit", "Fox", "Wolf", "Bear", "Hippo", "Deer", "Elephant", "Zebra", "Lion", "Tiger", "Raccoon", "Turtle", "Eagle"};

// Function to return balance
double balance() {
    return money;
}

// Display available items in the shop
void read(std::string item) {
    bool found = false;
    for (const auto& readableItem : itemsToRead) {
        if (item == readableItem) {
            // Find item in inventory
            auto it = std::find(inventory.begin(), inventory.end(), item);
            if (it != inventory.end()) {
                inventory.erase(it);
                found = true;
                if (item == "BookC++") {
                    experience += 10;
                    happiness += 10;
                }
                else if (item == "ComicBook") {
                    experience += 5;
                    happiness += 45;
                }
                else if (item == "Mathematics") {
                    experience += 30;
                    happiness += 15;
                }
                else if (item == "Encyclopedia") {
                    experience += 30;
                    happiness += 25;
                }
                else if (item == "Poems") {
                    experience += 25;
                    happiness += 30;
                }
                else if (item == "Dictionary") {
                    experience += 35;
                    happiness += 20;
                }
                else if (item == "Novel") {
                    experience += 25;
                    happiness += 20;
                }
                else if (item == "CulinaryGuide") {
                    experience += 40;
                    happiness += 25;
                }
                else if (item == "Atlas") {
                    experience += 30;
                    happiness += 30;
                }
                std::cout << "You read " << item << ".\n";
                break;
            }
        }
    }
    if (!found) {
        std::cout << "You either don't have this item or it's not readable.\n";
    }
}

void shop() {
    std::cout << "Here is what you can buy:\n";
    for (size_t i = 0; i < itemsToBuy.size(); ++i) {
        std::cout << itemsToBuy[i] << " : $" << prices[i];
        // Show item category
        if (std::find(itemsToEat.begin(), itemsToEat.end(), itemsToBuy[i]) != itemsToEat.end())
            std::cout << " (F)";
        else if (std::find(itemsToRead.begin(), itemsToRead.end(), itemsToBuy[i]) != itemsToRead.end())
            std::cout << " (B)";
        else if (std::find(itemsToUse.begin(), itemsToUse.end(), itemsToBuy[i]) != itemsToUse.end())
            std::cout << " (U)";
        else if (std::find(itemsToWear.begin(), itemsToWear.end(), itemsToBuy[i]) != itemsToWear.end())
            std::cout << " (C)";
        else if (std::find(Boosters.begin(), Boosters.end(), itemsToBuy[i]) != Boosters.end())
            std::cout << " (BS)";
        else if (std::find(Fish.begin(), Fish.end(), itemsToBuy[i]) != Fish.end())
            std::cout << " (SF)";
        else if (std::find(animalsToHunt.begin(), animalsToHunt.end(), itemsToBuy[i]) != animalsToHunt.end())
            std::cout << " (AH)";
        std::cout << "\n";
    }
}

void GiftBox(std::string item){
        if (item == "GiftBox") {
            std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution<std::size_t> dis(0, itemsToBuy.size() - 1);
            std::size_t randomIndex = dis(gen);
            std::string randomItem = itemsToBuy[randomIndex];
            inventory.erase(std::remove(inventory.begin(), inventory.end(), item), inventory.end());
            inventory.push_back(randomItem);
            std::cout << "You received a " << randomItem << "!\n";
            if (std::find(itemsToEat.begin(), itemsToEat.end(), item) != itemsToEat.end())
                std::cout << " (F)";
            else if (std::find(itemsToRead.begin(), itemsToRead.end(), item) != itemsToRead.end())
                std::cout << " (B)";
            else if (std::find(itemsToUse.begin(), itemsToUse.end(), item) != itemsToUse.end())
                std::cout << " (U)";
            else if (std::find(itemsToWear.begin(), itemsToWear.end(), item) != itemsToWear.end())
                std::cout << " (C)";
            else if (std::find(Boosters.begin(), Boosters.end(), item) != Boosters.end())
                std::cout << " (BS)";
            else if (std::find(Fish.begin(), Fish.end(), item) != Fish.end())
                std::cout << " (SF)";
            else if (std::find(animalsToHunt.begin(), animalsToHunt.end(), item) != animalsToHunt.end())
                std::cout << " (AH)";
            std::cout << "\n";
            return;
        }
    
    }
void fish(std::string item) {
    if (item == "FishingRod") {
        // Check if a worm is in the inventory
        if (std::find(inventory.begin(), inventory.end(), "Worm") != inventory.end()) {
            // Seed and select random fish
            std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution<std::size_t> dis(0, Fish.size() - 1);
            std::size_t randomIndex = dis(gen);
            std::string randomItem = Fish[randomIndex];

            // Remove one "Worm" and add the fish
            inventory.erase(std::remove(inventory.begin(), inventory.end(), "Worm"), inventory.end());
            if (probability> 50){
            inventory.push_back(randomItem);
            std::cout << "You caught a " << randomItem << "!\n";
            }
            else{
                std::cout << "Unfortunately, you didn't catch anything.\n";
            }
        } else {
            std::cout << "You need a worm to fish.\n";
        }
    } else {
        std::cout << "You don't have a fishing rod.\n";
    }
}
 void hunt(std::string item) {
    if (item == "HuntingRifle") {
        if (std::find(inventory.begin(), inventory.end(), "HuntingLicense") != inventory.end()) {
            std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution<std::size_t> dis(0, animalsToHunt.size() - 1);
            std::size_t randomIndex = dis(gen);
            std::string randomItem = animalsToHunt[randomIndex];
            if (probability> 50){
            inventory.push_back(randomItem);
            std::cout << "You caught a " << randomItem << "!\n";
            }
            else{
                std::cout << "Unfortunately, you didn't catch anything.\n";
            }
        }
        else {
            std::cout << "You need a hunting license to hunt.\n";
        }
    } else {
        std::cout << "You don't have a hunting rifle.\n";
    }
 }
 
void buy(std::string item) {
    for (size_t i = 0; i < itemsToBuy.size(); ++i) {
        if (item == itemsToBuy[i]) {
            if (money >= prices[i]) {
                money -= prices[i];
                inventory.push_back(item);
                std::cout << "You bought " << item << ".\n";
                return;
            }
            std::cout << "You cannot afford this item.\n";
            return;
        }
    }
    std::cout << "Item not found in shop.\n";
}

void sell(std::string item) {
    auto it = std::find(inventory.begin(), inventory.end(), item);
    if (it != inventory.end()) {
        auto itemIndex = std::find(itemsToBuy.begin(), itemsToBuy.end(), item) - itemsToBuy.begin();
        if (itemIndex < earningSell.size()) {
            money += earningSell[itemIndex];
            inventory.erase(it);
            std::cout << "You sold " << item << " for $" << earningSell[itemIndex] << ".\n";
        }
    } else {
        std::cout << "You don't have this item in your inventory.\n";
    }
}

void eat(std::string item) {
    auto it = std::find(inventory.begin(), inventory.end(), item);
    if (it != inventory.end()) {
        bool isEdible = false;
        for (const auto& foodItem : itemsToEat) {
            if (item == foodItem) {
                isEdible = true;
                inventory.erase(it);
                if (item == "Apple") {
                    health += 5;
                    energy += 5;
                } else if (item == "Banana") {
                    health += 10;
                    energy += 5;
                } else if (item == "Pineapple") {
                    health += 10;
                    energy += 10;
                } else if (item == "Orange") {
                    health += 15;
                    energy += 10;
                } else if (item == "Chocolate") {
                    health += 20;
                    energy += 30;
                } else if (item == "Milk") {
                    health += 30;
                    energy += 20;
                    happiness += 5;
                } else if (item == "Cheese") {
                    health += 35;
                    energy += 25;
                } else if (item == "Meat") {
                    health += 50;
                    energy += 40;
                } else if (item == "EnergyDrink") {
                    health += 10;
                    energy += 60;
                } else if (item == "Coffee") {
                    health += 25;
                    energy += 45;
                }
                else if (item =="Pizza"){
                    health += 35;
                    energy += 30;
                    happiness += 10;
                }
                std::cout << "You ate " << item << ".\n";
                break;
            }
        }
        if (!isEdible) {
            std::cout << "You cannot eat " << item << ".\n";
        }
    } else {
        std::cout << "You don't have " << item << " in your inventory.\n";
    }
}

double energyCount() {
    if (energy > 100) energy = 100;
    if (energy >= 75 && energy <= 100) {
        std::cout << "You are full of energy. It's time for some action!\n";
    } else if (energy < 75 && energy > 50) {
        std::cout << "Your energy is a bit low. To restore energy, go to sleep, eat food or do sports.\n";
    } else if (energy <= 50 && energy > 25) {
        std::cout << "Your energy is low. To restore energy, go to sleep, eat food or do sports.\n";
    } else if (energy <= 25 && energy > 0) {
        std::cout << "Energy level is critically low. Please take some measures immediately.\n";
    } else if (energy <= 0) {
        energy = 0;
        std::cout << "You died of exhaustion\n";
        return 0;
    }
    return energy;
}

// Similar fixes for healthCount(), happinessCount(), and expCount()


void CheckMyInventory() {
    if (inventory.empty()) {
        std::cout << "Your inventory is empty.\n";
    } else {
        std::cout << "Your inventory:\n";
        for (const auto& item : inventory) {
            std::cout << "- " << item;
            if (std::find(itemsToEat.begin(), itemsToEat.end(), item) != itemsToEat.end())
                std::cout << " (F)";
            else if (std::find(itemsToRead.begin(), itemsToRead.end(), item) != itemsToRead.end())
                std::cout << " (B)";
            else if (std::find(itemsToUse.begin(), itemsToUse.end(), item) != itemsToUse.end())
                std::cout << " (U)";
            else if (std::find(itemsToWear.begin(), itemsToWear.end(), item) != itemsToWear.end())
                std::cout << " (C)";
            else if (std::find(Boosters.begin(), Boosters.end(), item) != Boosters.end())
                std::cout << " (BS)";
            else if (std::find(Fish.begin(), Fish.end(), item) != Fish.end())
                std::cout << " (SF)";
            else if (std::find(animalsToHunt.begin(), animalsToHunt.end(), item) != animalsToHunt.end())
                std::cout << " (AH)";
            std::cout << "\n";
        }
    }
}
double healthCount() {
    if (health> 100) health = 100;
    if (health >= 75 && health <= 100) {
        std::cout<<"You are healthy. It's time for some action!\n";
    } else if (health < 75 && health > 50) {
        std::cout<<"Your health is a bit low. To restrore health, eat food or take some medicine.\n";
    } else if (health <= 50 && health > 25) {
        std::cout<<"Oh dear! Be careful, my friend! Your health is low.\n";
    } else if (health <= 25 && health > 0) {
        std::cout<<"Health level is critically low. Please take some measures immediately.\n";
    } else if (health == 0) {
        std::cout<<"You died\n";
        return 0;
    
    }
    return health;
}
double happinessCount() {
    if (happiness > 100) happiness = 100;
    if (happiness >= 75 && happiness <= 100) {
        std::cout<<"You are happy. Things will go very well!\n";
    } else if (happiness < 75 && happiness > 50) {
        std::cout<<"You look a bit sad. If you want to be happy, read books, watch movies, etc.\n";
    } else if (happiness <= 50 && happiness > 25) {
        std::cout<<"Gosh! You are really sad! Please take some measures, otherwise things will go badly.\n";
    } else if (happiness <= 25 && happiness > 0) {
        std::cout<<"You are burst into tears. Find something to calm down. Until it's too late.\n";
    } else if (happiness == 0) {
        std::cout<<"You died of sadness\n";
        return 0;
    
    }
    return happiness;
}

double expCount() {
    if (experience >= 75 && experience <= 100) {
        std::cout<<"You are experienced. It's time for some action!\n";
    } else if (experience < 75 && experience > 50) {
        std::cout<<"Your experience is a bit low. To restrore experience, eat food or take some medicine.\n";
    } else if (experience <= 50 && experience > 25) {
        std::cout<<"Oh dear! Be careful, my friend! Your experience is low.\n";
    } else if (experience <= 25 && experience > 0) {
        std::cout<<"Experience level is critically low. Please take some measures immediately.\n";
    } else if (experience == 0) {
        std::cout<<"You died of boredom\n";
        return 0;
    
    }
    return experience;
}
// Simulate work and return salary
double work() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100, 500);
    double salary = dis(gen);
    if (experience >= 100 && happiness <= 100 && happiness >= 85){
        std::uniform_int_distribution<> dis2(250, 750);
        salary += dis2(gen);
    }
    money += salary;
    energy -= 75;
    
    return salary;
}

// Display inventory


// Restore energy after sleeping
double sleep() {
    std::cout << "You have slept and restored your energy.\n";
    energy = 100;
    return energy;
}
void Rules(){
    std::cout<<"Commands:\n";
    std::cout << "balance - To check your balance. Your balance is all the money you have. You can spend it, you can earn it. It is completely up to you. You can earn money in different ways: doing work, selling items, mystery boxes, etc.\n";
    std::cout << "buy - To buy an item. This game offers you some items you can buy to use. if you have enough money, you can buy. To see what you can buy, type shop.\n";
    std::cout << "eat - To eat food. Food can increase your health and energy. But be careful it can become spoiled\n";
    std::cout << "experience - To check your experience. Experience level depends on your knowledge. Work and reading books can increase it.\n";
    std::cout << "energy - To check your energy. Energy level is one of the most important things. You should always regulate it. Try to keep it at high levels. Sleeping, eating or doing sports can restore your energy.\n";
    std::cout << "read - To read books. Reading books can increase your experience. This game has some books you can read. Each book has a different effect on your experience. It also increases your happiness.\n";
    std::cout << "sell - To sell an item. You have an item in your inventory which is completely unnecessary. Why should you move it to trash if you can make money from it? That's why you can easily sell it. The money you earn from selling is a half price of the item.\n";
    std::cout << "happiness - To check your happiness. Your happiness depends on everything you do. If you are happy everything is going fine. Work goes more succesfully, you earn more money. But it can easily be spoilt. If it happens try to take some measures to become happy.\n";
    std::cout << "health - To check your health. Your health depends on your lifestyle. If you are healthy, your happiness will be on high level. Try to keep your body healthy. There are many things that can affect your health.\n";
    std::cout << "inventory - To check your inventory. Here you can find everything you got. you can easily interact with it. you can easily add some new items or vice versa.\n";
    std::cout << "shop - your shop list of items.\n";
    std::cout << "quit - quits the program\n";
    std::cout << "sleep - to sleep. It will restore your energy\n";
    std::cout << "work - to work. Work is one of the most important commands in this game. It is your main source to gain money. Good work depends on your experince, happiness and energy. If you work once you can't work again because you are so tired. It is really resourceful command. So please be careful. If you want to work again, you can sleep.\n";
    std::cout<< "use - to use an item. You can use it. This games provides you a huge variety of special usable items. Each of them has its own effect and it can be used in different ways.\n";
    std::cout<<"About Items:\n";
    std::cout<<"Items are the most popular thing you should interact with. This game provides you a big variety of different items. \n You can divide them into some categories: \n 1)Food\n 2)Usable items\n 3)Books\n 4)Boosters\n 5)Clothes\n 6)Tickets\n 7)Pets\n 8) Other";
    std::cout<<"Endings:\n";
    std::cout<<"1) if energy = 0 then you died of exhaustion\n";
    std::cout<<"2) if experience = 0 then you died of boredom\n";
    std::cout<<"3) if health = 0 then you died of disease\n";
    std::cout<<"4) if happiness = 0 then you died of sadness\n";



}
void ListOfCommands(){
    std::cout<<"Commands:\n";
    std::cout<<"balance\n";
    std::cout<<"buy\n";
    std::cout<<"eat\n";
    std::cout<<"experience\n";
    std::cout<<"energy\n";
    std::cout<<"read\n";
    std::cout<<"sell\n";
    std::cout<<"happiness\n";
    std::cout<<"health\n";
    std::cout<<"inventory\n";
    std::cout<<"shop\n";
    std::cout<<"quit\n";
    std::cout<<"sleep\n";
    std::cout<<"work\n";
    std::cout<<"use\n";
}

// Quit the program
int quit() {
    std::cout << "Bye!\n";
    return 0;
}

int main() {
    std::string command;

    while (command != "quit") {
        std::cout << "Enter command (use help or all_commands): ";
        std::cin >> command;

        if (command == "balance") {
            std::cout << "You have $" << balance() << ".\n";
        } 
        else if (command == "inventory") {
            
            CheckMyInventory();
        } 
        else if (command == "work") {
            if (energy >= 75) {
                std::cout << "Work is finished by 25%.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Simulate work progress
                std::cout << "Work is finished by 50%.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                std::cout << "Work is finished by 75%.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                std::cout << "Work is finished by 100%.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                std::cout << "You earned $" << work() << ".\n";
                std::cout << "Now you have $" << balance() << ".\n";
                std::cout << "You are tired and can't work anymore today.\n";
            } else {
                std::cout << "You are too tired to work. Please sleep first.\n";
            }
        } 
        else if (command == "all_commands") {
            ListOfCommands();
        }
        else if (command == "help") {
            Rules();
        }
        else if (command == "shop") {
            shop();
        } 
        else if (command == "happiness") {
            std::cout << happinessCount() << " happiness.\n";
        } 
        else if (command == "experience") {
            std::cout << expCount() << " experience.\n";
        }
        else if (command == "eat") {
            std::string item;
            std::cout<<"What do you want to eat? \n";
            std::cin >> item;
            eat(item);
        } 
        else if (command == "health") {
            std::cout << healthCount() << " health.\n";
        }
        else if (command == "sleep") {
            sleep();
        } 
        else if (command == "buy") {
            std::string item;
            std::cout<<"What do you want to buy? \n";
            std::cin >> item;
            buy(item);
        } 
        else if (command == "energy") {
            std::cout << "You have " << energyCount() << " energy.\n";
        }
        else if (command == "read") {
            std::string item;
            std::cout<<"What do you want to read? \n";
            std::cin >> item;
            read(item);
        }
        else if (command == "sell") {
            std::string item;
            std::cout<<"What do you want to sell? \n";
            std::cin >> item;
            sell(item);
        }
        else if (command == "use") {
            std::string item;
            std::cout<<"What do you want to use? \n";
            std::cin >> item;
            if (item == "GiftBox") {
                GiftBox(item);
            }
            else if (item == "FishingRod") {
                fish(item);
            
        }
        }
        else if (command == "quit") {
            quit();
        } 
        else {
            std::cout << "Unknown command.\n";
        }
    }

    return 0;
}
