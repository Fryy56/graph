#include "colordisplay.hpp"
#include <QPainter>
#include <QPainterPath>


ColorDisplay::ColorDisplay(
	QColor& displayedColor,
	QColor const& borderColor,
	int borderWidth,
	int borderRadius,
	QWidget* parent
) : QWidget(parent),
	m_displayedColor(displayedColor),
	m_borderColor(borderColor),
	m_borderWidth(borderWidth),
	m_borderRadius(borderRadius)
{}

void ColorDisplay::paintEvent(QPaintEvent*) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QPen pen;
	pen.setWidth(m_borderWidth);
	pen.setColor(m_borderColor);
	painter.setPen(pen);
	painter.setBrush(m_displayedColor);
	painter.drawRoundedRect(
		m_borderWidth,
		m_borderWidth,
		this -> width() - 2 * m_borderWidth,
		this -> height() - 2 * m_borderWidth,
		m_borderRadius,
		m_borderRadius
	);

	return;
}