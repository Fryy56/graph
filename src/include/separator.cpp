#include "separator.hpp"
#include <QPainter>
#include <QLinearGradient>


Separator::Separator(
	Orientation orientation,
	QWidget* parent
) : QFrame(parent), m_orientation(orientation) {
	this -> setGeneralWidth(m_width);
}

Separator::Separator(
	int fadeLength,
	Orientation orientation,
	QWidget* parent
) : Separator(orientation, parent) {
	this -> setFadeLength(fadeLength);
}

Separator::Separator(
	int fadeLength,
	int width,
	Orientation orientation,
	QWidget* parent
) : Separator(fadeLength, orientation, parent) {
	this -> setGeneralWidth(width);
}

void Separator::setColor(QColor const& color) {
	m_color = color;
	this -> update();

	return;
}

void Separator::setWidth(int width) {
	m_width = width;
	this -> setGeneralWidth(width);
	this -> update();

	return;
}

void Separator::setFadeLength(int fadeLength) {
	m_staticFadeLength = fadeLength;
	this -> update();

	return;
}

void Separator::paintEvent(QPaintEvent*) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	int totalLength = this -> getLength();
	m_fadeLength = m_staticFadeLength;
	int solidLength = totalLength - (2 * m_fadeLength);
	if (solidLength < 3) {
		solidLength = 3;
		m_fadeLength = totalLength / 2;
	}

	auto grad1 = m_orientation ?
		QLinearGradient(0, 0, 0, m_fadeLength) // vertical
		:
		QLinearGradient(0, 0, m_fadeLength, 0); // horizontal
	auto grad2 = m_orientation ?
		QLinearGradient(0, totalLength - m_fadeLength, 0, totalLength) // vertical
		:
		QLinearGradient(totalLength - m_fadeLength, 0, totalLength, 0); // horizontal
	grad1.setColorAt(0, Qt::transparent);
	grad1.setColorAt(1, m_color);
	grad2.setColorAt(0, m_color);
	grad2.setColorAt(1, Qt::transparent);

	m_orientation ?
		painter.fillRect(0, 0, m_width, m_fadeLength, grad1) // vertical
		:
		painter.fillRect(0, 0, m_fadeLength, m_width, grad1); // horizontal
	m_orientation ?
		painter.fillRect(0, m_fadeLength, m_width, solidLength, m_color) // vertical
		:
		painter.fillRect(m_fadeLength, 0, solidLength, m_width, m_color); // horizontal
	m_orientation ?
		painter.fillRect(0, totalLength - m_fadeLength, m_width, m_fadeLength, grad2) // vertical
		:
		painter.fillRect(totalLength - m_fadeLength, 0, m_fadeLength, m_width, grad2); // horizontal


	return;
}