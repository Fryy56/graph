#include "tintinglabel.h"

TintingLabel::TintingLabel(QWidget* parent = nullptr) : QLabel(parent) {
	m_toWhiteAnim = new QPropertyAnimation(this, "color");
	m_toWhiteAnim -> setDuration(400);
	m_toWhiteAnim ->
}

void TintingLabel::setColor(QColor& color) {
	QPalette pal;
	pal.setColor(QPalette::WindowText, color);
	this -> setPalette(pal);

	return;
}

TintingLabel::~TintingLabel() {
	delete m_toWhiteAnim;
	delete m_toGrayAnim;
}