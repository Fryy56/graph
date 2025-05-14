#include "highlightedlineedit.hpp"
#include <QToolTip>


QString const HighlightedLineEdit::m_defaultFieldStyleSheetTemplate = "QLineEdit { border: 2px solid %1;"; 

HighlightedLineEdit::HighlightedLineEdit(QWidget* parent) : QWidget(parent) {
	QPalette pal;
	m_mainLayout = new QVBoxLayout(this);

	// Label
	m_labelLayout = new QHBoxLayout;
	m_label = new TintingLabel;
	pal.setColor(QPalette::WindowText, Colors.offLabelColor);
	m_label -> setPalette(pal);
	m_labelLayout -> addSpacing(10);
	m_labelLayout -> addWidget(m_label);
	m_mainLayout -> addLayout(m_labelLayout);

	// Input field
	m_defaultFieldStyleSheet = m_defaultFieldStyleSheetTemplate.arg(Colors.borderColor.name());
	m_customFieldStyleSheet = "";
	m_inputField = new QLineEdit;
	pal.setColor(QPalette::Base, Colors.fieldsColor);
	pal.setColor(QPalette::Text, Colors.textColor);
	m_inputField -> setPalette(pal);
	m_inputField -> setStyleSheet(m_defaultFieldStyleSheet + "}");
	m_inputField -> setMaximumHeight(50);
	m_inputField -> installEventFilter(this);
	m_mainLayout -> addWidget(m_inputField);
	this -> heightViaLabel(15);

	// Clear pulse
	m_curBorderColor = Colors.borderColor;
	m_clearPulse = new QPropertyAnimation(this, "borderColor");
	m_clearPulse -> setDuration(300);
	m_clearPulse -> setStartValue(Colors.onLabelColor);
	m_clearPulse -> setEndValue(Colors.borderColor);
	m_clearPulse -> setEasingCurve(QEasingCurve::Linear);
}

HighlightedLineEdit::HighlightedLineEdit(
	QString const& labelText,
	QWidget* parent
) : HighlightedLineEdit(parent) {
	m_label -> setText(labelText);
}

HighlightedLineEdit::HighlightedLineEdit(
	QString const& labelText,
	QString const& placeholderText,
	QWidget* parent
) : HighlightedLineEdit(labelText, parent) {
	m_inputField -> setPlaceholderText(placeholderText);
}

void HighlightedLineEdit::setColors(
	QColor const& offLabelColor,
	QColor const& onLabelColor,
	QColor const& fieldsColor,
	QColor const& textColor,
	QColor const& borderColor
) {
	Colors.offLabelColor = offLabelColor;
	Colors.onLabelColor = onLabelColor;
	TintingLabel::setColors(offLabelColor, onLabelColor);
	Colors.fieldsColor = fieldsColor;
	Colors.textColor = textColor;
	Colors.borderColor = borderColor;

	return;
}

void HighlightedLineEdit::heightViaLabel(int height) {
	m_label -> setMaximumHeight(height);
	this -> setMaximumHeight(m_label -> maximumHeight() + m_inputField -> maximumHeight());
	
	return;
}

void HighlightedLineEdit::widthViaField(int width) {
	m_inputField -> setMaximumWidth(width);
	this -> setMaximumWidth(m_inputField -> maximumWidth());

	return;
}

void HighlightedLineEdit::setFieldStyleSheetSafe(QString const& styleSheet) {
	m_customFieldStyleSheet = styleSheet;
	m_inputField -> setStyleSheet(m_defaultFieldStyleSheet + m_customFieldStyleSheet + "}");

	return;
}

bool HighlightedLineEdit::eventFilter(QObject* object, QEvent* event) {
	if (object == m_inputField) {
		if (event -> type() == QEvent::FocusIn)
			m_label -> onSelect();
		else if (event -> type() == QEvent::FocusOut)
			m_label -> onDeselect();
	}
	return false;
}

void HighlightedLineEdit::propSetBorderColor(QColor const& color) {
	m_curBorderColor = color;
	m_defaultFieldStyleSheet = m_defaultFieldStyleSheetTemplate.arg(color.name());
	m_inputField -> setStyleSheet(m_defaultFieldStyleSheet + m_customFieldStyleSheet + "}");

	return;
}

void HighlightedLineEdit::clearWithPulse() {
	m_inputField -> clear();
	m_clearPulse -> stop();
	m_clearPulse -> start();

	return;
}

void HighlightedLineEdit::pulse() {
	m_clearPulse -> stop();
	m_clearPulse -> start();

	return;
}

HighlightedLineEdit::~HighlightedLineEdit() {
	delete m_mainLayout;

	delete m_clearPulse;
}