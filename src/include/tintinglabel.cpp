#include "tintinglabel.hpp"


TintingLabel::TintingLabel(QWidget* parent) : QLabel(parent) {
	this -> setColors();

	// Animations
	m_toOnAnim = new QPropertyAnimation(this, "color");
	m_toOnAnim -> setDuration(400);
	m_toOnAnim -> setStartValue(QColor(Colors.offColor));
	m_toOnAnim -> setEndValue(QColor(Colors.onColor));
	m_toOnAnim -> setEasingCurve(QEasingCurve::Linear);

	m_toOffAnim = new QPropertyAnimation(this, "color");
	m_toOffAnim -> setDuration(400);
	m_toOffAnim -> setStartValue(QColor(Colors.onColor));
	m_toOffAnim -> setEndValue(QColor(Colors.offColor));
	m_toOffAnim -> setEasingCurve(QEasingCurve::Linear);
}

TintingLabel::TintingLabel(QString const& text, QWidget* parent) : TintingLabel(parent) {
	this -> setText(text);
}

void TintingLabel::setPresetColors(QColor const& offColor, QColor const& onColor) {
	PresetColors.offColor = offColor;
	PresetColors.onColor = onColor;

	return;
}

void TintingLabel::setPresetColors(QColor const& color, TintingLabel::ColorRoles role) {
	switch (role) {
		case TintingLabel::ColorRoles::OffColor:
			PresetColors.offColor = color;
			break;
		case TintingLabel::ColorRoles::OnColor:
			PresetColors.onColor = color;
			break;
		default:
			Q_UNREACHABLE();
			break;
	}

	return;
}

void TintingLabel::setColors(QColor const& offColor, QColor const& onColor) {
	Colors.offColor = offColor;
	Colors.onColor = onColor;

	return;
}

void TintingLabel::setColors(QColor const& color, TintingLabel::ColorRoles role) {
	switch (role) {
		case TintingLabel::ColorRoles::OffColor:
			Colors.offColor = color;
			break;
		case TintingLabel::ColorRoles::OnColor:
			Colors.onColor = color;
			break;
		default:
			Q_UNREACHABLE();
			break;
	}

	return;
}

void TintingLabel::propSetColor(QColor const& color) {
	QPalette pal;
	pal.setColor(QPalette::WindowText, color);
	this -> setPalette(pal);

	return;
}

TintingLabel::~TintingLabel() {
	delete m_toOnAnim;
	delete m_toOffAnim;
}