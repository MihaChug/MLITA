#include "stdafx.h"
#include "AppWindow.h"
#include "tinyfiledialogs.h"



using namespace sf;
namespace
{
const unsigned WINDOW_WIDTH = 800;
const unsigned WINDOW_HEIGHT = 600;
const unsigned MENU_BAR_HEIGHT = 20;
const unsigned WINDOW_STYLE = sf::Style::Titlebar | sf::Style::Close;
const int FRAME_SWITCH_INTERVAL = 1000;
}

CAppWindow::CAppWindow()
    : sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Point in Triangle", WINDOW_STYLE)
{
    m_menu = std::make_unique<CAppMenu>("File");
    m_menu->SetFrame(sf::FloatRect(0, 0, float(WINDOW_WIDTH), float(MENU_BAR_HEIGHT)));
    m_openActionId = m_menu->AddAction("Open...", std::bind(&CAppWindow::AskOpenInput, this));
    m_saveActionId = m_menu->AddAction("Save...", std::bind(&CAppWindow::AskSaveOutput, this));

    setFramerateLimit(60);
    setVerticalSyncEnabled(true);
    SetState(State::WaitingInput);

    m_font.loadFromFile("res/Ubuntu-R.ttf");
	InitializeText();
	m_view = getDefaultView();
}

CAppWindow::~CAppWindow()
{
    m_menu.reset();
}

void CAppWindow::EnterLoop()
{
    while (isOpen())
    {
        sf::Event event;
        while (pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                close();
                return;
            }
            m_menu->OnEvent(event);
        }

		clear(sf::Color::White);
		
		draw(*m_menu);
		if (m_hasParser)
		{
			m_pointAndTriangle->Visualisation();
			draw(*m_pointAndTriangle);
		}
		
		display();
    }
}

void CAppWindow::SetState(CAppWindow::State state)
{
    m_state = state;
    m_menu->SetActionEnabled(m_openActionId, state != State::RunningDemo);
    m_menu->SetActionEnabled(m_saveActionId, state == State::WaitingOutput);
}

void CAppWindow::AskOpenInput()
{
    const char *filters[] = { "*.txt" };
    char const *result = tinyfd_openFileDialog("Select input file", "", 1, filters, "", false);
    // Пользователь отменил выбор файла.
    if (result == nullptr)
    {
        return;
    }
   
    std::ifstream in(result);
    if (!in.is_open())
    {
        tinyfd_messageBox("Error", "I/O error when reading input file", "ok", "error", 1);
    }
    else
    {
		m_pointAndTriangle = std::make_shared<CPointAndTriangle>(CPointAndTriangle(in));
		RunAlgorithm();
		SetState(State::WaitingOutput);
    }
}

void CAppWindow::AskSaveOutput()
{
    const char *filters[] = { "*.txt" };
    char const *result = tinyfd_saveFileDialog("Select output file", "", 1, filters, "");
    // Пользователь отменил выбор файла.
    if (result == nullptr)
    {
        return;
    }

    std::ofstream out(result);
    if (!out.is_open())
    {
        tinyfd_messageBox("Error", "I/O error when writing output file", "ok", "error", 1);
    }
    else
    {
		WriteToFile(out);
        tinyfd_messageBox("Success", "File saved OK", "ok", "info", 1);
    }
}



void CAppWindow::InitializeText()
{
	//m_fontForText.loadFromFile("res/myFont.otf");
	m_text = sf::Text("", m_fontForText, 12);
	m_text.setColor(sf::Color::Black);
}

void CAppWindow::RunAlgorithm()
{
	m_pointAndTriangle->Visualisation();

	m_hasParser = true;
}

void CAppWindow::WriteToFile(std::ofstream & fout)
{
	if (m_pointAndTriangle->CheckPoint())
	{
		fout << "In";
	}
	else
	{
		fout << "Out" << std::endl;
	}

}
