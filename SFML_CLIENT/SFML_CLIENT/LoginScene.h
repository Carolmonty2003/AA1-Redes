#pragma once
#include "Scene.h"
#include "NetworkManager.h"
#include <iostream>
#include <functional>
#include <SFML/Graphics.hpp>
#include "SceneManager.h"
#include <string>
#include "Button.h"
#include "InputField.h"

class LoginScene: public Scene
{
private:
    sf::Font font;
    InputField* usernameInputfield;
    InputField* passwordInputfield;
    Button* loginButton;
    Button* signinButton;
    Button* closeButton;

public:
    LoginScene()
    {
        if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            std::cerr << "Warning: Failed to open arial.ttf font in Lobby" << std::endl;
        }
        usernameInputfield = new InputField(250, 220, 300, 25, font);
        passwordInputfield = new InputField (250, 250, 300, 25, font);
        loginButton = new Button(350, 280, 100, 25, font);
        loginButton->SetText("Log In");
        signinButton= new Button(350, 310, 100, 25, font);
        signinButton->SetText("Register");
        closeButton = new Button(800 - 64, 0, 64, 64, font);
        closeButton->SetText("X");
        closeButton->onClick = [](){
            
            SM.window.close();
        };
        signinButton->onClick = [this]() {
            std::string user = usernameInputfield->getText();
            std::string pass = passwordInputfield->getText();
            NM.SendRegisterRequest(user, pass);
        };
        loginButton->onClick = [this]() {
            PacketType pt = PacketType::LOGIN_REQUEST;
            std::string user = usernameInputfield->getText();
            std::string pass = passwordInputfield->getText();
            NM.SendLoginRequest(user, pass);
        };
    }

    void OnEnter() override
    {
        std::cout << "Entrando a la LoginScene..." << std::endl;
        
    }

    void HandleEvent(const sf::Event& event) override
    {
        closeButton->handleEvent(event);
        usernameInputfield->handleEvent(event);
        passwordInputfield->handleEvent(event);
        loginButton->handleEvent(event);
        signinButton->handleEvent(event);
    }

    void Update(float dt) override
    {
        NM.NetworkFetch();
        if (NM.GetClientState().IsLoggedIn())
            SM.SetNextScene("LobbyScene");
    }

    void Render(sf::RenderWindow& window) override
    {
        closeButton->Draw(window); 
        usernameInputfield->Draw(window);
        passwordInputfield->Draw(window);
        loginButton->Draw(window);
        signinButton->Draw(window);
    }

    void OnExit() override
    {
        std::cout << "Saliendo del Lobby..." << std::endl;
        delete loginButton;
        delete signinButton;
        delete usernameInputfield;
        delete passwordInputfield;
        delete closeButton;
    }


    void AskCreateRoom();

    void AskDeleteRoom();

    void GetRanking();
};

