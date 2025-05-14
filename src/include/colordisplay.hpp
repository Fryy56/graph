#pragma once

#include <QWidget>


class ColorDisplay : public QWidget {
	Q_OBJECT

public:
	explicit ColorDisplay(
		QColor& displayedColor,
		QColor const& borderColor = Qt::black,
		int borderWidth = 3,
		int borderRadius = 10,
		QWidget* parent = nullptr
	);
	~ColorDisplay() override {}

protected:
	void paintEvent(QPaintEvent*) override;

	// Fields
	QColor& m_displayedColor;
	QColor m_borderColor;
	int m_borderWidth;
	int m_borderRadius;
};