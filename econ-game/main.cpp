#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class Game;
class Player;
class Property;


class Player{
private:
  vector<Property> owned_properties;
  float money = 0;
public:
  // getters
  float GetMoney(){
    return money;
  }
  // setters
  void AddMoney(float amount){
    money += amount;
  }
  void SubtractMoney(float amount){
    money -= amount;
  }
  void SetMoney(float amount){
    money = amount;
  }
};

class Property {
private:
  float buy_value = 0;
  float sell_value = 0;
  float money_per_turn = 0;
  float cost_per_turn = 0;
  string property_name = "";
  // TODO: Implement randomness
  void GenerateRandom(){
    property_name = "Lemonade stand";
    money_per_turn = 0.2f;
    buy_value = 40;
    sell_value = 5;
  }
public:
  Property(bool random = true){
    if(random){
      GenerateRandom();
    }
  }
  void GenerateMoney(Player* pplayer){
    pplayer->AddMoney(money_per_turn);
  }
};

class Game {
private:
  vector<Property> available_properties;
  int game_iterations = 0;
  bool game_over = false;
  Player* pplayer = nullptr;
  void Debug_PrintProperties(){

  }
public:
  string last_input = "";
  // Game creation script
  Game(){
    pplayer = new Player();

    Property testProperty1_lemonadeStand{};
    available_properties.push_back(testProperty1_lemonadeStand);
  }
  void DrawUI(){
    cout << "\n\n\n";
    cout << "Money: ";
    cout << pplayer->GetMoney() << "$";
    cout << "\nOptions";
    cout << "1. Manage property\n";
  } 
  
  void BuyNewProperty(){
    
  }
  void ManagePropertyOption(){
    // Draw UI
    cout << "\n\n\n";
    cout << "- Manage property\n";
    cout << "Options\n";
    cout << "1. Buy new property\n";
    cout << "2. Manage owned property\n";
    cout << "3. Return\n";
    // Manage options
    string selected_option = "";
    while(true){
      cin >> selected_option;
      if(selected_option == "3"){
        break;
      }
      if(selected_option == "1"){
        BuyNewProperty();
      }
   }
  }
  // Called after check_input
  void CheckInput(){
    if(last_input == "1"){
      ManagePropertyOption();
      return;
    }
  }

  // getters
  int GetGameIterations(){
    return game_iterations;
  }
  bool GetGameOver() {
    return !game_over;
  }
  // setters
  void IncrementGameIterations(){
    game_iterations++;
  }
};



// Main MainLoop
bool MainLoop(Game* pgame){
  // Get input
  cin >> pgame->last_input; 
  pgame->CheckInput();
  
  return pgame->GetGameOver();
}

int main (int argc, char *argv[]) {
  Game* pgame = new Game();

  while(MainLoop(pgame));
  return 0;
}
