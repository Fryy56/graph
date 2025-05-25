#include "highlightedlineedit.hpp"
#include <QToolTip>


QString const HighlightedLineEdit::m_defaultFieldStyleSheetTemplate = "QLineEdit { border: 2px solid %1; background-color: %2; color: %3;"; 

HighlightedLineEdit::HighlightedLineEdit(QWidget* parent) : QWidget(parent) {
	m_label = new TintingLabel; // Used in TintingLabel::setColors(), called by this -> setColors()
	this -> setColors();
	QPalette pal;
	m_mainLayout = new QVBoxLayout(this);

	// Label
	m_labelLayout = new QHBoxLayout;
	pal.setColor(QPalette::WindowText, Colors.offLabelColor);
	m_label -> setPalette(pal);
	m_labelLayout -> addSpacing(10);
	m_labelLayout -> addWidget(m_label);
	m_mainLayout -> addLayout(m_labelLayout);

	// Input field
	m_defaultFieldStyleSheet = m_defaultFieldStyleSheetTemplate
		.arg(Colors.borderColor.name())
		.arg(Colors.fieldsColor.name())
		.arg(Colors.textColor.name());
	m_customFieldStyleSheet = "";
	m_inputField = new QLineEdit;
	m_inputField -> setStyleSheet(m_defaultFieldStyleSheet + "}");
	m_inputField -> setMaximumHeight(50);
	m_inputField -> installEventFilter(this);
	m_mainLayout -> addWidget(m_inputField);
	this -> heightViaLabel(15);

	m_curBorderColor = Colors.borderColor;
	// Clear pulse
	m_clearPulse = new QPropertyAnimation(this, "borderColor");
	m_clearPulse -> setDuration(300);
	m_clearPulse -> setStartValue(Colors.onLabelColor);
	m_clearPulse -> setEndValue(Colors.borderColor);
	m_clearPulse -> setEasingCurve(QEasingCurve::Linear);

	// Generic pulse
	m_genericPulse = new QPropertyAnimation(this, "borderColor");
	m_genericPulse -> setDuration(300);
	m_genericPulse -> setEndValue(Colors.borderColor);
	m_genericPulse -> setEasingCurve(QEasingCurve::Linear);

	auto borderFocusColor = QColor(
		Colors.borderColor.red() + 60,
		Colors.borderColor.green() + 60,
		Colors.borderColor.blue() + 60
	);
	// On border animation
	m_onBorderAnim = new QPropertyAnimation(this, "borderColor");
	m_onBorderAnim -> setDuration(400);
	m_onBorderAnim -> setStartValue(Colors.borderColor);
	m_onBorderAnim -> setEndValue(borderFocusColor);
	m_onBorderAnim -> setEasingCurve(QEasingCurve::Linear);
	// Off border animation
	m_offBorderAnim = new QPropertyAnimation(this, "borderColor");
	m_offBorderAnim -> setDuration(400);
	m_offBorderAnim -> setStartValue(borderFocusColor);
	m_offBorderAnim -> setEndValue(Colors.borderColor);
	m_offBorderAnim -> setEasingCurve(QEasingCurve::Linear);
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

void HighlightedLineEdit::setPresetColors(
	QColor const& offLabelColor,
	QColor const& onLabelColor,
	QColor const& fieldsColor,
	QColor const& textColor,
	QColor const& borderColor
) {
	PresetColors.offLabelColor = offLabelColor;
	PresetColors.onLabelColor = onLabelColor;
	TintingLabel::setPresetColors(offLabelColor, onLabelColor);
	PresetColors.fieldsColor = fieldsColor;
	PresetColors.textColor = textColor;
	PresetColors.borderColor = borderColor;

	return;
}

void HighlightedLineEdit::setPresetColors(QColor const& color, HighlightedLineEdit::ColorRoles role) {
	switch (role) {
		case HighlightedLineEdit::ColorRoles::OffLabelColor:
			PresetColors.offLabelColor = color;
			break;
		case HighlightedLineEdit::ColorRoles::OnLabelColor:
			PresetColors.onLabelColor = color;
			break;
		case HighlightedLineEdit::ColorRoles::FieldsColor:
			PresetColors.fieldsColor = color;
			break;
		case HighlightedLineEdit::ColorRoles::TextColor:
			PresetColors.textColor = color;
			break;
		case HighlightedLineEdit::ColorRoles::BorderColor:
			PresetColors.borderColor = color;
			break;
		default:
			Q_UNREACHABLE();
			break;
	}

	return;
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
	m_label -> setColors(offLabelColor, onLabelColor);
	Colors.fieldsColor = fieldsColor;
	Colors.textColor = textColor;
	Colors.borderColor = borderColor;

	return;
}

void HighlightedLineEdit::setColors(QColor const& color, HighlightedLineEdit::ColorRoles role) {
	switch (role) {
		case HighlightedLineEdit::ColorRoles::OffLabelColor:
			Colors.offLabelColor = color;
			m_label -> setColors(color, TintingLabel::ColorRoles::OffColor);
			break;
		case HighlightedLineEdit::ColorRoles::OnLabelColor:
			Colors.onLabelColor = color;
			m_label -> setColors(color, TintingLabel::ColorRoles::OnColor);
			break;
		case HighlightedLineEdit::ColorRoles::FieldsColor:
			Colors.fieldsColor = color;
			break;
		case HighlightedLineEdit::ColorRoles::TextColor:
			Colors.textColor = color;
			break;
		case HighlightedLineEdit::ColorRoles::BorderColor:
			Colors.borderColor = color;
			break;
		default:
			Q_UNREACHABLE();
			break;
	}

	return;
}

void HighlightedLineEdit::pulse(QColor const& pulseColor) {
	m_genericPulse -> stop();
	m_genericPulse -> setStartValue(pulseColor);
	m_genericPulse -> start();

	return;
}

QString HighlightedLineEdit::getShownText() const {
	auto ret = m_inputField -> text();
	if (ret.isEmpty())
		return m_inputField -> placeholderText();
	else
		return ret;
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
		if (event -> type() == QEvent::FocusIn) {
			m_label -> onSelect();
			this -> onSelect();
		}
		else if (event -> type() == QEvent::FocusOut) {
			m_label -> onDeselect();
			this -> onDeselect();
		}
	}
	return false;
}

void HighlightedLineEdit::propSetBorderColor(QColor const& color) {
	m_curBorderColor = color;
	m_defaultFieldStyleSheet = m_defaultFieldStyleSheetTemplate
		.arg(m_curBorderColor.name()).arg(Colors.fieldsColor.name()).arg(Colors.textColor.name());
	m_inputField -> setStyleSheet(m_defaultFieldStyleSheet + m_customFieldStyleSheet + "}");

	return;
}

void HighlightedLineEdit::clearWithPulse() {
	m_inputField -> clear();
	m_clearPulse -> stop();
	m_clearPulse -> start();

	return;
}

HighlightedLineEdit::~HighlightedLineEdit() {
	delete m_mainLayout;

	delete m_clearPulse;
	delete m_genericPulse;
	delete m_onBorderAnim;
	delete m_offBorderAnim;
}