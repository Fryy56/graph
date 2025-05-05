#include "highlightedlineedit.hpp"


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
	m_inputField = new QLineEdit;
	pal.setColor(QPalette::Base, Colors.fieldsColor);
	pal.setColor(QPalette::Text, Colors.textColor);
	m_inputField -> setPalette(pal);
	m_inputField -> setMaximumHeight(50);
	m_inputField -> installEventFilter(this);
	m_mainLayout -> addWidget(m_inputField);
	this -> heightViaLabel(15);
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
	QColor const& textColor
) {
	Colors.offLabelColor = offLabelColor;
	Colors.onLabelColor = onLabelColor;
	TintingLabel::setColors(offLabelColor, onLabelColor);
	Colors.fieldsColor = fieldsColor;
	Colors.textColor = textColor;

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

bool HighlightedLineEdit::eventFilter(QObject* object, QEvent* event) {
	if (object == m_inputField) {
		if (event -> type() == QEvent::FocusIn)
			m_label -> onSelect();
		else if (event -> type() == QEvent::FocusOut)
			m_label -> onDeselect();
	}
	return false;
}

HighlightedLineEdit::~HighlightedLineEdit() {
	delete m_mainLayout;
		delete m_labelLayout;
			delete m_label;
		delete m_inputField;
}