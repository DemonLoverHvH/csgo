#include "mainMenu.hpp"
#include "Controls/Tab/Tab.hpp"
#include "../../globals.hpp"
#include "../../../SDK/interfaces/interfaces.hpp"

GUI::Menu::Menu(const short key)
	: m_keyActivate{ key },
	m_X{ MENU_START_X }, m_Y{ MENU_START_Y },
	m_width{ MENU_WIDTH }, m_height{ MENU_HEIGHT },
	m_offsetX{ 20 }, m_offsetY{ 40 }
{}

GUI::Tab* GUI::Menu::addTab(const std::string& name)
{
	auto t = new Tab(name);
	t->setParent(this);
	m_tabs.emplace_back(t);
	return t;
}

void GUI::Menu::skip(void* addr, Vector2D* pos)
{
	m_addrToSkip = addr;
	m_oldPos = *pos;
	if (isKeyDown(VK_LBUTTON))
	{
		m_skipClick = true;
		raiseKey(VK_LBUTTON);
	}
}

void GUI::Menu::drawAllTabs(Vector2D* pos)
{
	if (m_tabs.empty())
		return;

	// have to somehow see what tab has been pressed
	static size_t tabNow = m_selectedTab;
	if (m_selectedTab != tabNow)
	{
		tabNow = m_selectedTab;
	}

	// based on menu size, and amount of tabs, you don't have to care about giving all values to draw in selected pos
	static int size = (MENU_WIDTH / m_tabs.size()) - 8;

	// fix style of this and pos
	for (int i = 0; i < m_tabs.size(); i++)
	{
		int x = pos->x + 8 + (i * size);
		int y = pos->y + 10;
		int w = size;
		int h = TAB_HEIGHT;

		bool isInRange = isMouseInRange(x, y, w, h);
		bool isSelected = m_selectedTab == i;

		if (isInRange && isKeyPressed(VK_LBUTTON))
		{
			m_selectedTab = i;
			selectedTabName = m_tabs.at(i)->getName();
			//printf("ostatnie %i \n", globals::lastTab);
		}

		render::drawGradient(x, y, w, h,
			isSelected ? Color(50, 120, 220, 255) : Color(50, 120, 180, 255),
			isSelected ? Colors::Black : Color(50, 120, 180, 255), false);

		render::drawOutlineRect(x + 1, y + 1, w - 2, h - 2,
			isInRange ? Color(50, 120, 220, 255) : Color(50, 120, 180, 255));

		if (isInRange)
			render::drawFilledRect(x, y, w, h, Colors::Grey);

		render::drawOutlineRect(x, y, w, h, Colors::Black);

		// TODO: add better calculation based on font
		render::text(x + (size / 2), y + 3, fonts::menuFont, m_tabs.at(i)->getName(), true, isSelected ? Color(200, 170, 70, 255) : Colors::White);
	}

	pos->x += this->getStartX();
	pos->y += this->getStartY();
}

void GUI::Menu::draw()
{
	// init first name
	if (this->getSelectedName().empty() && m_tabs.size())
	{
		this->setSelectedName(m_tabs.at(0)->getName());
	}

	if (isKeyPressed(m_keyActivate))
	{
		m_opened = !m_opened;
	}

	if (!m_opened)
		return;

	static std::pair<int, int> mousePos;
	static std::pair<int, int> menuPos;

	if (isKeyDown(VK_LBUTTON))
	{
		//printf("mouse x %i, mouse y %i\n", mousePos.first, mousePos.second);

		//printf("m_X %i, m_Y %i\n", m_X, m_Y);


		if (!m_dragging && isMouseInRange(m_X, m_Y, MENU_WIDTH, MENU_HEIGHT)) // patch this with header range
		{
			m_dragging = true;
			mousePos = { globals::mouseX, globals::mouseY };
			menuPos = { m_X, m_Y };
		}
		if (m_dragging)
		{
			//printf("dragging\n");

			m_X = menuPos.first + globals::mouseX - mousePos.first;
			m_Y = menuPos.second + globals::mouseY - mousePos.second;
		}
	}
	else
		m_dragging = false;

	static int x, y;
	interfaces::engine->getScreenSize(x, y);
	m_X = std::clamp(m_X, 0 + static_cast<int>(THICKNESS), x - m_width - static_cast<int>(THICKNESS));
	m_Y = std::clamp(m_Y, 0 + static_cast<int>(THICKNESS), y - m_height - static_cast<int>(THICKNESS));

	drawBackground();
	drawExtraBackgroundLines();

	Vector2D pos = Vector2D(m_X, m_Y);

	drawAllTabs(&pos);

	// this draws content
	for (const auto& el : m_tabs)
	{
		if (el->getName() == this->getSelectedName())
		{
			el->draw(&pos);
		}
	}

	pos.x += this->getStartX();
	pos.y += this->getStartY();

	if (m_addrToSkip)
	{
		if (m_skipClick)
		{
			forceKey(VK_LBUTTON);
		}
		reinterpret_cast<Element*>(m_addrToSkip)->draw(&m_oldPos, this, true);
		m_skipClick = false;
		m_addrToSkip = nullptr;
	}
}

void GUI::Menu::drawBackground() const
{
	render::drawFilledRect(m_X, m_Y, m_width, m_height, Color(50, 50, 50, 255));
}

void GUI::Menu::drawExtraBackgroundLines() const
{
	// left
	render::drawGradient(m_X - THICKNESS, m_Y - THICKNESS, THICKNESS, m_height + (THICKNESS * 2), Colors::LightBlue, Colors::Palevioletred, false);
	// top - remove and draw some header
	//render::drawGradient(m_X, m_Y - THICKNESS, m_width, THICKNESS, Colors::LightBlue, Colors::Palevioletred, true);
	// right
	render::drawGradient(m_X + m_width, m_Y - THICKNESS, THICKNESS, m_height + (THICKNESS * 2), Colors::Palevioletred, Colors::LightBlue, false);
	// bottom
	render::drawGradient(m_X, m_Y + m_height, m_width, THICKNESS, Colors::Palevioletred, Colors::LightBlue, true);
}