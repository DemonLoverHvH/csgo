#include "Slider.hpp"
#include "../../../../globals.hpp"
#include <format>

GUI::Slider::Slider(const std::string& title, const std::pair<float, float>& minMax, float* value)
	: m_title{ title }, m_minMax{ minMax }, m_value{ value }
{
	setElement(SILDER_WIDTH, SLIDER_HEIGHT);
	setPadding(30);
}

void GUI::Slider::draw(Vector2D* pos, Menu* parent, bool skipCall)
{
	bool isInRange = isMouseInRange(pos->x, pos->y, m_width, m_height);
	// needed for further calculations
	const float deltaBetween = m_minMax.second - m_minMax.first;

	// some floating point

	auto copy = *m_value;

	std::string toDraw = std::format("{} - {:.2f}", m_title, *m_value);

	render::text(pos->x, pos->y - 10, fonts::menuFont, toDraw, false, Colors::White);

	// run master active switch
	static bool toChange = false;
	static auto check = [this]() -> void
	{
		m_active = !m_active;
		toChange = false;
	};

	if (isKeyPressed(VK_LBUTTON) && isInRange)
	{
		if (toChange)
			check();
	}
	else if (isInRange)
	{
		toChange = true;
	}

	// if we are in range and pressed, then change slider with current delta
	if (isKeyDown(VK_LBUTTON) && this->isActive())
	{
		*m_value = m_minMax.first + deltaBetween * (globals::mouseX - pos->x) / m_width;
	}
	else if (!isKeyDown(VK_LBUTTON) && !isInRange && this->isActive())
	{
		m_active = false;
	}


	// do not go out of ranges
	*m_value = std::clamp(*m_value, m_minMax.first, m_minMax.second);

	// how many pixels to move, or call it offset.
	const float padX = ((*m_value - m_minMax.first) / deltaBetween) * m_width;

	render::drawFilledRect(pos->x, pos->y, m_width, m_height, Colors::LightBlue);
	render::drawGradient(pos->x + 1, pos->y + 1, padX - 2, m_height - 2, Colors::LightBlue, Color(30, 30, 30, 255), false);

	if (this->isActive())
		render::drawFilledRect(pos->x, pos->y, m_width, m_height, Color(90, 90, 90, 125));

	render::drawOutlineRect(pos->x, pos->y, m_width, m_height, Colors::Black);

	pos->y += this->getPadding();
}

bool GUI::Slider::isActive() const
{
	return m_active;
}