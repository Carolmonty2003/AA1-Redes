#include "RankingScene.h"
#include "Constants.h"

RankingScene::RankingScene()
{
    if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cerr << "Warning: Failed to open arial.ttf font in Ranking" << std::endl;
    }
}

void RankingScene::BuildUI()
{
    backButton = std::make_unique<Button>(
        200.f, 500.f, 400.f, 50.f, font
    );
    backButton->SetText("Volver al Lobby");
    backButton->onClick = [this]() {
        SM.SetNextScene("LobbyScene");
    };
}

void RankingScene::OnEnter()
{
    BuildUI();

    // ejemplo datos
    rankingData.clear();
    rankingData.push_back({"Player1", 1500});
    rankingData.push_back({"Player2", 1200});
    rankingData.push_back({"Player3", 1000});
}

void RankingScene::HandleEvent(const sf::Event& event)
{
    if (backButton) backButton->handleEvent(event);
}

void RankingScene::Update(float dt)
{
}

void RankingScene::Render(sf::RenderWindow& window)
{
    sf::Text title(font);
    title.setCharacterSize(40);
    title.setPosition({Config::Ranking::TITLE_X, Config::Ranking::TITLE_Y});
    title.setString("RANKING");
    title.setFillColor(sf::Color::Cyan);
    window.draw(title);

    float yPos = Config::Ranking::TEXT_Y;
    int position = 1;
    for (const auto& entry : rankingData)
    {
        sf::Text rankText(font);
        rankText.setCharacterSize(20);
        rankText.setFillColor(sf::Color::White);
        rankText.setPosition({ Config::Ranking::TEXT_X, yPos });

        std::string rankStr = std::to_string(position) + ". " + entry.first + " - " + std::to_string(entry.second);
        rankText.setString(rankStr);
        window.draw(rankText);

        yPos += 50.f;
        position++;
    }

    if (backButton) backButton->Draw(window);
}

void RankingScene::OnExit()
{
}