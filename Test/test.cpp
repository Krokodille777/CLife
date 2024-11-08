#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <atomic>
#include <mutex>
// JSON library
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

// Initial money and energy
double money = 100;
double energy = 100;
double health = 100;
double happiness = 50;
double experience = 50;
double luck = 1;
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(0, 100);
double probability = dis(gen);
std::string item;
std::string spoiltItem = "Spoilt " + item;
std::atomic<bool> gameRunning(true);  // Controls the spoilage thread
std::mutex inventoryMutex; 

// Configuration
const std::string saveFile = "save.json";

// Inventory and items to buy
std::vector<std::string> inventory;
std::vector<std::string> itemsToBuy = {"House", "Car", "Apple", "Banana", "Pineapple", "Tomato", "Orange", "Chocolate", "Milk", "Cheese", "Coffee", "EnergyDrink", "Meat", "Pizza","Phone", "Laptop", "Crown", "Trophy", "Ring", "T-Shirt", "Pants", "Shoes", "GiftBox", "MysteryBox", "FishingRod", "Worm", "HuntingRifle", "HuntingLicense", "Drill","CommonFish", "RareFish", "BigFish", "LegendaryFish", "Shark", "Rabbit", "Fox", "Wolf", "Bear", "Hippo", "Deer", "Elephant", "Zebra", "Lion", "Tiger", "Raccoon", "Turtle", "Eagle", "Iron", "Copper", "Gold", "Silver", "Diamond", "Mercury", "Titanium", "Aluminum", "Sand", "Cobblestone", "Clay", "Coal", "BookC++", "ComicBook", "Mathematics", "Encyclopedia", "Poems", "Dictionary", "Novel", "CulinaryGuide", "Atlas", "Spoilt_Apple", "Spoilt_Banana", "Spoilt_Pineapple", "Spoilt_Tomato", "Spoilt_Orange", "Spoilt_Chocolate", "Spoilt_Milk", "Spoilt_Cheese", "Spoilt_Meat", "Spoilt_EnergyDrink", "Spoilt_Coffee", "Spoilt_Pizza"};
std::vector<double> prices = {2500, 1000, 10, 15, 30, 15, 20, 25, 20, 35, 25, 30, 50, 65, 1000, 2000, 5000, 3000, 1500, 100, 250, 50,  250, 150, 75, 20, 350, 150, 450, 60, 120, 180, 240, 300,  70, 90, 110, 215, 230, 145, 320, 200, 220, 240, 85, 100, 115, 50, 35, 215, 7500, 180, 165, 240, 100, 25, 60, 30, 50, 75, 60, 45, 80, 40, 35, 45, 60, 30, 5, 7.5, 15, 7.5, 10, 12.5, 10, 17.5, 12.5, 15, 25, 32.5};  // Fixed to match itemsToBuy size
std::vector<double> earningSell = {1250, 500, 5, 7.5, 15, 7.5, 10, 12.5, 10, 17.5, 12.5, 15, 25, 32.5, 500, 1000, 2500, 1500, 750, 50, 125, 25,  125, 75, 37.5, 10, 175, 75, 30, 60, 90, 120, 150, 35, 45, 55, 115, 130, 75, 150, 100, 120, 140, 37.5, 50, 57.5, 25, 17.5, 107.5, 3750, 90, 82.5, 120, 100, 12.5, 30, 15, 25,  37.5, 30, 22.5, 40, 20, 17.5, 22.5, 30, 15, 2.5, 3.75, 7.5, 3.75, 5, 6.25, 5, 8.75, 6.25, 7.5, 12.5, 16.25};  // Fixed to match itemsToBuy size
std::vector<std::string> itemsToEat = {"Apple", "Banana",  "Pineapple", "Tomato", "Orange", "Chocolate", "Milk", "Cheese", "Meat", "EnergyDrink", "Coffee", "Pizza"};
std::vector<std::string> itemsToUse = {"GiftBox", "FishingRod", "Worm", "HuntingRifle", "HuntingLicense", "Drill", "MysteryBox"};
std::vector<std::string> itemsToRead = {"BookC++", "ComicBook", "Mathematics", "Encyclopedia", "Poems", "Dictionary", "Novel", "CulinaryGuide", "Atlas"};
std::vector<std::string> itemsToWear = {"T-Shirt", "Pants", "Shoes"};
std::vector<std::string> Boosters = {"House", "Car"};
std::vector<std::string> Fish = {"CommonFish", "RareFish", "BigFish", "LegendaryFish", "Shark"};
std::vector<std::string> animalsToHunt = {"Rabbit", "Fox", "Wolf", "Bear", "Hippo", "Deer", "Elephant", "Zebra", "Lion", "Tiger", "Raccoon", "Turtle", "Eagle"};
std::vector<std::string> spoiltItems = {"Spoilt_Apple", "Spoilt_Banana", "Spoilt_Pineapple", "Spoilt_Tomato", "Spoilt_Orange", "Spoilt_Chocolate", "Spoilt_Milk", "Spoilt_Cheese", "Spoilt_Meat", "Spoilt_EnergyDrink", "Spoilt_Coffee", "Spoilt_Pizza"};
std::vector<std::string>MiningStones = {"Iron", "Copper", "Gold", "Silver", "Mercury", "Titanium", "Aluminium", "Sand", "CobbleStone", "Clay", "Coal"};
std::vector<std::string> Treasure = {"Crown", "Trophy", "Ring", "Diamond"};

