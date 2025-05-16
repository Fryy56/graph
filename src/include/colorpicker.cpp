#include "colorpicker.hpp"
#include <QToolTip>


ColorPicker::ColorPicker(
	QColor& displayedColor,
	QColor const& borderColor,
	int borderWidth,
	int borderRadius,
	QWidget* parent
) : ColorDisplay(
	displayedColor,
	borderColor,
	borderWidth,
	borderRadius,
	parent
) {
	// Layout
	m_editLayout = new QHBoxLayout(this);
	// Label
	m_editLabel = new QLabel("Edit");
	QFont font(this -> font());
	font.setBold(true);
	font.setPointSize(12);
	m_editLabel -> setFont(font);
	QPalette pal;
	pal.setColor(QPalette::WindowText, Qt::white);
	m_editLabel -> setPalette(pal);
	m_editLabel -> setMaximumWidth(40);
	m_editLabel -> setAttribute(Qt::WA_TranslucentBackground);
	m_editLayout -> addWidget(m_editLabel);

	// Fade animation
	m_opacityEffect = new QGraphicsOpacityEffect;
	m_opacityEffect -> setOpacity(0);
	m_editLabel -> setGraphicsEffect(m_opacityEffect);
	m_editLabelOpacityAnim = new QPropertyAnimation(m_opacityEffect, "opacity");
	m_editLabelOpacityAnim -> setDuration(300);
	m_editLabelOpacityAnim -> setEasingCurve(QEasingCurve::Linear);

	// Menu
	m_colorPickerMenu = new QMenu;
	m_colorPickerWidget = new ColorPickerWidget;
	m_colorPickerWidgetAction = new QWidgetAction(m_colorPickerMenu);
	m_colorPickerWidgetAction -> setDefaultWidget(m_colorPickerWidget);
	m_colorPickerMenu -> addAction(m_colorPickerWidgetAction);
}

void ColorPicker::mousePressEvent(QMouseEvent*) {
	m_colorPickerMenu -> popup(mapToGlobal(this -> rect().bottomLeft()));

	return;
}

void ColorPicker::editLabelFadeIn() {
	m_editLabelOpacityAnim -> stop();
	m_editLabelOpacityAnim -> setStartValue(m_opacityEffect -> opacity());
	m_editLabelOpacityAnim -> setEndValue(1);
	m_editLabelOpacityAnim -> start();

	return;
}

void ColorPicker::editLabelFadeOut() {
	m_editLabelOpacityAnim -> stop();
	m_editLabelOpacityAnim -> setStartValue(m_opacityEffect -> opacity());
	m_editLabelOpacityAnim -> setEndValue(0);
	m_editLabelOpacityAnim -> start();

	return;
}

ColorPicker::~ColorPicker() {
	delete m_editLayout;

	delete m_opacityEffect;
	delete m_editLabelOpacityAnim;
	delete m_colorPickerMenu;

	delete m_colorPickerWidget;
	delete m_colorPickerWidgetAction;
}

// ---------------------------------------------------------------------------

ColorPickerWidget::ColorPickerWidget(QWidget* parent) : QWidget(parent) {
	this -> setFixedSize(300, 100);

	// Main layout
	m_mainLayout = new QHBoxLayout(this);
	// RGB|HSV layout
	m_rgbHsvLayout = new QGridLayout;
}

ColorPickerWidget::~ColorPickerWidget() {
	delete m_mainLayout;
}