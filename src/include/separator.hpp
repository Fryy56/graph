#pragma once

#include <QFrame>


class Separator final : public QFrame {
	Q_OBJECT

public:
	enum Orientation {
		H = 0,
		V = 1
	};

	explicit Separator(
		Orientation orientation = Orientation::H,
		QWidget* parent = nullptr
	);
	explicit Separator(
		int fadeLength,
		Orientation orientation = Orientation::H,
		QWidget* parent = nullptr
	);
	explicit Separator(
		int fadeLength,
		int width,
		Orientation orientation = Orientation::H,
		QWidget* parent = nullptr
	);
	~Separator() override {}

	// Setters
	void setColor(QColor const& color);
	void setWidth(int width);
	void setFadeLength(int fadeLength);

private:
	void paintEvent(QPaintEvent*) override;

	void setGeneralWidth(int width) { m_orientation ? this -> setFixedWidth(width) : this -> setFixedHeight(width); } // vertical : horizontal
	int getLength() { return (m_orientation ? this -> height() : this -> width()); } // vertical : horizontal

	// Fields
	QColor m_color = Qt::white;
	int m_width = 3;
	int m_fadeLength = 50;
	int m_staticFadeLength = 50;
	Orientation m_orientation;
};