// Save and load progress

int saveProgress() {
    json save;
    save["money"] = money;
    save["energy"] = energy;
    save["health"] = health;
    save["happiness"] = happiness;
    save["experience"] = experience;
    save["inventory"] = inventory;
    save["luck"] = luck;
    save["probability"] = probability;
    std::ofstream file(saveFile);
    file << save.dump(4);
    return 0;
}

int loadProgress() {
    // This function will load the progress from the save file
    std::ifstream
    file(saveFile);
    if (!file.is_open()) {
        std::cout << "No save file found.\n";
        return 1;
    }
    json save;
    file >> save;
    money = save["money"];
    energy = save["energy"];
    health = save["health"];
    happiness = save["happiness"];
    experience = save["experience"];
    inventory = save["inventory"];
    luck = save["luck"];
    probability = save["probability"];
    return 0;
}


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
                // save progress
                saveProgress();
                break;
            }
        }
    }
    if (!found) {
        std::cout << "You either don't have this item or it's not readable.\n";
    }
}

void shop() {
    std::cout << "\n=== AVAILABLE ITEMS ===\n\n";
    
    // Get maximum item name length for alignment
    size_t maxNameLength = 0;
    for (const auto& item : itemsToBuy) {
        maxNameLength = std::max(maxNameLength, item.length());
    }
    
    // Print header with categories explanation
    std::cout << "Categories: (F)ood, (B)ooks, (U)sable items, (C)lothes, \n"
              << "           (BS)Boosters, (SF)Store Fish, (AH)Animals to Hunt, (Fs)Spoilt Items\n\n";
    
    // Print items in columns with proper alignment
    for (size_t i = 0; i < itemsToBuy.size(); ++i) {
        // Item name with padding
        std::cout << std::left << std::setw(maxNameLength + 2) << itemsToBuy[i];
        
        // Price with alignment
        std::cout << "$ " << std::right << std::setw(5) << prices[i];
        
        // Category tag
        if (std::find(itemsToEat.begin(), itemsToEat.end(), itemsToBuy[i]) != itemsToEat.end())
            std::cout << "  (F)";
        else if (std::find(itemsToRead.begin(), itemsToRead.end(), itemsToBuy[i]) != itemsToRead.end())
            std::cout << "  (B)";
        else if (std::find(itemsToUse.begin(), itemsToUse.end(), itemsToBuy[i]) != itemsToUse.end())
            std::cout << "  (U)";
        else if (std::find(itemsToWear.begin(), itemsToWear.end(), itemsToBuy[i]) != itemsToWear.end())
            std::cout << "  (C)";
        else if (std::find(Boosters.begin(), Boosters.end(), itemsToBuy[i]) != Boosters.end())
            std::cout << "  (BS)";
        else if (std::find(Fish.begin(), Fish.end(), itemsToBuy[i]) != Fish.end())
            std::cout << "  (SF)";
        else if (std::find(animalsToHunt.begin(), animalsToHunt.end(), itemsToBuy[i]) != animalsToHunt.end())
            std::cout << "  (AH)";
        else if (std::find(spoiltItems.begin(), spoiltItems.end(), itemsToBuy[i]) != spoiltItems.end())
            std::cout << "  (Fs)";
        else if (std::find(MiningStones.begin(), MiningStones.end(), itemsToBuy[i]) != MiningStones.end())
            std::cout << "  (Ms)";
        else if (std::find(Treasure.begin(), Treasure.end(), itemsToBuy[i]) != Treasure.end())
            std::cout << "  (T)";
        else
            std::cout << "  (O)"; // Other
            
        std::cout << '\n';
    }
    std::cout << "\n===================\n";
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
            else if(std::find(spoiltItems.begin(), spoiltItems.end(), item) != spoiltItems.end())
                std::cout << " (Fs)"; // Food spoilt
            else if(std::find(MiningStones.begin(), MiningStones.end(), item) != MiningStones.end())
                std::cout << " (Ms)"; // Mining Stone
            std::cout << "\n";
            saveProgress();
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
            saveProgress();
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
            saveProgress();
        }
        else {
            std::cout << "You need a hunting license to hunt.\n";
        }
    } else {
        std::cout << "You don't have a hunting rifle.\n";
    }
 }
 
