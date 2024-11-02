//./game.out < inputs/gigantica.in

// include libraries here (STL not allowed)
#include <iostream>
#include <string>
#include "trie.h"


int main(){
    
    Trie t{};

    std::string command{};
    std::string word{};

    while(true){
        std::cin >> command;
        if(command=="load"){
            t.load();
        }else if(command=="i"){
            std::cin >> word;
            if(t.insert(word)){
                std::cout << "success" << std::endl;
            }else{
                std::cout << "failure" << std::endl;
            }
        }else if(command=="c"){
             std::cin >> word;
             t.count(word);
        }else if(command=="e"){
             std::cin >> word;
             t.erase(word);
        }else if(command=="p"){
            t.print();
        }else if(command=="spellcheck"){
             std::cin >> word;
             t.spellcheck(word);
        }else if(command=="empty"){
            t.empty();
        }else if(command=="clear"){
            if(t.clear()){
                std::cout << "success" << std::endl;
            }
        }else if(command=="size"){
            t.size();
        }else if(command=="exit"){
            break;
        }
    }
}