void mine(std::string item){
    if (item == "Drill"){
        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<std::size_t> dis(0, MiningStones.size() - 1);
        std::size_t randomIndex = dis(gen);
        std::string randomItem = MiningStones[randomIndex];
        if (energy > 20){
            if (probability> 50){
        inventory.push_back(randomItem);
        std::cout << "You mined a " << randomItem << "!\n";
        }
        else{
            std::cout << "Unfortunately, you didn't mine anything.\n";
        }
        energy -= 10;
        }
        else if (energy < 20){
            std::cout << "You don't have enough energy to mine anything.\n";
        }
        
        saveProgress();
    }
} 

void MysteryBox(std::string item){
    if (item == "MysteryBox"){
        inventory.erase(std::remove(inventory.begin(), inventory.end(), item), inventory.end());
        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<std::size_t> dis(1, 14);
        int randomNimber = dis(gen);
        if (randomNimber == 1){
            std::cout<<"You have found a chest with money! Lucky you!\n";
            std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution<std::size_t> dis(25, 225);
            int randomNimber = dis(gen);
            money += randomNimber;
            luck += 0.2;
            saveProgress();
        }
        else if (randomNimber == 2){
            std::cout<<"You found a magic magnifying glass that doubled your money!\n";
            money *= 2;
            luck += 0.5;
            saveProgress();
        }
        else if (randomNimber == 3){
            std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution<std::size_t> dis(1, Treasure.size() - 1);
            std::size_t randomIndex = dis(gen);
            std::string randomItem = Treasure[randomIndex];
            inventory.push_back(randomItem);
            std::cout << "You found a " << randomItem << "!\n";
            luck += 0.3;
            saveProgress();
        }
        else if (randomNimber == 4){
            std::cout<<"You found another mystery box in this box!\n";
            inventory.push_back("MysteryBox");
            luck += 0.1;
            saveProgress();
        }
        else if (randomNimber == 5){
            std::cout<<"There was nothing in this box! Well, at least you didn't lose anything.\n";
        }
        else if (randomNimber == 6){
            std::cout<<"There was a water bomb that made you lose some money!\n";
            std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution<std::size_t> dis(1, 5);
            int randomNimber = dis(gen);
            money -= randomNimber;
            luck -= 0.2;
            saveProgress();
        }
        else if (randomNimber == 7){
            std::cout<<"There was a thief who stole half of your money!\n";
            money /= 2;
            luck -= 0.7;
            saveProgress();
        }
        else if (randomNimber == 8){
            std::cout<<"There was a fire ball that destroyed some of your items!\n";
            std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution<std::size_t> dis(1, 5);
            int randomNimber = dis(gen);
            inventory.erase(inventory.begin() + randomNimber);
            luck -= 0.3;
            saveProgress();
        }
        else if (randomNimber == 9){
            std::cout<<"There was a potion that made you lose some your stats!\n";
            std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution<std::size_t> dis(20, 30);
            int healthLoss = dis(gen);
            std::uniform_int_distribution<std::size_t> dis2(20, 50);
            int energyLoss = dis2(gen);
            std::uniform_int_distribution<std::size_t> dis3(15, 50);
            int happinessLoss = dis3(gen);
            health -= healthLoss;
            energy -= energyLoss;
            happiness -= happinessLoss;
            luck -= 0.5;
            saveProgress();
        }
        else if (randomNimber == 10){
            std::cout<<"There was a healing potion that healed you!\n";
            std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution<std::size_t> dis(20, 30);
            int healthGain = dis(gen);
            health += healthGain;
            luck += 0.3;
            saveProgress();
        }
        else if (randomNimber == 11){
            std::cout<<"There was an elixir that increases your intelligence!\n";
            std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution<std::size_t> dis(5, 45);
            int randomNimber = dis(gen);
            experience += randomNimber;
            luck += 0.3;
            saveProgress();
        }
        else if (randomNimber == 12){
            std::cout<<"There was a magic fruit that restored your energy!\n";
            std::mt19937 gen(std::random_device{}());
            std::uniform_int_distribution<std::size_t> dis(5, 45);
            int randomNimber = dis(gen);
            energy += randomNimber;
            luck += 0.3;
            saveProgress();
        }
    }
    else{
        std::cout<<"You don't have a mystery box.\n";
    }
}
void buy(std::string item) {
    
    for (size_t i = 0; i < itemsToBuy.size(); ++i) {
        if (item == itemsToBuy[i]) {
            if (money >= prices[i]) {
                money -= prices[i];
                inventory.push_back(item);
                std::cout << "You bought " << item << ".\n";
                saveProgress();
                return;
            }
            std::cout << "You cannot afford this item.\n";
            return;
        }
    }
    std::cout << "Item not found in shop.\n";
}
void spoil(std::string item) {
    std::lock_guard<std::mutex> lock(inventoryMutex);  // Lock the inventory
    
    auto it = std::find(inventory.begin(), inventory.end(), item);
    
    if (it == inventory.end()) {
        return;
    }
    
    bool isFood = std::find(itemsToEat.begin(), itemsToEat.end(), item) != itemsToEat.end();
    if (!isFood) {
        return;
    }
    
    std::string spoiltItem = "Spoilt_" + item;
    
    if (std::find(spoiltItems.begin(), spoiltItems.end(), spoiltItem) != spoiltItems.end()) {
        inventory.erase(it);
        inventory.push_back(spoiltItem);
        std::cout << "\nYour " << item << " has spoiled.\n";
        saveProgress();
    }
}

// Function that runs in the background thread
void spoilageThread() {
    
    while (gameRunning) {
        
        std::this_thread::sleep_for(std::chrono::seconds(420));
        
        // Create a copy of inventory to check
        std::vector<std::string> itemsToCheck;
        {
            std::lock_guard<std::mutex> lock(inventoryMutex);
            itemsToCheck = inventory;
        }
        
        // Find food items that could spoil
        for (const auto& item : itemsToCheck) {
            if (std::find(itemsToEat.begin(), itemsToEat.end(), item) != itemsToEat.end()) {
                // 20% chance to spoil each food item
                
                    spoil(item);
                
            }
        }
    }
}

void sell(std::string item) {
    auto it = std::find(inventory.begin(), inventory.end(), item);
    if (it != inventory.end()) {
        auto itemIndex = std::find(itemsToBuy.begin(), itemsToBuy.end(), item) - itemsToBuy.begin();
        if (itemIndex < earningSell.size()) {
            money += earningSell[itemIndex];
            inventory.erase(it);
            std::cout << "You sold " << item << " for $" << earningSell[itemIndex] << ".\n";
            saveProgress();
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
                } else if (item == "Spoilt_Apple") {
                    health -= 5;
                    energy -= 5;
                    happiness -= 2;
                } else if (item == "Banana") {
                    health += 10;
                    energy += 5;
                } else if (item == "Spoilt_Banana") {
                    health -= 7;
                    energy -= 5;
                    happiness -= 4;
                }
                else if (item == "Pineapple") {
                    health += 10;
                    energy += 10;
                } else if (item == "Spoilt_Pineapple") {
                    health -= 10;
                    energy -= 10;
                    happiness -= 5;
                }
                 else if (item == "Orange") {
                    health += 15;
                    energy += 10;
                } else if (item == "Spoilt_Orange") {
                    health -= 7;
                    energy -= 5;
                    happiness -= 3;
                }
                else if (item == "Tomato") {
                    health += 15;
                    energy += 15;
                }
                else if (item == "Spoilt_Tomato") {
                    health -= 10;
                    energy -= 10;
                    happiness -= 4;
                }
                else if (item == "Chocolate") {
                    health += 20;
                    energy += 30;
                } else if (item == "Spoilt_Chocolate") {
                    health -= 10;
                    energy -= 12;
                    happiness -= 7;
                } 
                else if (item == "Milk") {
                    health += 30;
                    energy += 20;
                    happiness += 5;
                } 
                else if (item == "Spoilt_Milk") {
                    health -= 12;
                    energy -= 10;
                    happiness -= 5;
                }
                else if (item == "Cheese") {
                    health += 35;
                    energy += 25;
                } 
                else if (item == "Spoilt_Cheese") {
                    health -= 10;
                    energy -= 13;
                    happiness -= 7;
                } 
                else if (item == "Meat") {
                    health += 50;
                    energy += 40;}
                else if (item == "Spoilt_Meat") {
                    health -= 20;
                    energy -= 20;
                    happiness -= 10;
                
                } else if (item == "EnergyDrink") {
                    health += 10;
                    energy += 60;
                }  
                else if (item == "Spoilt_EnergyDrink") {
                    health -= 45;
                    energy -= 60;
                    happiness -= 25;
                }
                else if (item == "Coffee") {
                    health += 25;
                    energy += 45;
                }
                else if (item == "Spoilt_Coffee") {
                    health -= 30;
                    energy -= 45;
                    happiness -= 40;
                }
                else if (item =="Pizza"){
                    health += 35;
                    energy += 30;
                    happiness += 10;
                }
                else if (item == "Spoilt_Pizza"){
                    health -= 30;
                    energy -= 30;
                    happiness -= 60;
                }
                
                std::cout << "You ate " << item << ".\n";
                saveProgress();
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
 void clearInventory() {
    inventory.clear();
    std::cout << "Inventory cleared.\n";
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
    std::lock_guard<std::mutex> lock(inventoryMutex);
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
            else if(std::find(spoiltItems.begin(), spoiltItems.end(), item) != spoiltItems.end())
                std::cout << " (Fs)";
            else if(std::find(MiningStones.begin(), MiningStones.end(), item) != MiningStones.end())
                std::cout << " (Ms)"; // Mining stone
            else if (std::find(Treasure.begin(), Treasure.end(), item) != Treasure.end())
                std::cout << " (T)";
            else {
                std::cout << " (O)"; // Other
            }
            std::cout << "\n";
        }
    }
}
void Trash(std::string item) {
    inventory.erase(std::remove(inventory.begin(), inventory.end(), item), inventory.end());
    saveProgress();

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
    saveProgress();
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
    std::cout<<"trash - to remove an item from your inventory. For example, you have some spoiled food or unnecessary items. You can remove them from your inventory. But remember, you can't get them back.\n";
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
    std::cout<<"trash\n";
    std::cout<<"all_commands\n";
    std::cout<<"help\n";
}

// Quit the program
int quit() {
    std::cout << "Goodbye!\n";
    return 0;
}
void new_game() {
    std::cout << "New game started.\n";
    inventory.clear();
    money = 100;
    energy = 100;
     health = 100;
     happiness = 50;
     experience = 50;
     luck = 1;
    gameRunning = true;
    saveProgress();
}

int main() {
    std::cout << "Welcome to the game!\n";
    if (loadProgress() == 0) {
        std::cout << "Progress loaded.\n";
    } else {
        std::cout << "New game started.\n";
    }
    std::thread spoiler(spoilageThread);
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
                saveProgress();
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
        else if (command == "destroy") {
            clearInventory();
        }
        else if (command == "buy") {
            std::string item;
            std::cout << "What do you want to buy? \n";
            std::cin >> item;
            {
                std::lock_guard<std::mutex> lock(inventoryMutex);
                buy(item);
            }
        
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
        else if (command == "trash") {
            std::string item;
            std::cout<<"What do you want to trash? \n";
            std::cin >> item;
            Trash(item);
        }
        else if (command == "new_game") {
         std::cout<<"Are you sure you want to start a new game? \n";
         std::string answer;
         std::cin >> answer;
         if (answer == "yes" || answer == "Yes" || answer == "YES" || answer == "y" || answer == "Y") {
            new_game();
            std::cout<<"Welcome to the game.\n";
         }
         else if (answer == "no" || answer == "No" || answer == "NO" || answer == "n" || answer == "N") {
            std::cout<<"Ok, no new game started.\n";
         }
            
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
            else if (item == "HuntingRifle") {
                hunt(item);
        }
            else if (item == "Drill") {
                mine(item);
        }
            else if (item == "MysteryBox") {
                MysteryBox(item);
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
